#include "createmultitag_basicparams.h"
#include "ui_createmultitag_basicparams.h"
#include "createmultitag_nameconflict.h"

CreateMultitag_BasicParams::CreateMultitag_BasicParams(QWidget *parent,
                                                       MealManager *mm) :
    QDialog(parent),
    ui(new Ui::CreateMultitag_BasicParams)
{
    if (mm == nullptr)
        close();
    else
        this->mm = mm;
    ui->setupUi(this);

    // set value limits
    ui->lineEdit_name->setMaxLength(this->mm->getMaximumNameLength());
    ui->lineEdit_description->setMaxLength(this->mm->getMaximumDescriptionLength());

    // enabled days
    ui->checkBox_mon->setChecked(true);
    ui->checkBox_tue->setChecked(true);
    ui->checkBox_wed->setChecked(true);
    ui->checkBox_thu->setChecked(true);
    ui->checkBox_fri->setChecked(true);
    ui->checkBox_sat->setChecked(true);
    ui->checkBox_sun->setChecked(true);
}

CreateMultitag_BasicParams::~CreateMultitag_BasicParams()
{
    delete ui;
}

// cancel clicked
void CreateMultitag_BasicParams::on_pushButton_cancel_clicked()
{
    close();
}

// confirm clicked
void CreateMultitag_BasicParams::on_pushButton_confirm_clicked()
{
    // get settings
    MultiTag *newMT = nullptr;
    QString name = ui->lineEdit_name->text();
    QString desc = ui->lineEdit_description->text();
    bool elevatedPriority = ui->checkBox_elevatedPriority->isChecked();
    bool totalFulfillment = ui->checkBox_requireTotalFulfillment->isChecked();
    QMap<DaysOfTheWeek, bool> enabledDays;

    // get enabledDays
    enabledDays[MONDAY] = ui->checkBox_mon->isChecked();
    enabledDays[TUESDAY] = ui->checkBox_tue->isChecked();
    enabledDays[WEDNESDAY] = ui->checkBox_wed->isChecked();
    enabledDays[THURSDAY] = ui->checkBox_thu->isChecked();
    enabledDays[FRIDAY] = ui->checkBox_fri->isChecked();
    enabledDays[SATURDAY] = ui->checkBox_sat->isChecked();
    enabledDays[SUNDAY] = ui->checkBox_sun->isChecked();

    // dont bother creating if name is empty
    if (!name.isEmpty() && name.trimmed() != "")
        newMT = mm->createMultiTag(name, desc, elevatedPriority, totalFulfillment, enabledDays);

    // if failed due to name conflict
    if (newMT == nullptr)
    {
        // warn user and clear name
        CreateMultitag_NameConflict *window = new CreateMultitag_NameConflict(this);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->exec();
        ui->lineEdit_name->clear();
    }
    else
    {
        // send the multitag
        emit sendNewMultitag(newMT);
        close();
    }
}

