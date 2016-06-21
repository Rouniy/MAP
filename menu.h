#ifndef MENU_H
#define MENU_H

#include <QDir>
#include <QObject>
#include "src/mapwidget/poiitem.h"
#include <QMenu>

class Menu : public QObject
{
    Q_OBJECT
public:
    explicit Menu(QObject *parent, const QHash<QString, QString> &info, QMenu *popUpMenu, const QString &iconsPath = ".");
    QMenu* PopUpMenu;
private:
    QHash<QString, QMenu*> MenuItemsList;
    QSignalMapper* signalMapper;
    QHash<QString, QString> info;
    void ScanDir(QDir dir, const QStringList &parentDirs, QMenu* menuParent);
    void AddFileItem(QDir dir, const QStringList &parentDirs, QMenu* menuParent);
    void AddFolderItem(QDir dir, const QStringList &parentDirs, QMenu* menuParent);
    QMenu* GetMenuItem(const QStringList &parentDirs, QString name, QMenu *menuParent,
                       const QString &absoluteFilePath, bool addAction);
    QString Menu::GetName(QString &code);
signals:
    void clicked(QString text);

public slots:
    void menuSelection(QString name);
};

#endif // MENU_H
