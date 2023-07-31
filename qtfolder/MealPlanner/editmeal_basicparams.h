#ifndef EDITMEAL_BASICPARAMS_H
#define EDITMEAL_BASICPARAMS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class EditMeal_BasicParams;
}

class EditMeal_BasicParams : public QDialog
{
    Q_OBJECT

public:
    explicit EditMeal_BasicParams(QWidget *parent = nullptr,
                                  MealManager *mm = nullptr,
                                  Meal *mealPtr = nullptr);
    ~EditMeal_BasicParams();

private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::EditMeal_BasicParams *ui;
    MealManager *mm;
    Meal *mealPtr;
};

#endif // EDITMEAL_BASICPARAMS_H
