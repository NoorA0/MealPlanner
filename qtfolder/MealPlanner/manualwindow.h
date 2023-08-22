#ifndef MANUALWINDOW_H
#define MANUALWINDOW_H

#include <QMainWindow>

namespace Ui {
class ManualWindow;
}

class ManualWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ManualWindow(QWidget *parent = nullptr);
    ~ManualWindow();

private slots:
    void on_returnMenuButton_clicked();

    void on_explainUseButton_clicked();

    void on_explainMealsButton_clicked();

    void on_explainTagsButton_clicked();

    void on_explainMultiTagsButton_clicked();

    void on_explainExamplesButton_clicked();

private:
    Ui::ManualWindow *ui;
};

#endif // MANUALWINDOW_H
