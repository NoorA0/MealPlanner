#ifndef EDITTAGSWINDOW_H
#define EDITTAGSWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMap>
#include "mealmanager.h"

namespace Ui {
class EditTagsWindow;
}

class EditTagsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditTagsWindow(QWidget *parent = nullptr,
                            MealManager *mm = nullptr);
    ~EditTagsWindow();

    void receiveNewTag(Tag* createdTag) { newTag = createdTag; }
    void receiveBoolAssignNewMeal(bool userResponse) { this->userResponse = userResponse; }

private slots:
    void on_pushButton_exit_clicked();

    void on_pushButton_newTag_clicked();

    void on_pushButton_editTag_clicked();

    void on_pushButton_deleteTag_clicked();

    void on_pushButton_editAssignedMeals_clicked();

    void on_listWidget_tags_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidget_tags_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::EditTagsWindow *ui;
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

#endif // EDITTAGSWINDOW_H
