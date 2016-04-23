#ifndef POIITEM_H
#define POIITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QLabel>
#include "../internals/pointlatlng.h"
#include <QObject>
#include "opmapwidget.h"

namespace mapcontrol
{
class POIItem : public AbstractBaseItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    enum itemType { ePOIRed = UserType + 16, ePOIGreen, ePOIBlue, ePOIYellow,  eCamera, eTarget };
    //enum itemSubType{ };
    POIItem(MapGraphicItem* map,OPMapWidget* parent, const QString &path, itemType type = ePOIRed);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject value);

    QRectF boundingRect() const;
    int type() const;
    void RefreshPos();
    bool ShowSafeArea()const{return showsafearea;}
    void SetShowSafeArea(bool const& value){showsafearea=value;}
    int SafeArea()const{return safearea;}
    void SetSafeArea(int const& value){safearea=value;}
    bool safe;
    void SetCoord(internals::PointLatLng const& value);
    internals::PointLatLng Coord()const{return coord;}
    void SetAltitude(int const& value){altitude=value;}
    int Altitude()const{return altitude;}
    void SaveImage();
    QString filePath() const;
	void SetIconPath(QString path);
	void SetType(int type);
	void SetHost(int host);
    QString GetGPSPos();
    QString GetCK42Pos();
    QString GetDescription();
    void SetDescription(QString desc);

protected:
    void RefreshToolTip();
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
protected:
    virtual void Saveobjects();
    MapGraphicItem* map;
    OPMapWidget* mapwidget;
    QPixmap pic;
    core::Point localposition;
    internals::PointLatLng coord;
    bool showsafearea;
    int safearea;
    int localsafearea;
    int altitude;
    bool isDragging;
    QString description;
    QString name;
    QString m_filePath; //< Path to screenshot on the local file system
    double yaw;
    itemType m_type;
    QAction *actionSetRed;
    QAction *actionSetGreen;
    QAction *actionSetBlue;
    QAction *actionSetYellow;
    QAction *actionOpen;
    QAction *actionLockUnlock;
    QAction *actionDelete;

	QString iconPath;
	int m_type2;
	int m_host;

public slots:
    void setRed();
    void setGreen();
    void setBlue();
    void setYellow();

    void OpenItem();
    void LockUnlockItem();
    virtual void Delete();
    //void UnLockItem();

signals:


};
}

#endif // POIITEM_H
