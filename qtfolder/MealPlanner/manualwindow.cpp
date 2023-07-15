#include "manualwindow.h"
#include "ui_manualwindow.h"

ManualWindow::ManualWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManualWindow)
{
    ui->setupUi(this);
}

ManualWindow::~ManualWindow()
{
    delete ui;

    // unhide the previous window
    QMainWindow* mw = qobject_cast<QMainWindow*>(parent());
    if (mw != nullptr)
    {
        mw->show();
    }
}

void ManualWindow::on_returnMenuButton_clicked()
{
    this->close();
}

