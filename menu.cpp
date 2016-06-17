#include "menu.h"
#include <QDir>
#include <QAction>
#include <QMenu>
#include "src/mapwidget/poiitem.h"

Menu::Menu(QObject *parent, const QHash<QString, QString> &info, const QString &iconsPath)
    : QObject(parent)
{
    this->info = info;
    signalMapper = new QSignalMapper(this);
    QDir dir;
    dir.setPath(iconsPath);
    ScanDir(dir, QStringList(), &PopUpMenu);
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(menuSelection(QString)));
}

void Menu::ScanDir(QDir dir, const QStringList &parentDirs, QMenu* menuParent)
{
    AddFolderItem(dir, parentDirs, menuParent);
    AddFileItem(dir, parentDirs, menuParent);
}

void Menu::AddFileItem(QDir dir, const QStringList &parentDirs, QMenu* menuParent)
{
    dir.setNameFilters(QStringList("*.svg"));
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    QStringList fileList = dir.entryList();
    for (int i=0; i<fileList.count(); ++i)
    {
        QFileInfo fileInfo = QFileInfo(dir, fileList.at(i));
        QString absoluteFilePath = fileInfo.absoluteFilePath();
        QString fileName = fileInfo.fileName();
        QStringList fileNameItems = fileName.split(".");
        QMenu* menuPrev = menuParent;
        QStringList dirs = QStringList(parentDirs);
        for(int j=0; j< fileNameItems.count() - 1; ++j)
        {
            menuPrev = GetMenuItem(dirs, fileNameItems[j], menuPrev, absoluteFilePath,
                                   j >= fileNameItems.count() - 2);
            dirs.append(fileNameItems[j]);
        }
    }
}

void Menu::AddFolderItem(QDir dir, const QStringList &parentDirs, QMenu* menuParent)
{
    dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList dirList = dir.entryList();
    for (int i=0; i<dirList.size(); ++i)
    {
        QFileInfo fileInfo = dirList.at(i);
        QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(dirList.at(i));
        QStringList dirs = QStringList(parentDirs);
        dirs.append(fileInfo.fileName());
        QMenu* menu = GetMenuItem(parentDirs, fileInfo.fileName(), menuParent, "", false);
        ScanDir(QDir(newPath), dirs, menu);
    }
}

QMenu* Menu::GetMenuItem(const QStringList &parentDirs, QString name, QMenu *menuParent,
                         const QString &absoluteFilePath, bool addAction)
{
    QString key = parentDirs.join('\\') + "\\" + name;
    if(MenuItemsList.contains(key))
    {
        return MenuItemsList.value(key);
    }
    else
    {
        if(addAction)
        {
            QAction* action = new QAction(GetName(name), this);
            QIcon* icon = new QIcon(absoluteFilePath);
            action->setIcon(* icon);
            connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
            signalMapper->setMapping(action, name);
            menuParent->addAction(action);
            return menuParent;
        }
        else
        {
            QMenu* menu = menuParent->addMenu(name);
            MenuItemsList[key] = menu;
            return menu;
        }
    }
}

void Menu::menuSelection(QString name)
{
    emit clicked(name);
}

QString Menu::GetName(QString &code)
{
    return this->info.contains(code)?this->info[code]:code;
}
