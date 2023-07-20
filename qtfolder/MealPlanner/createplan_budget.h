#ifndef CREATEPLAN_BUDGET_H
#define CREATEPLAN_BUDGET_H

#include <QDialog>

namespace Ui {
class createPlan_Budget;
}

class createPlan_Budget : public QDialog
{
    Q_OBJECT

public:
    explicit createPlan_Budget(QWidget *parent = nullptr);
    ~createPlan_Budget();

signals:
    void sendPlanBudget(const bool &isValid, const double &budget);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::createPlan_Budget *ui;
};

#endif // CREATEPLAN_BUDGET_H
