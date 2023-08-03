#ifndef EDITTAG_BASICPARAMS_H
#define EDITTAG_BASICPARAMS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class EditTag_BasicParams;
}

class EditTag_BasicParams : public QDialog
{
    Q_OBJECT

public:
    explicit EditTag_BasicParams(QWidget *parent = nullptr,
                                 MealManager *mm = nullptr,
                                 Tag *tagPtr = nullptr);
    ~EditTag_BasicParams();

private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::EditTag_BasicParams *ui;
    MealManager *mm;
    Tag *tagPtr;
};

#endif // EDITTAG_BASICPARAMS_H
