#ifndef CREATEPLAN_CREATIONDISPLAYERROR_H
#define CREATEPLAN_CREATIONDISPLAYERROR_H

#include <QDialog>

namespace Ui {
class CreatePlan_CreationDisplayError;
}

class CreatePlan_CreationDisplayError : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlan_CreationDisplayError(QWidget *parent = nullptr,
                                             const unsigned int &daysErrored = 0,
                                             const double &failedBudget = 0.0);
    ~CreatePlan_CreationDisplayError();

private slots:
    void on_pushButton_OK_clicked();

private:
    Ui::CreatePlan_CreationDisplayError *ui;
};

#endif // CREATEPLAN_CREATIONDISPLAYERROR_H
