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
    explicit CreatePlan_InProgress(QWidget *parent = nullptr);
    ~CreatePlan_InProgress();

private:
    Ui::CreatePlan_InProgress *ui;

private slots:
    void closeWindow(void);
};

#endif // CREATEPLAN_INPROGRESS_H
