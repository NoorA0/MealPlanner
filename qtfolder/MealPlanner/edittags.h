#ifndef EDITTAGS_H
#define EDITTAGS_H

#include <QDialog>
#include <QListWidgetItem>
#include <QMap>
#include "mealmanager.h"

namespace Ui {
class EditTags;
}

class EditTags : public QDialog
{
    Q_OBJECT

public slots:
    void receiveNewTag(Tag* createdTag) { newTag = createdTag; }
    void receiveBoolAssignNewMeal(bool userResponse) { this->userResponse = userResponse; }

private slots:
    void on_listWidget_tags_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidget_tags_itemDoubleClicked(QListWidgetItem *item);

    void on_newTagButton_clicked();

    void on_editTag_clicked();

    void on_deleteTagButton_clicked();

    void on_editAssignedMeals_clicked();

    void on_exitButton_clicked();

public:
    explicit EditTags(QWidget *parent = nullptr,
                      MealManager *mm = nullptr);
    ~EditTags();

private:
    Ui::EditTags *ui;
    MealManager *mm;
    QMap<QString, Tag*> itemToTag;
    QVector<QString> itemsInOrder;
    Tag *newTag;
    bool userResponse;

    // helper functions

    /* RebuildItems
     *
     * clears itemToTag and itemsInOrder,
     * then gets all Tags in MealManager.
     *
     * for every Tag found, construct an item string (used for listWidget),
     * and maps the string to the Tag used to create it
     *
     * itemsInOrder is also populated in alphabetical order of Tag names
    */
    void RebuildItems(void);

    /* RefreshTagsList
     *
     * calls RebuildItems and refreshes the listWidget for Tags
     *
     * use when a new Tag is created or edited
    */
    void RefreshTagsList(void);
};

#endif // EDITTAGS_H
