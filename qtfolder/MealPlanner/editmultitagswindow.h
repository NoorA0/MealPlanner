#ifndef EDITMULTITAGSWINDOW_H
#define EDITMULTITAGSWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "qlistwidget.h"
#include "mealmanager.h"

namespace Ui {
class EditMultitagsWindow;
}

class EditMultitagsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditMultitagsWindow(QWidget *parent = nullptr,
                                 MealManager *mm = nullptr);
    ~EditMultitagsWindow();

public slots:
    void receiveNewMultiTag(MultiTag *createdMT) { newMT = createdMT; }
    void receiveBoolAssignNewTags(bool userResponse) { this->userResponse = userResponse; }

private slots:
    void on_pushButton_exit_clicked();

    void on_pushButton_deleteMultitag_clicked();

    void on_pushButton_newMultitag_clicked();

    void on_pushButton_editMultitag_clicked();

    void on_listWidget_multitags_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_editLinkedTags_clicked();

    void on_listWidget_multitags_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::EditMultitagsWindow *ui;
    MealManager *mm;
    QMap<QString, MultiTag*> itemToMultitag;
    QVector<QString> itemsInOrder;
    MultiTag *newMT;
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

#endif // EDITMULTITAGSWINDOW_H
