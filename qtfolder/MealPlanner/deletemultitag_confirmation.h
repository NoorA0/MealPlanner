#ifndef DELETEMULTITAG_CONFIRMATION_H
#define DELETEMULTITAG_CONFIRMATION_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class DeleteMultitag_Confirmation;
}

class DeleteMultitag_Confirmation : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteMultitag_Confirmation(QWidget *parent = nullptr,
                                         MealManager *mm = nullptr,
                                         MultiTag *tagPtr = nullptr);
    ~DeleteMultitag_Confirmation();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DeleteMultitag_Confirmation *ui;
    MealManager *mm;
    MultiTag *tagPtr;
};

#endif // DELETEMULTITAG_CONFIRMATION_H
