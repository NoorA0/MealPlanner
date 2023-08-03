#ifndef EDITMEALSWINDOW_H
#define EDITMEALSWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMap>
#include "mealmanager.h"

namespace Ui {
class EditMealsWindow;
}

class EditMealsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditMealsWindow(QWidget *parent = nullptr,
                             MealManager *mm = nullptr);
    ~EditMealsWindow();

public slots:
    void receiveNewMeal(Meal* createdMeal) { newMeal = createdMeal; }
    void receiveBoolAssignNewTags(bool userResponse) { this->userResponse = userResponse; }

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_listWidget_meals_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_meals_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::EditMealsWindow *ui;
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

#endif // EDITMEALSWINDOW_H
