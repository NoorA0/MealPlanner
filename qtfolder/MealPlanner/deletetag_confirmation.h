#ifndef DELETETAG_CONFIRMATION_H
#define DELETETAG_CONFIRMATION_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class DeleteTag_Confirmation;
}

class DeleteTag_Confirmation : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteTag_Confirmation(QWidget *parent = nullptr,
                                    MealManager *mm = nullptr,
                                    Tag *tagPtr = nullptr);
    ~DeleteTag_Confirmation();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DeleteTag_Confirmation *ui;
    MealManager *mm;
    Tag *tagPtr;
};

#endif // DELETETAG_CONFIRMATION_H
