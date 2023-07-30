#ifndef CREATEMEAL_BASICPARAMS_H
#define CREATEMEAL_BASICPARAMS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class createmeal_basicparams;
}

class createmeal_basicparams : public QDialog
{
    Q_OBJECT

public:
    explicit createmeal_basicparams(QWidget *parent = nullptr, MealManager *mm = nullptr);
    ~createmeal_basicparams();

private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::createmeal_basicparams *ui;
    MealManager *mm;
};

#endif // CREATEMEAL_BASICPARAMS_H
