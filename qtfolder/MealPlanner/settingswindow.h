#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>
#include "mealmanager.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr, MealManager *mm = nullptr);
    ~SettingsWindow();

private slots:
    void on_backMainMenuButton_clicked();

    void on_editMealsButton_clicked();

    void on_editTagsButton_clicked();

    void on_editMultiTagsButton_clicked();

private:
    Ui::SettingsWindow *ui;
    MealManager *mm;
};

#endif // SETTINGSWINDOW_H
