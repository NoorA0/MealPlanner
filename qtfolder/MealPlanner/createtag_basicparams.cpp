#include "createtag_basicparams.h"
#include "ui_createtag_basicparams.h"
#include "createtag_nameconflict.h"

CreateTag_BasicParams::CreateTag_BasicParams(QWidget *parent,
                                             MealManager *mm) :
    QDialog(parent),
    ui(new Ui::CreateTag_BasicParams)
{
    if (mm == nullptr)
        close();
    else
        this->mm = mm;

    ui->setupUi(this);

    // set value limits
    ui->lineEdit_name->setMaxLength(this->mm->getMaximumNameLength());
    ui->lineEdit_description->setMaxLength(this->mm->getMaximumDescriptionLength());
    ui->spinBox_consecutiveDaysLimit->setMinimum(1);
    ui->spinBox_consecutiveDaysLimit->setMaximum(this->mm->getConsecutiveDaysLimit());
}

CreateTag_BasicParams::~CreateTag_BasicParams()
{
    delete ui;
}

// confirm clicked
void CreateTag_BasicParams::on_pushButton_confirm_clicked()
{
    // get settings
    QString name = ui->lineEdit_name->text();
    QString desc = ui->lineEdit_description->text();
    bool multitagDependency = ui->checkBox_dependsOnMultiTag->isChecked();
    int consecutiveDays = ui->spinBox_consecutiveDaysLimit->value();
    QMap<DaysOfTheWeek, bool> enabledDays;

    // get enabledDays
    enabledDays[MONDAY] = ui->checkBox_mon->isChecked();
    enabledDays[TUESDAY] = ui->checkBox_tue->isChecked();
    enabledDays[WEDNESDAY] = ui->checkBox_wed->isChecked();
    enabledDays[THURSDAY] = ui->checkBox_thu->isChecked();
    enabledDays[FRIDAY] = ui->checkBox_fri->isChecked();
    enabledDays[SATURDAY] = ui->checkBox_sat->isChecked();
    enabledDays[SUNDAY] = ui->checkBox_sun->isChecked();

    Tag* newTag = mm->createNormalTag(name, desc, multitagDependency, consecutiveDays, enabledDays);

    // if failed due to name conflict
    if (newTag == nullptr)
    {
        // warn user and clear name
        CreateTag_NameConflict *window = new CreateTag_NameConflict(this);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->exec();
        ui->lineEdit_name->clear();
    }
    else
    {
        // send the tag
        emit sendNewTag(newTag);
        close();
    }
}

// cancel
void CreateTag_BasicParams::on_pushButton_cancel_clicked()
{
    close();
}

