#include "poidetails.h"
#include "ui_poidetails.h"
#include "poiitem.h"

POIDetails::POIDetails(mapcontrol::POIItem* item, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::POIDetails),
    m_item(item)
{
    ui->setupUi(this);
    if(item->filePath().size())
    {
        ui->img->setPixmap(QPixmap(item->filePath()));
    }
    ui->ck42->setText(item->GetCK42Pos());
    ui->gps->setText(item->GetGPSPos());
    ui->descriptionEdit->setPlainText(item->GetDescription());
}

POIDetails::~POIDetails()
{
    delete ui;
}

void POIDetails::on_pushButton_clicked()
{

}

void POIDetails::on_descriptionEdit_textChanged()
{
     m_item->SetDescription(ui->descriptionEdit->toPlainText());
}
