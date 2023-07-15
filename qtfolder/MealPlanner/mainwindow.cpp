#include "mainwindow.h"
#include "settingswindow.h"
#include "manualwindow.h"
#include "attributionsdialogue.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// clicked settings button
void MainWindow::on_settingsButton_clicked()
{
    hide();
    SettingsWindow *sw = new SettingsWindow(this);
    sw->setAttribute(Qt::WA_DeleteOnClose);
    sw->show();
}

// clicked manual button
void MainWindow::on_manualButton_clicked()
{
    hide();
    ManualWindow *mw = new ManualWindow(this);
    mw->setAttribute(Qt::WA_DeleteOnClose);
    mw->show();
}


void MainWindow::on_attributionsButton_clicked()
{
    AttributionsDialogue* ad = new AttributionsDialogue(this);
    ad->setAttribute(Qt::WA_DeleteOnClose);
    ad->show();
}

