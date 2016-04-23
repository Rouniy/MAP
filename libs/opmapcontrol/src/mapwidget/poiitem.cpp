#include "poiitem.h"
#include "poidetails.h"
#include <QAction>
#include <QMenu>
//#include "coreutils.h"
//#include "managers/pointitemmanager.h"
//#include <MARS/marsmessager.h>
//#include "QSHMImage.h"

namespace mapcontrol
{
POIItem::POIItem(MapGraphicItem* l_map, OPMapWidget* parent, const QString &path, itemType type)
	:safe(true),
	  map(l_map),
	  mapwidget(parent),
	  showsafearea(true),
	  safearea(1000),
	  altitude(0),
	  isDragging(false),
	  m_filePath(path),
	  yaw(0.0),
	  m_type(type),
	  m_type2(0),
	  m_host(0)
{
	name = "No name";

	switch(m_type)
	{
	case ePOIRed:
	{
		setRed();
		break;
	}
	case ePOIGreen:
	{
		setGreen();
		break;
	}
	case ePOIBlue:
	{
		setBlue();
		break;
	}
	case ePOIYellow:
	{
		setYellow();
		break;
	}
	}

	this->setFlag(QGraphicsItem::ItemIsMovable,false);
	this->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
	this->setFlag(QGraphicsItem::ItemIsSelectable,true);

	localposition=map->FromLatLngToLocal(mapwidget->CurrentPosition());
	this->setPos(localposition.X(),localposition.Y());
	this->setZValue(4);
	coord=internals::PointLatLng(0,0);
	RefreshToolTip();

	actionSetRed = new QAction(tr("HOSTILE"), this);
	connect(actionSetRed, SIGNAL(triggered()), this, SLOT(setRed()));

	actionSetGreen = new QAction(tr("NEUTRAL"), this);
	connect(actionSetGreen, SIGNAL(triggered()), this, SLOT(setGreen()));

	actionSetBlue = new QAction(tr("FRIEND"), this);
	connect(actionSetBlue, SIGNAL(triggered()), this, SLOT(setBlue()));

	actionSetYellow = new QAction(tr("UNKNOWN"), this);
	connect(actionSetYellow, SIGNAL(triggered()), this, SLOT(setYellow()));

	actionOpen = new QAction(tr("Open details"), this);
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(OpenItem()));

	actionLockUnlock = new QAction(tr("Lock Unlock Move"), this);
	connect(actionLockUnlock, SIGNAL(triggered()), this, SLOT(LockUnlockItem()));

	actionDelete = new QAction(tr("Delete Item (no recovery)"), this);
	connect(actionDelete, SIGNAL(triggered()), this, SLOT(Delete()));

    SetType(30);
    SetHost(5);
    SetIconPath(":/MARS/placemark_icons/Point/Point_enemy.png");

    //setContextMenuPolicy(Qt::CustomContextMenu);
	RefreshToolTip();
}

QJsonObject POIItem::toJson()
{
	QJsonObject value = AbstractBaseItem::toJson();
	value.insert("screen_path", m_filePath);
	value.insert("name", name);
	value.insert("description", description);
	value.insert("yaw", yaw);
	value.insert("showsafearea", showsafearea);
	value.insert("safearea", safearea);
	value.insert("localsafearea", localsafearea);
	value.insert("altitude", altitude);
	value.insert("isDragging", isDragging);
	value.insert("localposition.X", localposition.X());
	value.insert("localposition.Y", localposition.Y());
	value.insert("coord.lat", coord.Lat());
	value.insert("coord.lng", coord.Lng());
	return value;
}

void POIItem::fromJson(QJsonObject value)
{
	AbstractBaseItem::fromJson(value);
	m_filePath = value["screen_path"].toString();
	name = value["name"].toString();
	description = value["description"].toString();
	yaw = value["yaw"].toDouble();
	showsafearea = value["showsafearea"].toBool();
	safearea = value["safearea"].toInt();
	localsafearea = value["localsafearea"].toInt();
	altitude = value["altitude"].toInt();
	isDragging = value["isDragging"].toBool();
	localposition.SetX(value["localposition.X"].toInt());
	localposition.SetY(value["localposition.Y"].toInt());
	coord.SetLat(value["coord.lat"].toDouble());
	coord.SetLng(value["coord.lng"].toDouble());
	RefreshToolTip();
	RefreshPos();
}

void POIItem::SaveImage()
{
	QDateTime local(QDateTime::currentDateTime());
	m_filePath = local.toString(Qt::ISODate);
	m_filePath.replace(':','.');
	m_filePath+=".png";
	RefreshToolTip();
}

void POIItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	if(coord.Lat() > 0.01)
	{
//		if(iconPath.size())
//		{
//			QPixmap pix;
//			pix.load(iconPath)
//					painter->drawPixmap(-pic.width()/2,-pic.height()/2,pic);
//		}
//		else
		{
			painter->drawPixmap(-pic.width()/2,-pic.height()/2,pic);
		}
	}
}

void POIItem::SetIconPath(QString path)
{
	iconPath = path;
	bool res = pic.load(iconPath);
    pic=pic.scaled(24, 24,Qt::KeepAspectRatio);
}

QRectF POIItem::boundingRect()const
{
	return QRectF(-pic.width()/2,-pic.height()/2,pic.width(),pic.height());
}


int POIItem::type()const
{
	return UserType + 4;
}

