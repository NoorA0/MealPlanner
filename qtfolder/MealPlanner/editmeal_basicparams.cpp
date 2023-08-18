#include "editmeal_basicparams.h"
#include "createmeal_nameconflict.h"
#include "ui_editmeal_basicparams.h"

EditMeal_BasicParams::EditMeal_BasicParams(QWidget *parent,
                                           MealManager *mm,
                                           Meal *mealPtr) :
    QDialog(parent),
    ui(new Ui::EditMeal_BasicParams)
{
    if (mm == nullptr || mealPtr == nullptr)
        close();
    else
    {
        this->mm = mm;
        this->mealPtr = mealPtr;
    }

    ui->setupUi(this);

    // set value limits
    ui->lineEdit_name->setMaxLength(this->mm->getMaximumNameLength());
    ui->doubleSpinBox_price->setMinimum(this->mm->getMinimumPrice());
    ui->doubleSpinBox_price->setMaximum(this->mm->getMaximumPrice());
    ui->spinBox_duration->setMinimum(1);
    ui->spinBox_duration->setMaximum(this->mm->getMealDurationLimit());
    ui->spinBox_daysbetweenoccurrences->setMinimum(0);
    ui->spinBox_daysbetweenoccurrences->setMaximum(this->mm->getDaysBetweenOccurrencesLimit());

    // initialize to meal's values
    ui->lineEdit_name->setText(mealPtr->getName());
    ui->doubleSpinBox_price->setValue(mealPtr->getPrice());
    ui->spinBox_duration->setValue(mealPtr->getMealDuration());
    ui->spinBox_daysbetweenoccurrences->setValue(mealPtr->getDaysBetweenOccurrences());
    ui->checkBox_isEnabled->setChecked(!mealPtr->isDisabled());
}

EditMeal_BasicParams::~EditMeal_BasicParams()
{
    delete ui;
}

// confirm clicked
void EditMeal_BasicParams::on_pushButton_confirm_clicked()
{
    // get inputs
    QString name = ui->lineEdit_name->text();
    double price = ui->doubleSpinBox_price->value();
    int duration = ui->spinBox_duration->value();
    int dbo      = ui->spinBox_daysbetweenoccurrences->value();
    bool isEnabled = ui->checkBox_isEnabled->isChecked();

    // if same name, then change the rest of the params
    if (name.toUpper() == mealPtr->getName().toUpper())
    {
        mealPtr->setPrice(price);
        mealPtr->setMealDuration(duration);
        mealPtr->setDaysBetweenOccurrences(dbo);
        mealPtr->setDisabled(!isEnabled);
        close();
    }
    else // check if conflicts
    {
        if (name.isEmpty() || name.trimmed() == "" || mm->findMeal(name) != nullptr)
        {
            // name conflicts
            CreateMeal_NameConflict *window = new CreateMeal_NameConflict(this);
            window->setAttribute(Qt::WA_DeleteOnClose);
            window->exec();

            // reset name field
            ui->lineEdit_name->setText(mealPtr->getName());
        }
        else
        {
            // re-sort meal according to alphabetical order
            if (mm->resortMeal(mealPtr, name) != 0)
            {
                // sort failed, show warning to user
                CreateMeal_NameConflict *window = new CreateMeal_NameConflict(this);
                window->setAttribute(Qt::WA_DeleteOnClose);
                window->exec();

                // reset name field
                ui->lineEdit_name->setText(mealPtr->getName());
            }
            else // re-sort successful
            {
                mealPtr->setPrice(price);
                mealPtr->setMealDuration(duration);
                mealPtr->setDaysBetweenOccurrences(dbo);
                mealPtr->setDisabled(!isEnabled);
                close();
            }
        }
    }
}

// cancel clicked
void EditMeal_BasicParams::on_pushButton_cancel_clicked()
{
    close();
}

