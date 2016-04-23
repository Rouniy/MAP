#ifndef POIETATARGET_H
#define POIETATARGET_H
#include "poiitem.h"
namespace mapcontrol
{
class POIETATarget : public POIItem
{
public:
    POIETATarget(MapGraphicItem *map, OPMapWidget *parent);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject value);
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    void Saveobjects();
    //        QPixmap pic;
};
}
#endif // POIETATARGET_H
