#include "mapwidget.h"
//#include "utils.h"
#include <QAction>
//#include "mainwindow.h"
#include <QMenu>
#include <QContextMenuEvent>
#include "src/mapwidget/poiitem.h"
#include "menu.h"
#include "xmlparser.h"

#define USE_LOCAL_CACHE "USE_LOCAL_CACHE"
#define USE_INTERNET_SERVER "USER_INTERNET_SERVER"

MapWidget::MapWidget(QWidget *parent) :
    mapcontrol::OPMapWidget(parent),
    m_window (NULL)
{
    SetUseOpenGL(false);
    //default appears to be Google Hybrid, and is broken currently
#if defined MAP_DEFAULT_TYPE_BING
    this->SetMapType(MapType::BingHybrid);
#elif defined MAP_DEFAULT_TYPE_GOOGLE
    this->SetMapType(MapType::GoogleHybrid);
#else
    this->SetMapType(MapType::OpenStreetMap);
#endif

    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    /*QSignalMapper* signalMapper = new QSignalMapper (this) ;
    m_GoogleHybrid = new QAction(this);
    m_GoogleHybrid->setText("GoogleHybrid");
    m_GoogleHybrid->setChecked(true);
    //this->addAction(GoogleHybrid);
    connect(m_GoogleHybrid, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(m_GoogleHybrid, MapType::GoogleHybrid);

    m_GoogleTerrain = new QAction(this);
    m_GoogleTerrain->setText("GoogleTerrain");
    //this->addAction(m_GoogleTerrain);
    connect(m_GoogleTerrain, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(m_GoogleTerrain, MapType::GoogleTerrain);

    m_GoogleSatellite = new QAction(this);
    m_GoogleSatellite->setText("GoogleSatellite");
    //this->addAction(m_GoogleSatellite);
    connect(m_GoogleSatellite, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(m_GoogleSatellite, MapType::GoogleSatellite);

    m_GoogleMap = new QAction(this);
    m_GoogleMap->setText("GoogleMap");
    //this->addAction(m_GoogleMap);
    connect(m_GoogleMap, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(m_GoogleMap, MapType::GoogleMap);

    m_YandexMap = new QAction(this);
    m_YandexMap->setText("YandexMap");
    //this->addAction(m_YandexMap);
    connect(m_YandexMap, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(m_YandexMap, MapType::YandexMapRu);

    m_ArcGIS_Map = new QAction(this);
    m_ArcGIS_Map->setText("ArcGIS_Map");
    //this->addAction(m_ArcGIS_Map);
    connect(m_ArcGIS_Map, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(m_ArcGIS_Map, MapType::ArcGIS_Map);

    m_ArcGIS_Satellite = new QAction(this);
    m_ArcGIS_Satellite->setText("ArcGIS_Satellite");
    //this->addAction(m_ArcGIS_Satellite);
    connect(m_ArcGIS_Satellite, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(m_ArcGIS_Satellite, MapType::ArcGIS_Satellite);

    m_ArcGIS_Terrain = new QAction(this);
    m_ArcGIS_Terrain->setText("ArcGIS_Terrain");
    //this->addAction(m_ArcGIS_Terrain);
    connect(m_ArcGIS_Terrain, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(m_ArcGIS_Terrain, MapType::ArcGIS_Terrain);
*/
    //QSettings settings(QUOTE(COMPANY_NAME), QUOTE(APP_NAME));
	//QSettings& settings = AppCore::Impl().GetSettings().getSettings();

    m_useLocalCache = new QAction(this);
    m_useLocalCache->setText("Use local cache");
    m_useLocalCache->setCheckable(true);
	//m_useLocalCache->setChecked(settings.value(USE_LOCAL_CACHE, true).toBool());
    useCache(m_useLocalCache->isChecked());
    connect(m_useLocalCache, SIGNAL(triggered(bool)), this, SLOT(useCache(bool)));
    //this->addAction(m_useLocalCache);

    m_useInternetServer = new QAction(this);
    m_useInternetServer->setText("Use internet server");
    m_useInternetServer->setCheckable(true);
	//m_useInternetServer->setChecked(settings.value(USE_INTERNET_SERVER, true).toBool());
    useServer(m_useInternetServer->isChecked());
    connect(m_useInternetServer, SIGNAL(triggered(bool)), this, SLOT(useServer(bool)));
    //this->addAction(m_useInternetServer);
	//LOCAL_ASSERT(connect(signalMapper, SIGNAL(mapped(int)),this, SLOT(mapTypeChanged(int))));
    //setContextMenuPolicy(Qt::DefaultContextMenu);

	//OPMaps::Instance()->setAccessMode((AccessMode::Types)(OPMaps::Instance()->GetAccessMode() & AccessMode::UseServer));
	OPMaps::Instance()->setAccessMode((AccessMode::Types)(OPMaps::Instance()->GetAccessMode() | AccessMode::UseServer));
}

