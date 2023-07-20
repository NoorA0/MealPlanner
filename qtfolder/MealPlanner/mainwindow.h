#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

private:
    Ui::MainWindow *ui;
    QString fileName;
    int planLength;
    double planBudget;
    bool settingsConfirmed;
};
#endif // MAINWINDOW_H
