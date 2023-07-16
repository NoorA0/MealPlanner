#include <QDebug>
#include "mainwindow.h"
#include "settingswindow.h"
#include "manualwindow.h"
#include "attributionsdialogue.h"
#include "ui_mainwindow.h"
#include "createplan_filename.h"

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

// clicked attributions
void MainWindow::on_attributionsButton_clicked()
{
    AttributionsDialogue* ad = new AttributionsDialogue(this);
    ad->setAttribute(Qt::WA_DeleteOnClose);
    ad->show();
}


void MainWindow::on_quitProgramButton_clicked()
{
    //TODO: implement file saving here
    close();

}

// clicked generate program
void MainWindow::on_generatePlanButton_clicked()
{
    // prompt user for file name
    // default name is "New_Meal_Plan.txt"
    QString fileName;

    createPlan_Filename* cpf = new createPlan_Filename(this);
    cpf->setAttribute(Qt::WA_DeleteOnClose);

    // use signal to tell if user quit or entered a filename

    // if not quit, then receive the file name

    cpf->exec();
    qDebug() << "filename: " << fileName;



    // if OK, prompt for schedule length

    // if OK, prompt for budget limit

    // confirm settings

    // error page if failed at any point

    // success
}

