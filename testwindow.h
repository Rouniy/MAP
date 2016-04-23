#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QDialog>

namespace Ui {
class TestWindow;
}

class TestWindow : public QDialog
{
	Q_OBJECT

public:
	explicit TestWindow(QWidget *parent = 0);
	~TestWindow();

private slots:
	void on_rotate_valueChanged(int value);

private:
	Ui::TestWindow *ui;
};

#endif // TESTWINDOW_H
