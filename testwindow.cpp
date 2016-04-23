#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TestWindow)
{
	ui->setupUi(this);
	this->setWindowFlags(this->windowFlags() | Qt::WindowMinMaxButtonsHint );
}

TestWindow::~TestWindow()
{
	delete ui;
}

void TestWindow::on_rotate_valueChanged(int value)
{
	ui->widget->SetRotate(value);
}