void POIItem::RefreshPos()
{
	prepareGeometryChange();
	localposition=map->FromLatLngToLocal(coord);
	this->setPos(localposition.X(),localposition.Y());
	if(showsafearea)
	{
		localsafearea=safearea/map->Projection()->GetGroundResolution(map->ZoomTotal(),coord.Lat());
	}
	Saveobjects();
}

void POIItem::Saveobjects()
{
//	PointItemManager::Impl().SaveItems();
}

void POIItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button()==Qt::LeftButton)
	{
		isDragging=true;
	}
	else {
	}
	QGraphicsItem::mousePressEvent(event);
}

void POIItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button()==Qt::LeftButton)
	{
		coord=map->FromLocalToLatLng(this->pos().x(),this->pos().y());
		isDragging=false;
	}
	RefreshToolTip();
	QGraphicsItem::mouseReleaseEvent(event);
}

void POIItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(isDragging)
	{
		coord=map->FromLocalToLatLng(this->pos().x(),this->pos().y());
	}
	QGraphicsItem::mouseMoveEvent(event);
}

void POIItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	OpenItem();
}

void POIItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu(mapwidget);
	menu.addAction(actionSetRed);
	menu.addAction(actionSetGreen);
	menu.addAction(actionSetBlue);
	menu.addAction(actionSetYellow);
	menu.addSeparator();
	menu.addAction(actionOpen);
	menu.addSeparator();
	menu.addAction(actionLockUnlock);
	menu.addSeparator();
	menu.addAction(actionDelete);
	QMenu* renderMenu = menu.addMenu("Send to");
    //renderMenu->addActions(MarsMessager::Impl().GetRecipientList());


//	cg::geo_point_2 pos;
//	pos.lat = this->coord.Lat();
//	pos.lon = this->Coord().Lng();

    //MarsMessager::Impl().SetCurrentItem(pos, m_type2, m_host);
	menu.exec(event->screenPos());
	this->RefreshPos();
}

void POIItem::SetType(int type)
{
	m_type2 = type;
}

void POIItem::SetHost(int host)
{
	m_host = host;
}

QString POIItem::filePath() const
{
	return m_filePath;
}

QString POIItem::GetGPSPos()
{
	return "Lat:" + QString::number(coord.Lat(), 'f', 7) + " Lon:" + QString::number(coord.Lng(), 'f', 7);
}

QString POIItem::GetCK42Pos()
{
//	cg::geo_point_2 pos;
//	pos.lat = coord.Lat();
//	pos.lon = coord.Lng();
//	cg::glb_point_2 ck42pos = cg::GPS2CK42(pos);
//	return QString("X:") + QString::number(ck42pos.X, 'f', 0) + "  Y:" + QString::number(ck42pos.Y, 'f', 0);
	return "";
}

QString POIItem::GetDescription()
{
	return description;
}

void POIItem::SetDescription(QString desc)
{
	description = desc;
	RefreshToolTip();
}

void POIItem::RefreshToolTip()
{
	QString gps_str = GetGPSPos();
	QString ck42_str = GetCK42Pos();
	if(m_filePath.size())
	{
		setToolTip(QString("<html>\
						   <head>\
						   <meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\
				<title></title>\
				</head>\
				<body>\
				<img alt=\"Screen\" src=\"%4\"\
				height=\"219\" width=\"352\"><br>\
				GPS: %2<br>\
				CK42: %3<br>\
				Description: %1<br>\
				</body>\
				</html>")
				.arg(description)
				.arg(gps_str)
				.arg(ck42_str)
				.arg(m_filePath)
				);
	}
	else
	{
		setToolTip(QString("<html>\
						   <head>\
						   <meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\
				<title></title>\
				</head>\
				<body>\
				GPS: %2<br>\
				CK42: %3<br>\
				Description: %1<br>\
				</body>\
				</html>")
				.arg(description)
				.arg(gps_str)
				.arg(ck42_str)
				);
	}
}

void POIItem::setRed()
{
	pic.load(QString::fromUtf8(":/NATO/images/NATO/red.png"));
	pic=pic.scaled(30,30,Qt::IgnoreAspectRatio);
}

void POIItem::setGreen()
{
	pic.load(QString::fromUtf8(":/NATO/images/NATO/green.png"));
	pic=pic.scaled(30,30,Qt::IgnoreAspectRatio);
}

void POIItem::setBlue()
{
	pic.load(QString::fromUtf8(":/NATO/images/NATO/blue.png"));
	pic=pic.scaled(30,30,Qt::IgnoreAspectRatio);
}

void POIItem::setYellow()
{
	pic.load(QString::fromUtf8(":/NATO/images/NATO/yellow.png"));
	pic=pic.scaled(30,30,Qt::IgnoreAspectRatio);
}

void POIItem::SetCoord(internals::PointLatLng const& value)
{
	coord=value;
	RefreshToolTip();
}

void POIItem::OpenItem()
{
	POIDetails dlg(this, mapwidget);
	dlg.exec();
	Saveobjects();
}

void POIItem::LockUnlockItem()
{
	if(this->flags() & QGraphicsItem::ItemIsMovable)
	{
		this->setFlag(QGraphicsItem::ItemIsMovable, false);
	}
	else
	{
		this->setFlag(QGraphicsItem::ItemIsMovable, true);
	}
	Saveobjects();
}

void POIItem::Delete()
{
//	PointItemManager::Impl().RemoveItem(this);
	Saveobjects();
	this->deleteLater();
	//delete this;
	//map->removeSceneEventFilter(this);
}

}
