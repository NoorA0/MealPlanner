#ifndef EDITMEAL_ASSIGNEDTAGS_H
#define EDITMEAL_ASSIGNEDTAGS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class EditMeal_AssignedTags;
}

class EditMeal_AssignedTags : public QDialog
{
    Q_OBJECT

public:
    explicit EditMeal_AssignedTags(QWidget *parent = nullptr,
                                   MealManager *mm = nullptr,
                                   Meal* mealPtr = nullptr);
    ~EditMeal_AssignedTags();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_unassignTag_clicked();

    void on_pushButton_assignTag_clicked();

private:
    Ui::EditMeal_AssignedTags *ui;
    MealManager *mm;
    Meal *mealPtr;
    //QVector<Tag*> assignCache;   // cache of Tags to assign to the Meal
    //QVector<Tag*> unassignCache; // cache of Tags to unassign from the Meal
    QVector<Tag*> mealTagsCopy;  // copy of Tags already assigned to the Meal

    // UI vars
    QMap<QString, Tag*> itemToTag; // all item strings mapped to their Tags
    QVector<QString> itemsInOrder_Assigned; // items that belong to the assigned Tags
    QVector<QString> itemsInOrder_Unassigned; // items that belong to the unassigned Tags

    // helper functions

    /* RefreshTagsList
     *
     * Refreshes the listWidget for Tags
     *
     * use when a Tag is moved from assigned/unassigned
    */
    void RefreshTagsList(void);
};

#endif // EDITMEAL_ASSIGNEDTAGS_H
