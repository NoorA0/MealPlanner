#include "createmeal_basicparams.h"
#include "ui_createmeal_basicparams.h"
#include "createmeal_nameconflict.h"

createmeal_basicparams::createmeal_basicparams(QWidget *parent,
                                               MealManager *mm) :
    QDialog(parent),
    ui(new Ui::createmeal_basicparams)
{
    if (mm == nullptr)
    {
        close();
    }
    else
    {
        this->mm = mm;
    }

    ui->setupUi(this);

    // set value limits
    ui->lineEdit_name->setMaxLength(this->mm->getMaximumNameLength());
    ui->doubleSpinBox_price->setMinimum(this->mm->getMinimumPrice());
    ui->doubleSpinBox_price->setMaximum(this->mm->getMaximumPrice());
    ui->spinBox_duration->setMinimum(0);
    ui->spinBox_duration->setMaximum(this->mm->getMealDurationLimit());
    ui->spinBox_daysbetweenoccurrences->setMinimum(0);
    ui->spinBox_daysbetweenoccurrences->setMaximum(this->mm->getDaysBetweenOccurrencesLimit());

    // default meal to enabled
    ui->checkBox_isEnabled->setChecked(true);
}

createmeal_basicparams::~createmeal_basicparams()
{
    delete ui;
}

// confirm clicked
void createmeal_basicparams::on_pushButton_confirm_clicked()
{
    // get inputs
    Meal* newMeal = nullptr;
    QString name = ui->lineEdit_name->text();
    double price = ui->doubleSpinBox_price->value();
    int duration = ui->spinBox_duration->value();
    int dbo      = ui->spinBox_daysbetweenoccurrences->value();

    // dont bother creating if name is empty
    if (!name.isEmpty() && name.trimmed() != "")
        newMeal = this->mm->createMeal(name, price, duration, dbo);

    // if failed due to name conflict
    if (newMeal == nullptr)
    {
        CreateMeal_NameConflict *window = new CreateMeal_NameConflict(this);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->exec();
        ui->lineEdit_name->clear();
    }
    else
    {
        // send the meal
        emit sendNewMeal(newMeal);
        close();
    }
}

// cancel clicked
void createmeal_basicparams::on_pushButton_cancel_clicked()
{
    close();
}

