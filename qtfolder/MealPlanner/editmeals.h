#ifndef EDITMEALS_H
#define EDITMEALS_H

#include <QDialog>
#include <QListWidgetItem>
#include <QMap>
#include "mealmanager.h"

namespace Ui {
class EditMeals;
}

class EditMeals : public QDialog
{
    Q_OBJECT

public:
    explicit EditMeals(QWidget *parent = nullptr,
                       MealManager *mm = nullptr);
    ~EditMeals();

public slots:
    void receiveNewMeal(Meal* createdMeal) { newMeal = createdMeal; }
    void receiveBoolAssignNewTags(bool userResponse) { this->userResponse = userResponse; }

private slots:

    void on_listWidget_meals_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_meals_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_newMealButton_clicked();

    void on_editMealButton_clicked();

    void on_deleteMealButton_clicked();

    void on_editTagsButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::EditMeals *ui;
    MealManager *mm;
    QMap<QString, Meal*> itemToMeal;
    QVector<QString> itemsInOrder;
    Meal* newMeal;
    bool userResponse;

    // helper functions

    /* RebuildItems
     *
     * clears itemToMeal and itemsInOrder,
     * then gets all meals in MealManager.
     *
     * for every meal found, construct an item string (used for listWidget),
     * and maps the string to the Meal* used to create it
     *
     * itemsInOrder is also populated in alphabetical order of Meal names
    */
    void RebuildItems(void);

    /* RefreshMealsList
     *
     * calls RebuildItems and refreshes the listWidget for Meals
     *
     * use when a new Meal is created or edited
    */
    void RefreshMealsList(void);
};

#endif // EDITMEALS_H
