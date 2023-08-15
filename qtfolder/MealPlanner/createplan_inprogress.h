#ifndef CREATEPLAN_INPROGRESS_H
#define CREATEPLAN_INPROGRESS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class CreatePlan_InProgress;
}

class CreatePlan_InProgress : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlan_InProgress(QWidget *parent = nullptr,
                                   MealManager *mm = nullptr,
                                   const QString &fileName = "",
                                   const double &budget = 0,
                                   const int &durationWeeks = 1);
    ~CreatePlan_InProgress();

signals:
    void createPlanReturn(const int &returnCode);

private:
    Ui::CreatePlan_InProgress *ui;
};

#endif // CREATEPLAN_INPROGRESS_H
