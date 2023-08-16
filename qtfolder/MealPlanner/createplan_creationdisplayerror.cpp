#include "createplan_creationdisplayerror.h"
#include "ui_createplan_creationdisplayerror.h"

const QString DAYSERRORED_PREAMBLE = "<html><head/><body><p align=\"center\">Number of days with no meals: <span style=\" font-weight:700;\">";
const QString FAILEDBUDGET_PREAMBLE = "</span></p><p align=\"center\">Total cost for plan: <span style=\" font-weight:700;\">";
const QString END_TAGS = "</span></p></body></html>";

CreatePlan_CreationDisplayError::CreatePlan_CreationDisplayError(QWidget *parent,
                                                                 const unsigned int &daysErrored,
                                                                 const double &failedBudget) :
    QDialog(parent),
    ui(new Ui::CreatePlan_CreationDisplayError)
{
    ui->setupUi(this);

    // set label text to display error values
    QString setText = DAYSERRORED_PREAMBLE + QString::number(daysErrored)
                      + FAILEDBUDGET_PREAMBLE + QString::number(failedBudget)
                      + END_TAGS;

    ui->label_errorDetails->setText(setText);
}

CreatePlan_CreationDisplayError::~CreatePlan_CreationDisplayError()
{
    delete ui;
}

void CreatePlan_CreationDisplayError::on_pushButton_OK_clicked()
{
    close();
}