MapWidget::~MapWidget()
{
    // From QGCMapWidget.cc
    SetShowHome(false);	// doing this appears to stop the map lib crashing on exit
    SetShowUAV(false);	//   "          "
    //storeSettings();
    //
}


void MapWidget::contextMenuEvent(QContextMenuEvent *event)
{
    mapcontrol::OPMapWidget::contextMenuEvent(event);
    if(!event->isAccepted())
    {
        XmlParser parser;
        parser.Parse("");
        //TODO: Replace absolute path with the configured path
        Menu* m = new Menu(this, "D:/Project/Prototype/Map/Ico/");
        menuPosition = event->globalPos();
        m->PopUpMenu.exec(menuPosition);
        connect(m, SIGNAL(clicked(QString)), this, SLOT(PopUp(QString)));

        //QMenu menu(this);
        //menu.addAction(m_GoogleTerrain);
        //menu.addAction(m_GoogleSatellite);
        //menu.addAction(m_GoogleMap);
        //menu.addAction(m_YandexMap);
        //menu.addAction(m_ArcGIS_Map);
        //menu.addAction(m_ArcGIS_Satellite);
        //menu.addAction(m_ArcGIS_Terrain);
        //menu.addSeparator();
        //menu.addAction(m_useLocalCache);
        //menu.addAction(m_useInternetServer);
        //menu.exec(event->globalPos());
    }
}

void MapWidget::PopUp(const QString &text)
{
    QPointF p=menuPosition;
    p=map->mapFromParent(p);

    QString path;
    internals::PointLatLng coord = map->FromLocalToLatLng(p.x(), p.y());
    mapcontrol::POIItem * item = new mapcontrol::POIItem(this->getMap(), this, path);
    item->setParentItem(this->getMap());
    item->SetCoord(coord);
    item->RefreshPos();
}

void MapWidget::useCache(bool use)
{
    if(use)
    {
        OPMaps::Instance()->setAccessMode((AccessMode::Types)(OPMaps::Instance()->GetAccessMode() | AccessMode::UseCache));
    }
    else
    {
        OPMaps::Instance()->setAccessMode((AccessMode::Types)(OPMaps::Instance()->GetAccessMode() & AccessMode::UseServer));
    }
    //QSettings settings(QUOTE(COMPANY_NAME), QUOTE(APP_NAME));
	//AppCore::Impl().GetSettings().PutValue(USE_LOCAL_CACHE, use);
}

void MapWidget::useServer(bool use)
{
    if(use)
    {
        OPMaps::Instance()->setAccessMode((AccessMode::Types)(OPMaps::Instance()->GetAccessMode() | AccessMode::UseServer));
    }
    else
    {
        OPMaps::Instance()->setAccessMode((AccessMode::Types)(OPMaps::Instance()->GetAccessMode() & AccessMode::UseCache));
    }
    //QSettings settings(QUOTE(COMPANY_NAME), QUOTE(APP_NAME));
//	AppCore::Impl().GetSettings().PutValue(USE_INTERNET_SERVER, use);
}

void MapWidget::mapTypeChanged(int type)
{
    this->SetMapType((core::MapType::Types)type);
}

void MapWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    QPointF p=event->localPos();
    p=map->mapFromParent(p);

    QString path;
    internals::PointLatLng coord = map->FromLocalToLatLng(p.x(), p.y());
    mapcontrol::POIItem * item = new mapcontrol::POIItem(this->getMap(), this, path);
    item->setParentItem(this->getMap());
    item->SetCoord(coord);
    item->RefreshPos();
}

void MapWidget::SetExtWindows(MainWindow* window)
{
    m_window = window;
}
