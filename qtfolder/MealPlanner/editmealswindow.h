#ifndef EDITMEALSWINDOW_H
#define EDITMEALSWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class EditMealsWindow;
}

class EditMealsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditMealsWindow(QWidget *parent = nullptr);
    ~EditMealsWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_listWidget_meals_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_meals_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::EditMealsWindow *ui;
};

#endif // EDITMEALSWINDOW_H
