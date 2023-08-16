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

private slots:
    void getFileName(const bool &isValid, const QString &newFileName);

    void getPlanLength(const bool &isValid, const int &newLength);

    void getPlanBudget(const bool &isValid, const double &newBudget);

    void getConfirmation(const bool &isValid);

    void getCreationStatus(const int &returnCode,
                           const unsigned int &daysErrored,
                           const double &failedBudget);

    void on_settingsButton_clicked();

    void on_manualButton_clicked();

    void on_attributionsButton_clicked();

    void on_quitProgramButton_clicked();

    void on_generatePlanButton_clicked();

private:
    Ui::MainWindow *ui;
    MealManager *mm;
    QString fileName;
    int planLength; // in weeks
    double planBudget;
    bool settingsConfirmed;

    // received after generating a plan
    int returnCode;
    unsigned int erroredDays;
    double failedBudget;

};
#endif // MAINWINDOW_H
