/**
******************************************************************************
*
* @file       pureimagecache.cpp
* @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
* @brief      
* @see        The GNU Public License (GPL) Version 3
* @defgroup   OPMapWidget
* @{
* 
*****************************************************************************/
/* 
* This program is free software; you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 3 of the License, or 
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
* for more details.
* 
* You should have received a copy of the GNU General Public License along 
* with this program; if not, write to the Free Software Foundation, Inc., 
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include "pureimagecache.h"
#include <QDateTime>
//#include <QSettings>
#include <QDir>
#include <QMutexLocker>
#include "opmaps.h"
//#include "appcore.h"
//#include "coresettings.h"
//#define DEBUG_PUREIMAGECACHE
namespace core {
static QMutex addDatabaseMutex; // QSqlDatabase::addDatabase is not thread safe when loadingn plugins

qlonglong PureImageCache::ConnCounter=0;

PureImageCache::PureImageCache()
{

}

void PureImageCache::setGtileCache(const QString &value)
{
    rw_lock.lockForWrite();
    gtilecache=value;
    QDir d;
    if(!d.exists(gtilecache))
    {
        d.mkdir(gtilecache);
#ifdef DEBUG_PUREIMAGECACHE
        qDebug()<<"Create Cache directory";
#endif //DEBUG_PUREIMAGECACHE
    }
    {
        QString db=gtilecache+"Data.qmdb";
        if(!QFileInfo(db).exists())
        {
#ifdef DEBUG_PUREIMAGECACHE
            qDebug()<<"Try to create EmptyDB";
#endif //DEBUG_PUREIMAGECACHE
            CreateEmptyDB(db);
        }
    }
    rw_lock.unlock();
}
QString PureImageCache::GtileCache()
{
    return gtilecache;
}


bool PureImageCache::CreateEmptyDB(const QString &file)
{
#ifdef DEBUG_PUREIMAGECACHE
    qDebug()<<"Create database at!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!:"<<file;
#endif //DEBUG_PUREIMAGECACHE
    QFileInfo File(file);
    QDir dir=File.absoluteDir();
    QString path=dir.absolutePath();
    QString filename=File.fileName();
    if(File.exists())
        QFile(filename).remove();
    if(!dir.exists())
    {
#ifdef DEBUG_PUREIMAGECACHE
        qDebug()<<"CreateEmptyDB: Cache path doesn't exist, try to create";
#endif //DEBUG_PUREIMAGECACHE
        if(!dir.mkpath(path))
        {
#ifdef DEBUG_PUREIMAGECACHE
            qDebug()<<"CreateEmptyDB: Could not create path";
#endif //DEBUG_PUREIMAGECACHE
            return false;
        }
    }
    addDatabaseMutex.lock();
    QSqlDatabase db(QSqlDatabase::addDatabase("QSQLITE",QLatin1String("CreateConn")));
    addDatabaseMutex.unlock();
    db.setDatabaseName(file);
    if (!db.open())
    {
#ifdef DEBUG_PUREIMAGECACHE
        qDebug()<<"CreateEmptyDB: Unable to create database";
#endif //DEBUG_PUREIMAGECACHE

        return false;
    }
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS Tiles (id INTEGER NOT NULL PRIMARY KEY, X INTEGER NOT NULL, Y INTEGER NOT NULL, Zoom INTEGER NOT NULL, Type INTEGER NOT NULL,Date TEXT)");
    if(query.numRowsAffected()==-1)
    {
#ifdef DEBUG_PUREIMAGECACHE
        qDebug()<<"CreateEmptyDB: "<<query.lastError().driverText();
#endif //DEBUG_PUREIMAGECACHE
        db.close();
        return false;
    }
    query.exec("CREATE TABLE IF NOT EXISTS TilesData (id INTEGER NOT NULL PRIMARY KEY CONSTRAINT fk_Tiles_id REFERENCES Tiles(id) ON DELETE CASCADE, Tile BLOB NULL)");
    if(query.numRowsAffected()==-1)
    {
#ifdef DEBUG_PUREIMAGECACHE
        qDebug()<<"CreateEmptyDB: "<<query.lastError().driverText();
#endif //DEBUG_PUREIMAGECACHE
        db.close();
        return false;
    }
    query.exec(
                "CREATE TRIGGER fki_TilesData_id_Tiles_id "
                "BEFORE INSERT ON [TilesData] "
                "FOR EACH ROW BEGIN "
                "SELECT RAISE(ROLLBACK, 'insert on table TilesData violates foreign key constraint fki_TilesData_id_Tiles_id') "
                "WHERE (SELECT id FROM Tiles WHERE id = NEW.id) IS NULL; "
                "END");
    if(query.numRowsAffected()==-1)
    {
#ifdef DEBUG_PUREIMAGECACHE
        qDebug()<<"CreateEmptyDB: "<<query.lastError().driverText();
#endif //DEBUG_PUREIMAGECACHE
        db.close();
        return false;
    }
    query.exec(
                "CREATE TRIGGER fku_TilesData_id_Tiles_id "
                "BEFORE UPDATE ON [TilesData] "
                "FOR EACH ROW BEGIN "
                "SELECT RAISE(ROLLBACK, 'update on table TilesData violates foreign key constraint fku_TilesData_id_Tiles_id') "
                "WHERE (SELECT id FROM Tiles WHERE id = NEW.id) IS NULL; "
                "END");
    if(query.numRowsAffected()==-1)
    {
#ifdef DEBUG_PUREIMAGECACHE
        qDebug()<<"CreateEmptyDB: "<<query.lastError().driverText();
#endif //DEBUG_PUREIMAGECACHE
        db.close();
        return false;
    }
    query.exec(
                "CREATE TRIGGER fkdc_TilesData_id_Tiles_id "
                "BEFORE DELETE ON Tiles "
                "FOR EACH ROW BEGIN "
                "DELETE FROM TilesData WHERE TilesData.id = OLD.id; "
                "END");
    if(query.numRowsAffected()==-1)
    {
#ifdef DEBUG_PUREIMAGECACHE
        qDebug()<<"CreateEmptyDB: "<<query.lastError().driverText();
#endif //DEBUG_PUREIMAGECACHE
        db.close();
        return false;
    }
    db.close();
    QSqlDatabase::removeDatabase(QLatin1String("CreateConn"));
    return true;
}
bool PureImageCache::PutImageToCache(const QByteArray &tile, const MapType::Types &type,const Point &pos,const int &zoom)
{
    if(gtilecache.isEmpty()|gtilecache.isNull())
        return false;

	QString path = "./cache/";//AppCore::Impl().GetSettings().mapCachePath();
    path += QString("/%1/%2/%3/%4.jpg")
            .arg(TypeToSubfolder(type))
            .arg(zoom)
            .arg(pos.Y())
            .arg(pos.X());
	QString dirPath = "./cache/";//AppCore::Impl().GetSettings().mapCachePath();
    dirPath += QString("/%1/%2/%3")
            .arg(TypeToSubfolder(type))
            .arg(zoom)
            .arg(pos.Y());
    QFile file(path);

    QMutexLocker m(&m_writeGuard);
    if((OPMaps::Instance()->GetAccessMode() & AccessMode::UseCache) && file.exists())
    {
        return true;
    }
    QDir dir =dirPath;
    if(dir.mkpath(dirPath))
    {
        if(file.open(QIODevice::WriteOnly))
        {
            qDebug() << "add file:" << path ;
            file.write(tile);
            file.flush();
            file.close();
            return true;
        }
    }

    return false;
}
QByteArray PureImageCache::GetImageFromCache(MapType::Types type, Point pos, int zoom)
{
    //rw_lock.lockForRead();
    QByteArray ar;
    if(gtilecache.isEmpty()|gtilecache.isNull())
        return ar;
	QString path = "./cache/";//AppCore::Impl().GetSettings().mapCachePath();
    path += QString("/%1/%2/%3/%4.jpg")
            .arg(TypeToSubfolder(type))
            .arg(zoom)
            .arg(pos.Y())
            .arg(pos.X());
    QFile file(path);
    if(file.exists() && file.size() > 0)
    {
        //qDebug() << "found:" << path ;
        file.open(QIODevice::ReadOnly);
        ar = file.readAll();
        file.close();
    }
    else
    {
        qDebug() << "absent:" << path ;
    }
    return ar;
}

void PureImageCache::deleteOlderTiles(int const& days)
{
    if(gtilecache.isEmpty()|gtilecache.isNull())
        return;
    QList<long> add;
    bool ret=true;
    QString dir=gtilecache;
    {
        QString db=dir+"Data.qmdb";
        ret=QFileInfo(db).exists();
        if(ret)
        {
            Mcounter.lock();
            qlonglong id=++ConnCounter;
            Mcounter.unlock();
            addDatabaseMutex.lock();
            QSqlDatabase cn(QSqlDatabase::addDatabase("QSQLITE",QString::number(id)));
            addDatabaseMutex.unlock();
            cn.setDatabaseName(db);
            cn.setConnectOptions("QSQLITE_ENABLE_SHARED_CACHE");
            if(cn.open())
            {
                {
                    QSqlQuery query(cn);
                    query.exec(QString("SELECT id, X, Y, Zoom, Type, Date FROM Tiles"));
                    while(query.next())
                    {
                        if(QDateTime::fromString(query.value(5).toString()).daysTo(QDateTime::currentDateTime())>days)
                            add.append(query.value(0).toLongLong());
                    }
                    foreach(long i,add)
                    {
                        query.exec(QString("DELETE FROM Tiles WHERE id = %1;").arg(i));
                    }
                }

                cn.close();
            }
            QSqlDatabase::removeDatabase(QString::number(id));
        }
    }
}
// PureImageCache::ExportMapDataToDB("C:/Users/Xapo/Documents/mapcontrol/debug/mapscache/data.qmdb","C:/Users/Xapo/Documents/mapcontrol/debug/mapscache/data2.qmdb");
bool PureImageCache::ExportMapDataToDB(QString sourceFile, QString destFile)
{
    bool ret=true;
    QList<long> add;
    if(!QFileInfo(destFile).exists())
    {
#ifdef DEBUG_PUREIMAGECACHE
        qDebug()<<"Try to create EmptyDB";
#endif //DEBUG_PUREIMAGECACHE
        ret=CreateEmptyDB(destFile);
    }
    if(!ret) return false;
    addDatabaseMutex.lock();
    QSqlDatabase ca(QSqlDatabase::addDatabase("QSQLITE","ca"));
    addDatabaseMutex.unlock();
    ca.setDatabaseName(sourceFile);

    if(ca.open())
    {
        addDatabaseMutex.lock();
        QSqlDatabase cb(QSqlDatabase::addDatabase("QSQLITE","cb"));
        addDatabaseMutex.unlock();
        cb.setDatabaseName(destFile);
        if(cb.open())
        {
            QSqlQuery queryb(cb);
            queryb.exec(QString("ATTACH DATABASE \"%1\" AS Source").arg(sourceFile));
            QSqlQuery querya(ca);
            querya.exec("SELECT id, X, Y, Zoom, Type, Date FROM Tiles");
            while(querya.next())
            {
                long id=querya.value(0).toLongLong();
                queryb.exec(QString("SELECT id FROM Tiles WHERE X=%1 AND Y=%2 AND Zoom=%3 AND Type=%4;").arg(querya.value(1).toLongLong()).arg(querya.value(2).toLongLong()).arg(querya.value(3).toLongLong()).arg(querya.value(4).toLongLong()));
                if(!queryb.next())
                {
                    add.append(id);
                }

            }
            long f;
            foreach(f,add)
            {
                queryb.exec(QString("INSERT INTO Tiles(X, Y, Zoom, Type, Date) SELECT X, Y, Zoom, Type, Date FROM Source.Tiles WHERE id=%1").arg(f));
                queryb.exec(QString("INSERT INTO TilesData(id, Tile) Values((SELECT last_insert_rowid()), (SELECT Tile FROM Source.TilesData WHERE id=%1))").arg(f));
            }
            add.clear();
            ca.close();
            cb.close();

        }
        else return false;
    }
    else return false;
    QSqlDatabase::removeDatabase("ca");
    QSqlDatabase::removeDatabase("cb");
    return true;

}

QString PureImageCache::TypeToSubfolder(MapType::Types type)
{
    switch(type)
    {
    case MapType::ArcGIS_Map:
        return "ArcGIS_Map";
    case MapType::ArcGIS_Satellite:
        return "ArcGIS_Satellite";
    case MapType::ArcGIS_Terrain:
        return "ArcGIS_Terrain";
    case MapType::GoogleHybridChina:
        return "GoogleHybridChina";
    case MapType::GoogleTerrainChina:
        return "GoogleTerrainChina";
    case MapType::GoogleMap:
        return "GoogleMapOrig";
    case MapType::GoogleLabels:
        return "GoogleLabels";
    case MapType::GoogleHybrid:
        return "GoogleHybridMap";
    case MapType::GoogleSatellite:
        return "GoogleSatelliteMap";
    case MapType::GoogleTerrain:
        return "GoogleTerrainMap";
    case MapType::OpenStreetMap:
        return "OpenStreet4UMap";
    case MapType::OpenStreetOsm:
        return "OpenStreetOsm";
    case MapType::OpenStreetMapSurfer:
        return "OpenStreetMapSurfer";
    case MapType::OpenStreetMapSurferTerrain:
        return "OpenStreetMapSurferTerrain";
    case MapType::YandexMapRu:
        return "YandexSatelliteMap";
    case MapType::BingMap:
        return "BingMap";
    case MapType::BingSatellite:
        return "BingSatellite";
    case MapType::BingHybrid:
        return "BingHybrid";
    }
    return "UNKNOWN";
}
}
