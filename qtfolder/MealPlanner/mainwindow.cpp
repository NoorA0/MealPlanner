#include <QDebug>
#include "mainwindow.h"
#include "mealmanager.h"
//#include "settingswindow.h"
#include "attributionsdialogue.h"
#include "ui_mainwindow.h"
#include "createplan_filename.h"
#include "createplan_length.h"
#include "createplan_budget.h"
#include "createplan_confirmation.h"
#include "createplan_nomealsortagswarning.h"
#include "createplan_inprogress.h"
#include "createplan_success.h"
#include "createplan_creationdisplayerror.h"
#include "createplan_generatorcrashed.h"
#include "editmeals.h"
#include "edittags.h"
#include "editmultitagswindow.h"
#include "manual_summaryinfo.h"
#include "manual_mealinfo.h"
#include "manual_taginfo.h"
#include "manual_multitaginfo.h"
#include "manual_exampleinfo.h"
#include <QTime>

// returns to the main menu page of the QStackedWidget and sets the title
// used by the "return to main menu" button in the other pages
void MainWindow::ReturnToMainMenuPage()
{
    ui->stackedWidget->setCurrentWidget(ui->MainMenuPage);
    this->setWindowTitle(titleBarTitle_MainMenu);
}

MainWindow::MainWindow(QWidget *parent, MealManager *mm)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    if (mm == nullptr)
    {
        qDebug() << "MainWindow instantiated with null ref to MealManager!";
        close();
    }
    else
        this->mm = mm;

    ui->setupUi(this);
    fileName = QString();
    planLength = 0;
    planBudget = -1.0;
    settingsConfirmed = false;
    ui->stackedWidget->setCurrentWidget(ui->MainMenuPage);
}

MainWindow::~MainWindow()
{
    delete ui;
    mm = nullptr;
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
    settingsConfirmed = isValid;
}

// switch to the settings page
void MainWindow::on_settingsButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->SettingsPage);
    this->setWindowTitle(titleBarTitle_Settings);

}

// switch to the manual page
void MainWindow::on_manualButton_clicked()
{    
    ui->stackedWidget->setCurrentWidget(ui->ManualPage);
    this->setWindowTitle(titleBarTitle_ProgramManual);
}

// show attributions
void MainWindow::on_attributionsButton_clicked()
{
    AttributionsDialogue* ad = new AttributionsDialogue(this);
    ad->setAttribute(Qt::WA_DeleteOnClose);
    ad->exec();
}


void MainWindow::on_quitProgramButton_clicked()
{
    // save data before quit
    try
    {
        std::ofstream oFile;
        mm->saveState(oFile);
    }
    catch (QString &error)
    {
        // write error to log
        std::ofstream errOut(mm->getLogFileName().toStdString(), std::ios::app);

        if (errOut.is_open())
        {
            // get system time
            auto now = std::chrono::system_clock::now();
            std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

            errOut << ctime(&currentTime);
            errOut << "ERROR: Could not save program data.\n"
                   << "REASON: " << error.toStdString() << ".\n"
                   << "\nThis may be caused by insufficient write permissions or lack of drive space.\n\n";
        }
        errOut.close();
    }

    close();
}

