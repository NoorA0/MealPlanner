#ifndef EDITMULTITAG_BASICPARAMS_H
#define EDITMULTITAG_BASICPARAMS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class EditMultitag_BasicParams;
}

class EditMultitag_BasicParams : public QDialog
{
    Q_OBJECT

public:
    explicit EditMultitag_BasicParams(QWidget *parent = nullptr,
                                      MealManager *mm = nullptr,
                                      MultiTag *tag = nullptr);
    ~EditMultitag_BasicParams();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_confirm_clicked();

private:
    Ui::EditMultitag_BasicParams *ui;
    MealManager *mm;
    MultiTag *tag;
};

#endif // EDITMULTITAG_BASICPARAMS_H
