#include "editmultitag_basicparams.h"
#include "ui_editmultitag_basicparams.h"
#include "createmultitag_nameconflict.h"

EditMultitag_BasicParams::EditMultitag_BasicParams(QWidget *parent,
                                                   MealManager *mm,
                                                   MultiTag *tag) :
    QDialog(parent),
    ui(new Ui::EditMultitag_BasicParams)
{
    if (mm == nullptr || tag == nullptr)
        close();
    else
    {
        this->mm = mm;
        this->tag = tag;
    }
    ui->setupUi(this);

    // set value limits
    ui->lineEdit_name->setMaxLength(this->mm->getMaximumNameLength());
    ui->lineEdit_description->setMaxLength(this->mm->getMaximumDescriptionLength());

    // initialize to MulitTag's values
    ui->lineEdit_name->setText(tag->getName());
    ui->lineEdit_description->setText(tag->getDescription());
    ui->checkBox_elevatedPriority->setChecked(tag->getPriority());
    ui->checkBox_requireTotalFulfillment->setChecked(tag->getRequireFulfillment());

    // enabled days
    {
        QMap<DaysOfTheWeek, bool> enabledDays = tag->getEnabledDays();

        ui->checkBox_mon->setChecked(enabledDays.find(MONDAY).value());
        ui->checkBox_tue->setChecked(enabledDays.find(TUESDAY).value());
        ui->checkBox_wed->setChecked(enabledDays.find(WEDNESDAY).value());
        ui->checkBox_thu->setChecked(enabledDays.find(THURSDAY).value());
        ui->checkBox_fri->setChecked(enabledDays.find(FRIDAY).value());
        ui->checkBox_sat->setChecked(enabledDays.find(SATURDAY).value());
        ui->checkBox_sun->setChecked(enabledDays.find(SUNDAY).value());
    }
}

EditMultitag_BasicParams::~EditMultitag_BasicParams()
{
    delete ui;
}

// user said cancel
void EditMultitag_BasicParams::on_pushButton_cancel_clicked()
{
    close();
}

// user wants to apply settings
void EditMultitag_BasicParams::on_pushButton_confirm_clicked()
{
    // get settings
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

    // if same name, apply params
    if (name.toUpper() == tag->getName().toUpper())
    {
        tag->setDescription(desc);
        tag->setHighestPriority(elevatedPriority);
        tag->setRequireFulfillment(totalFulfillment);
        tag->setEnabledDays(enabledDays);
    }
    else // check if name conflict
    {
        if (mm->findMultiTag(name) != nullptr)
        {
            // name conflicts, tell user
            CreateMultitag_NameConflict *window = new CreateMultitag_NameConflict(this);
            window->setAttribute(Qt::WA_DeleteOnClose);
            window->exec();

            // reset name field
            ui->lineEdit_name->setText(tag->getName());
        }
        else
        {
            // re-sort the tag by its new name
            if (mm->resortMultiTag(tag, name) != 0)
            {
                // failed to sort, tell user
                CreateMultitag_NameConflict *window = new CreateMultitag_NameConflict(this);
                window->setAttribute(Qt::WA_DeleteOnClose);
                window->exec();

                // reset name field
                ui->lineEdit_name->setText(tag->getName());
            }
            else // resorted, apply other params
            {
                tag->setDescription(desc);
                tag->setHighestPriority(elevatedPriority);
                tag->setRequireFulfillment(totalFulfillment);
                tag->setEnabledDays(enabledDays);

                close();
            }
        }
    }
}

