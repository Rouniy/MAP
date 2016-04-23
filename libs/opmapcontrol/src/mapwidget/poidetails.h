#ifndef POIDETAILS_H
#define POIDETAILS_H

#include <QDialog>

namespace Ui {
class POIDetails;
}

namespace mapcontrol
{
class POIItem;
}

class POIDetails : public QDialog
{
    Q_OBJECT

public:
    explicit POIDetails(mapcontrol::POIItem* item, QWidget *parent = 0);
    ~POIDetails();

private slots:
    void on_pushButton_clicked();
    void on_descriptionEdit_textChanged();

private:
    Ui::POIDetails *ui;
    mapcontrol::POIItem* m_item;
};

#endif // POIDETAILS_H
