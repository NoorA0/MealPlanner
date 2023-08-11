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

private slots:
    void on_pushButton_clicked();

    void on_pushButton_unassignTag_clicked();

    void on_pushButton_assignTag_clicked();

private:
    Ui::EditMultitag_AssignedTags *ui;
    MealManager *mm;
    MultiTag *tagPtr;
    QVector<Tag*> tagsCopy; // copy of Tags already assigned to the MultiTag

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

#endif // EDITMULTITAG_ASSIGNEDTAGS_H
