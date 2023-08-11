#ifndef CREATEMULTITAG_BASICPARAMS_H
#define CREATEMULTITAG_BASICPARAMS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class CreateMultitag_BasicParams;
}

class CreateMultitag_BasicParams : public QDialog
{
    Q_OBJECT

public:
    explicit CreateMultitag_BasicParams(QWidget *parent = nullptr,
                                        MealManager *mm = nullptr);
    ~CreateMultitag_BasicParams();

signals:
    void sendNewMultitag(MultiTag *createdTag);

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_confirm_clicked();

private:
    Ui::CreateMultitag_BasicParams *ui;
    MealManager *mm;
};

#endif // CREATEMULTITAG_BASICPARAMS_H
