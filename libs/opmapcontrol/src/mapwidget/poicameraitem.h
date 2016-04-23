#ifndef POICAMERAITEM_H
#define POICAMERAITEM_H
#include "poiitem.h"
namespace mapcontrol
{
class POICameraItem : public POIItem
{
public:
    POICameraItem(MapGraphicItem *map, OPMapWidget *parent);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
private:
//        QPixmap pic;
};
}
#endif // POICAMERAITEM_H