// clicked generate program
void MainWindow::on_generatePlanButton_clicked()
{
    // post-generation validation vars
    int returnCode = -1;
    unsigned int erroredDays = 0;
    double failedBudget = 0;

    // if no meals AND tags, cannot create a plan
    if (mm->getMeals().size() < 1 || mm->getNormalTags().size() < 1)
    {
        // tell user and close
        CreatePlan_NoMealsOrTagsWarning *window = new CreatePlan_NoMealsOrTagsWarning(this);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->exec();
        return;
    }

    // prompt user for file name
    // default name is stored in createplan_filename.cpp
    createPlan_Filename* cpf = new createPlan_Filename(this, mm);
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
    createPlan_Length* cpl = new createPlan_Length(this, mm);
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
    createPlan_Budget* cpb = new createPlan_Budget(this, mm);
    cpb->setAttribute(Qt::WA_DeleteOnClose);

    // connect signal to receive plan budget
    connect(cpb,
            SIGNAL(sendPlanBudget(bool,double)),
            this,
            SLOT(getPlanBudget(bool,double)),
            Qt::UniqueConnection);
    cpb->exec();

    // continue if user did not cancel
    if (planBudget < 0)
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

    // opens window to tell user to wait, will close when done creating plan
    CreatePlan_InProgress *cpip = new CreatePlan_InProgress(this);
    cpip->setAttribute(Qt::WA_DeleteOnClose);
    cpip->setModal(true);

    //open window to make user wait
    cpip->show();

    // generate plan
    {
        std::ofstream oFile;
        // append .txt to file name
        fileName += ".txt";
        returnCode = mm->generateSchedule(fileName, planBudget, planLength, oFile, erroredDays, failedBudget);
    }

    // done, close the window
    emit closeModalWindow();

    // if succeeded in creating a plan
    if (returnCode == 0)
    {
        // tell user
        CreatePlan_Success *cps = new CreatePlan_Success(this);
        cps->setAttribute(Qt::WA_DeleteOnClose);
        cps->exec();
    }
    else if (returnCode != -1)
    {
        // display error window
        CreatePlan_CreationDisplayError *window = new CreatePlan_CreationDisplayError(this, erroredDays, failedBudget);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->exec();
    }
    else
    {
        // display critical error window
        CreatePlan_GeneratorCrashed *window = new CreatePlan_GeneratorCrashed(this);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->exec();
    }
}

// settings - view/edit meals
void MainWindow::on_editMealsButton_clicked()
{
    EditMeals *em = new EditMeals(this, this->mm);
    em->setAttribute(Qt::WA_DeleteOnClose);
    em->exec();
}

// settings - view/edit tags
void MainWindow::on_editTagsButton_clicked()
{
    EditTags *et = new EditTags(this, this->mm);
    et->setAttribute(Qt::WA_DeleteOnClose);
    et->exec();
}

// settings - view/edit multitags
void MainWindow::on_editMultiTagsButton_clicked()
{
    hide();
    EditMultitagsWindow *emw = new EditMultitagsWindow(this, this->mm);
    emw->setAttribute(Qt::WA_DeleteOnClose);
    emw->show();
}

// settings - return to main menu page
void MainWindow::on_returnFromSettingsPage_clicked()
{
    ReturnToMainMenuPage();
}

// manual - general overview
void MainWindow::on_explainUseButton_clicked()
{
    Manual_SummaryInfo *msi = new Manual_SummaryInfo(this);
    msi->setAttribute(Qt::WA_DeleteOnClose);
    msi->exec();
}

// manual - meal info
void MainWindow::on_explainMealsButton_clicked()
{
    Manual_MealInfo *mmi = new Manual_MealInfo(this);
    mmi->setAttribute(Qt::WA_DeleteOnClose);
    mmi->exec();
}

// manual - tag info
void MainWindow::on_explainTagsButton_clicked()
{
    Manual_TagInfo *mti = new Manual_TagInfo(this);
    mti->setAttribute(Qt::WA_DeleteOnClose);
    mti->exec();
}

// manual - multitag info
void MainWindow::on_explainMultiTagsButton_clicked()
{
    Manual_MultitagInfo *mmi = new Manual_MultitagInfo(this);
    mmi->setAttribute(Qt::WA_DeleteOnClose);
    mmi->exec();
}

// manual - example use cases
void MainWindow::on_explainExamplesButton_clicked()
{
    Manual_ExampleInfo *mei = new Manual_ExampleInfo(this);
    mei->setAttribute(Qt::WA_DeleteOnClose);
    mei->exec();
}

// manual - return to main menu page
void MainWindow::on_returnFromManualPage_clicked()
{
    ReturnToMainMenuPage();
}
