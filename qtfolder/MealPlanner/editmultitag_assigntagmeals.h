#ifndef EDITMULTITAG_ASSIGNTAGMEALS_H
#define EDITMULTITAG_ASSIGNTAGMEALS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class EditMultitag_AssignTagMeals;
}

class EditMultitag_AssignTagMeals : public QDialog
{
    Q_OBJECT

public:
    explicit EditMultitag_AssignTagMeals(QWidget *parent = nullptr,
                                         MealManager *mm = nullptr,
                                         unsigned int requestedMeals = 0);
    ~EditMultitag_AssignTagMeals();

signals:
    void sendNumberRequestedMeals(unsigned int requestedMeals);

private slots:
    void on_pushButton_done_clicked();

private:
    Ui::EditMultitag_AssignTagMeals *ui;
    MealManager *mm;
};

#endif // EDITMULTITAG_ASSIGNTAGMEALS_H
