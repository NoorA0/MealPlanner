#ifndef CREATEPLAN_FILENAME_H
#define CREATEPLAN_FILENAME_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class createPlan_Filename;
}

class createPlan_Filename : public QDialog
{
    Q_OBJECT

public:
    explicit createPlan_Filename(QWidget *parent = nullptr,
                                 MealManager *mm = nullptr);
    ~createPlan_Filename();

signals:
    void sendFileName(const bool &isValid, const QString &fileName);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::createPlan_Filename *ui;
    MealManager *mm;
};

#endif // CREATEPLAN_FILENAME_H
