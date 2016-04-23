#ifndef ABSTRACTBASEITEM_H
#define ABSTRACTBASEITEM_H
#include <QGraphicsLineItem>
#include <QGraphicsObject>
class BASEMESSAGE;
class AbstractBaseItem :public QGraphicsObject
{
    Q_OBJECT
public:
    AbstractBaseItem(QGraphicsItem *parent = 0);
    virtual QJsonObject toJson();
    virtual void fromJson(QJsonObject);
    void SetYaw(float yaw);
	virtual void fromBaseObj(const BASEMESSAGE& data);
    //virtual BASEMESSAGE toBaseObject();
public slots:
signals:
protected:
    float m_yaw;      ///< Yaw angle of the MAV
};

#endif // ABSTRACTBASEITEM_H
