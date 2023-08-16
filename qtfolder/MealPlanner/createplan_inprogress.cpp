#include "createplan_inprogress.h"
#include "ui_createplan_inprogress.h"
#include <fstream>

CreatePlan_InProgress::CreatePlan_InProgress(QWidget *parent,
                                             MealManager *mm,
                                             const QString &fileName,
                                             const double &budget,
                                             const int &durationWeeks) :
    QDialog(parent),
    ui(new Ui::CreatePlan_InProgress)
{
    if (mm == nullptr)
        close();

    ui->setupUi(this);

    // generate plan
    int errorsPresent;
    unsigned int errors;
    double failedBudget;
    std::ofstream oFile;

    errorsPresent = mm->generateSchedule(fileName, budget, durationWeeks, oFile, errors, failedBudget);

    emit createPlanReturn(errorsPresent, errors, failedBudget);
    close();
}

CreatePlan_InProgress::~CreatePlan_InProgress()
{
    delete ui;
}
