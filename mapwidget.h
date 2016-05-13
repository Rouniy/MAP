#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QObject>
#include "opmapcontrol.h"
class MainWindow;

class MapWidget : public mapcontrol::OPMapWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = 0);
    ~MapWidget();
    void SetExtWindows(MainWindow* window);
protected:
    void mouseDoubleClickEvent(QMouseEvent* event);
    void contextMenuEvent(QContextMenuEvent *event);
public slots:
    void useCache(bool use);
    void useServer(bool use);
    void mapTypeChanged(int type);
    void PopUp(const QString &text);
private:
    MainWindow* m_window; //uses for POI creation
    QAction * m_GoogleHybrid;
    QAction * m_GoogleTerrain;
    QAction * m_GoogleSatellite;
    QAction * m_GoogleMap;
    QAction * m_YandexMap;
    QAction * m_ArcGIS_Map;
    QAction * m_ArcGIS_Satellite;
    QAction * m_ArcGIS_Terrain;
    QAction * m_useLocalCache;
    QAction * m_useInternetServer;
    QPoint menuPosition;
};

#endif // MAPWIDGET_H
