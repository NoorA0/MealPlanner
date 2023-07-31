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
    explicit EditMealsWindow(QWidget *parent = nullptr, MealManager *mm = nullptr);
    ~EditMealsWindow();

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

    // helper functions

    /* RebuildMap
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

    /* CreateDaysEnabledString
     *
     * INPUTS:
     * const QMap<DaysOfTheWeek, bool> enabledDays : the enabled days for a Meal/Tag
     *
     * OUTPUTS:
     * QString : contains "[Mon, Tue, Wed, ..., Sun]" according to the days enabled
     */
    QString CreateDaysEnabledString(const QMap<DaysOfTheWeek, bool> enabledDays);

};

#endif // EDITMEALSWINDOW_H
