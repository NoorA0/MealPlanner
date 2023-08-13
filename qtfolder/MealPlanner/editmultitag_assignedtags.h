#ifndef EDITMULTITAG_ASSIGNEDTAGS_H
#define EDITMULTITAG_ASSIGNEDTAGS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class EditMultitag_AssignedTags;
}

class EditMultitag_AssignedTags : public QDialog
{
    Q_OBJECT

public:
    explicit EditMultitag_AssignedTags(QWidget *parent = nullptr,
                                       MealManager *mm = nullptr,
                                       MultiTag *tagPtr = nullptr);
    ~EditMultitag_AssignedTags();

public slots:
    void receiveRequestedMeals(unsigned int requestedMeals) { this->requestedMeals = requestedMeals; }

private slots:
    void on_pushButton_clicked();

    void on_pushButton_unassignTag_clicked();

    void on_pushButton_assignTag_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::EditMultitag_AssignedTags *ui;
    MealManager *mm;
    MultiTag *tagPtr;
    unsigned int requestedMeals; // received upon assigning a Tag

    // UI vars
    QMap<QString, QPair<Tag*, int>> itemToTag; // all item strings mapped to their Tags
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

    /* BuildItemString
     *
     * Generates the String to use with listWidgets,
     *  assigned Tags should display the number of requested Meals
     *  while unassigned Tags should not
     *
     *  use when moving a tag from assigned/unassigned, before RefreshTagsList()
     *
     *  INPUTS:
     *      Tag *tagPtr                 : pointer to the Tag
     *      unsigned int requestedMeals : number of requested Meals, set arbitrary value when below is FALSE
     *      bool displayRequestedMeals  : false when Tag is unassigned
     *
     *  OUTPUTS:
     *      QString : the prepared Item
     */
    QString BuildItemString(const Tag *tagPtr,
                            const unsigned int &requestedMeals,
                            const bool &displayRequestedMeals);
};

#endif // EDITMULTITAG_ASSIGNEDTAGS_H
