#include "edittag_basicparams.h"
#include "ui_edittag_basicparams.h"
#include "createtag_nameconflict.h"

EditTag_BasicParams::EditTag_BasicParams(QWidget *parent,
                                         MealManager *mm,
                                         Tag* tag) :
    QDialog(parent),
    ui(new Ui::EditTag_BasicParams)
{
    if (mm == nullptr || tag == nullptr)
        close();
    else
    {
        this->tagPtr = tag;
        this->mm = mm;
    }

    ui->setupUi(this);

    // set value limits
    ui->lineEdit_name->setMaxLength(this->mm->getMaximumNameLength());
    ui->lineEdit_description->setMaxLength(this->mm->getMaximumDescriptionLength());
    ui->spinBox_consecutiveDaysLimit->setMinimum(1);
    ui->spinBox_consecutiveDaysLimit->setMaximum(this->mm->getConsecutiveDaysLimit());

    // initialize to Tag's values
    ui->lineEdit_name->setText(tagPtr->getName());
    ui->lineEdit_description->setText(tagPtr->getDescription());
    ui->checkBox_dependsOnMultiTag->setChecked(tagPtr->getDependency());
    ui->spinBox_consecutiveDaysLimit->setValue(tagPtr->getConsecutiveLimit());

    // enabled days
    {
        QMap<DaysOfTheWeek, bool> enabledDays = tagPtr->getEnabledDays();

        ui->checkBox_mon->setChecked(enabledDays.find(MONDAY).value());
        ui->checkBox_tue->setChecked(enabledDays.find(TUESDAY).value());
        ui->checkBox_wed->setChecked(enabledDays.find(WEDNESDAY).value());
        ui->checkBox_thu->setChecked(enabledDays.find(THURSDAY).value());
        ui->checkBox_fri->setChecked(enabledDays.find(FRIDAY).value());
        ui->checkBox_sat->setChecked(enabledDays.find(SATURDAY).value());
        ui->checkBox_sun->setChecked(enabledDays.find(SUNDAY).value());
    }

}

EditTag_BasicParams::~EditTag_BasicParams()
{
    delete ui;
}

// apply settings
void EditTag_BasicParams::on_pushButton_confirm_clicked()
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

    // if same name, then safe to apply rest
    if (name.toUpper() == tagPtr->getName().toUpper())
    {
        tagPtr->setDescription(desc);
        tagPtr->setDependency(multitagDependency);
        tagPtr->setConsecutiveLimit(consecutiveDays);
        tagPtr->setEnabledDays(enabledDays);
    }
    else // check if conflicts
    {
        if (mm->findNormalTag(name) != nullptr)
        {
            // name conflicts
            CreateTag_NameConflict *window = new CreateTag_NameConflict(this);
            window->setAttribute(Qt::WA_DeleteOnClose);
            window->exec();

            // reset name field
            ui->lineEdit_name->setText(tagPtr->getName());
        }
        else
        {
            // re-sort tag according to alphabetical order
            if (mm->resortNormalTag(tagPtr, name) != 0)
            {
                // sort failed, show warning to user
                CreateTag_NameConflict *window = new CreateTag_NameConflict(this);
                window->setAttribute(Qt::WA_DeleteOnClose);
                window->exec();

                // reset name field
                ui->lineEdit_name->setText(tagPtr->getName());
            }
            else // re-sort success
            {
                tagPtr->setDescription(desc);
                tagPtr->setDependency(multitagDependency);
                tagPtr->setConsecutiveLimit(consecutiveDays);
                tagPtr->setEnabledDays(enabledDays);

                close();
            }
        }
    }
}

// user canceled
void EditTag_BasicParams::on_pushButton_cancel_clicked()
{
    close();
}

