#include "deletemeal_confirmation.h"
#include "ui_deletemeal_confirmation.h"

const QString MEAL_PREAMBLE = "<html><head/><body><p align=\"center\">Name: <span style=\" font-weight:700;\">";
const QString PRICE_PREAMBLE = "</span></p><p align=\"center\">Price: <span style=\" font-weight:700;\">";
const QString DURATION_PREAMBLE = "</span></p><p align=\"center\">Duration: <span style=\" font-weight:700;\">";
const QString DAYS_BETWEEN_OCC_PREAMBLE = "</span></p><p align=\"center\">Days between occurrences: <span style=\" font-weight:700;\">";
const QString END_TAGS = "</span></p></body></html>";

DeleteMeal_Confirmation::DeleteMeal_Confirmation(QWidget *parent,
                                                 MealManager *mm,
                                                 Meal *mealPtr) :
    QDialog(parent),
    ui(new Ui::DeleteMeal_Confirmation)
{
    if (mm == nullptr || mealPtr == nullptr)
        close();
    else
    {
        this->mm = mm;
        this->mealPtr = mealPtr;
    }
    ui->setupUi(this);

    // display meal's information
    QString labelText = MEAL_PREAMBLE + mealPtr->getName()
                       + PRICE_PREAMBLE + QString::number(mealPtr->getPrice())
                       + DURATION_PREAMBLE;

    labelText += QString::number(mealPtr->getMealDuration());
    labelText += (mealPtr->getMealDuration() > 1)? " days" : " day";

    labelText += DAYS_BETWEEN_OCC_PREAMBLE + QString::number(mealPtr->getDaysBetweenOccurrences());
    labelText += (mealPtr->getDaysBetweenOccurrences() > 1)? " days" : " day";

    labelText += END_TAGS;

    ui->label_mealInfo->setText(labelText);
}

DeleteMeal_Confirmation::~DeleteMeal_Confirmation()
{
    delete ui;
}

// said yes, delete the meal
void DeleteMeal_Confirmation::on_pushButton_clicked()
{
    mm->deleteMeal(mealPtr);
    mealPtr = nullptr;
    close();
}

// cancel delete
void DeleteMeal_Confirmation::on_pushButton_2_clicked()
{
    close();
}

