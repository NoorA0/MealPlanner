#ifndef EDITTAG_ASSIGNEDMEALS_H
#define EDITTAG_ASSIGNEDMEALS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class EditTag_AssignedMeals;
}

class EditTag_AssignedMeals : public QDialog
{
    Q_OBJECT

public:
    explicit EditTag_AssignedMeals(QWidget *parent = nullptr,
                                   MealManager *mm = nullptr,
                                   Tag* tagPtr = nullptr);
    ~EditTag_AssignedMeals();

private slots:
    void on_pushButton_unassignMeal_clicked();

    void on_pushButton_assignMeal_clicked();

    void on_pushButton_clicked();

private:
    Ui::EditTag_AssignedMeals *ui;
    MealManager *mm;
    Tag *tagPtr;

    // UI vars
    QMap<QString, Meal*> itemToMeal; // all item strings mapped to their Meals
    QVector<QString> itemsInOrder_Assigned; // items that belong to assigned Meals
    QVector<QString> itemsInOrder_Unassigned; // items that belong to unassigned Meals

    // helper functions

    /* RefreshMealsList
     *
     * Refreshes the listWidget for Meals
     *
     * use when a Meal is moved from assigned/unassigned
    */
    void RefreshMealsList(void);
};

#endif // EDITTAG_ASSIGNEDMEALS_H
