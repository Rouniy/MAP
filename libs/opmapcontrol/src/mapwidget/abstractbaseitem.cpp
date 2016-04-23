#include "abstractbaseitem.h"
#include <QJsonObject>
//#include "MARS/messages.pb.h"

AbstractBaseItem::AbstractBaseItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
	, m_yaw(0)
{

}

QJsonObject AbstractBaseItem::toJson()
{
    QJsonObject value;
    return value;
}

void AbstractBaseItem::fromJson(QJsonObject)
{

}

void AbstractBaseItem::SetYaw(float yaw)
{
    m_yaw = yaw;
}

void AbstractBaseItem::fromBaseObj(const BASEMESSAGE& data)
{

}

//BASEMESSAGE AbstractBaseItem::toBaseObject()
//{
//	BASEMESSAGE retval;
//	return retval;
//}
