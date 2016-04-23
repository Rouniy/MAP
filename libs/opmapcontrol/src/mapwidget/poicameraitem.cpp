#include "poicameraitem.h"
namespace mapcontrol
{
POICameraItem::POICameraItem(MapGraphicItem* map,OPMapWidget* parent)
    :POIItem(map, parent, QString())
{
     pic.load(QString::fromUtf8(":/camera/images/cam/eye2.png"));
     this->setFlag(QGraphicsItem::ItemIsMovable,false);
     this->setFlag(QGraphicsItem::ItemIgnoresTransformations,false);
     this->setFlag(QGraphicsItem::ItemIsSelectable,false);
}

void POICameraItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(coord.Lat() > 0.01)
    {
        painter->drawPixmap(-pic.width()/2,-pic.height()/2,pic);
    }
}
}
