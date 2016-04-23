#include "poietatarget.h"
//#include "managers/pointitemmanager.h"

namespace mapcontrol
{
POIETATarget::POIETATarget(MapGraphicItem *map, OPMapWidget *parent)
    :POIItem(map, parent, QString())
{
     pic.load(QString::fromUtf8(":/ETA/images/ETA/target.png"));
     this->setFlag(QGraphicsItem::ItemIsMovable,false);
     this->setFlag(QGraphicsItem::ItemIgnoresTransformations,false);
     this->setFlag(QGraphicsItem::ItemIsSelectable,false);
     setZValue(42);
}

void POIETATarget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(coord.Lat() > 0.01)
    {
        painter->drawPixmap(-pic.width()/2,-pic.height()/2,pic);
    }
}

QJsonObject POIETATarget::toJson()
{
    QJsonObject value = POIItem::toJson();
    value.insert("type", QString("POIETATarget"));
    return value;
}

void POIETATarget::fromJson(QJsonObject value)
{
    POIItem::fromJson(value);
}

void POIETATarget::Saveobjects()
{
}

void POIETATarget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}
}
