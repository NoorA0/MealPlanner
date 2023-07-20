#include <QDebug>
#include "mainwindow.h"
#include "settingswindow.h"
#include "manualwindow.h"
#include "attributionsdialogue.h"
#include "ui_mainwindow.h"
#include "createplan_filename.h"
#include "createplan_length.h"
#include "createplan_budget.h"
#include "createplan_confirmation.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileName = QString();
    planLength = 0;
    planBudget = -1.0;
    settingsConfirmed = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// gets file name to generate if isValid is true
void MainWindow::getFileName(const bool &isValid, const QString &newFileName)
{
    if (isValid)
        fileName = newFileName;
    else
        fileName = QString(); // set to null string
}

// gets plan length to generate if isValid is true
void MainWindow::getPlanLength(const bool &isValid, const int &newLength)
{
    if (isValid)
        planLength = newLength;
    else
        planLength = 0;
}

// gets plan budget to generate if isValid is true
void MainWindow::getPlanBudget(const bool &isValid, const double &newBudget)
{
    if (isValid)
        planBudget = newBudget;
    else
        planBudget = -1.0;
}

// gets final plan configuration confirmation
void MainWindow::getConfirmation(const bool &isValid)
{
    if (isValid)
        settingsConfirmed = true;
    else
        settingsConfirmed = false;
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
    // default name is stored in createplan_filename.cpp

    createPlan_Filename* cpf = new createPlan_Filename(this);
    cpf->setAttribute(Qt::WA_DeleteOnClose);

    // connect signal to receive file name information
    connect(cpf,
            SIGNAL(sendFileName(bool,QString)),
            this,
            SLOT(getFileName(bool,QString)),
            Qt::UniqueConnection);
    cpf->exec();

    // continue if user did not cancel
    if (fileName.isNull())
    {
        return;
    }

    // prompt for schedule length
    createPlan_Length* cpl = new createPlan_Length(this);
    cpl->setAttribute(Qt::WA_DeleteOnClose);

    // connect signal to receive plan length
    connect(cpl,
            SIGNAL(sendPlanLength(bool,int)),
            this,
            SLOT(getPlanLength(bool,int)),
            Qt::UniqueConnection);
    cpl->exec();

    // continue if user did not cancel
    if (planLength < 1)
    {
        return;
    }

    // prompt for budget limit
    createPlan_Budget* cpb = new createPlan_Budget(this);
    cpb->setAttribute(Qt::WA_DeleteOnClose);

    // connect signal to receive plan budget
    connect(cpb,
            SIGNAL(sendPlanBudget(bool,double)),
            this,
            SLOT(getPlanBudget(bool,double)),
            Qt::UniqueConnection);
    cpb->exec();

    // continue if user did not cancel
    if (planBudget <= -1.0)
    {
        return;
    }

    // confirm settings
    createplan_confirmation* cpc = new createplan_confirmation(this,
                                                               fileName,
                                                               planLength,
                                                               planBudget);
    cpc->setAttribute(Qt::WA_DeleteOnClose);

    // connect signal to receive confirmation
    connect(cpc,
            SIGNAL(sendConfirmation(bool)),
            this,
            SLOT(getConfirmation(bool)),
            Qt::UniqueConnection);
    cpc->exec();

    // continue if user did not cancel
    if (!settingsConfirmed)
    {
        return;
    }

    qDebug() << "----stats: ----\nfilename: "
             << fileName << "\nlength (weeks): " << planLength
             << "\nbudget: " << planBudget;

    // error page if failed at any point

    // success
}

