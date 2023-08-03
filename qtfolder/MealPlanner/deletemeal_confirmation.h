#ifndef DELETEMEAL_CONFIRMATION_H
#define DELETEMEAL_CONFIRMATION_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class DeleteMeal_Confirmation;
}

class DeleteMeal_Confirmation : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteMeal_Confirmation(QWidget *parent = nullptr,
                                     MealManager *mm = nullptr,
                                     Meal *mealPtr   = nullptr);
    ~DeleteMeal_Confirmation();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DeleteMeal_Confirmation *ui;
    MealManager *mm;
    Meal* mealPtr;
};

#endif // DELETEMEAL_CONFIRMATION_H
