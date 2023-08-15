#include "createplan_inprogress.h"
#include "ui_createplan_inprogress.h"

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
    unsigned int errors;
    double failedBudget;
    mm->generateSchedule(fileName, budget, durationWeeks, //ofile, )
    // send return value
}

CreatePlan_InProgress::~CreatePlan_InProgress()
{
    delete ui;
}
