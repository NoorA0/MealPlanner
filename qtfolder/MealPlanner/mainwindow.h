#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mealmanager.h"
#include <fstream>
#include <chrono>
#include <ctime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr,
               MealManager *mm = nullptr);
    ~MainWindow();

signals:
    void closeModalWindow(void);

private slots:
    void getFileName(const bool &isValid, const QString &newFileName);

    void getPlanLength(const bool &isValid, const int &newLength);

    void getPlanBudget(const bool &isValid, const double &newBudget);

    void getConfirmation(const bool &isValid);

    void on_settingsButton_clicked();

    void on_manualButton_clicked();

    void on_attributionsButton_clicked();

    void on_quitProgramButton_clicked();

    void on_generatePlanButton_clicked();

    void on_returnFromManualPage_clicked();

    void on_returnFromSettingsPage_clicked();

    void on_explainExamplesButton_clicked();

    void on_explainUseButton_clicked();

    void on_explainMealsButton_clicked();

    void on_explainTagsButton_clicked();

    void on_explainMultiTagsButton_clicked();

    void on_editMealsButton_clicked();

    void on_editTagsButton_clicked();

    void on_editMultiTagsButton_clicked();

private:
    Ui::MainWindow *ui;
    MealManager *mm;
    QString fileName;
    int planLength; // in weeks
    double planBudget;
    bool settingsConfirmed;

    // titles for each page in QStackedLayout
    const QString titleBarTitle_MainMenu = "Main Menu";
    const QString titleBarTitle_Settings = "Program Settings";
    const QString titleBarTitle_CreatePlan = "Create a Meal Plan";
    const QString titleBarTitle_ProgramManual = "User Manual";

    // returns to the main menu page of the QStackedWidget and sets the title
    // used by the "return to main menu" button in the other pages
    void ReturnToMainMenuPage();
};
#endif // MAINWINDOW_H
