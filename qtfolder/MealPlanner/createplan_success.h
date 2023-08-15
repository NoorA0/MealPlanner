#ifndef CREATEPLAN_SUCCESS_H
#define CREATEPLAN_SUCCESS_H

#include <QDialog>

namespace Ui {
class CreatePlan_Success;
}

class CreatePlan_Success : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlan_Success(QWidget *parent = nullptr);
    ~CreatePlan_Success();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CreatePlan_Success *ui;
};

#endif // CREATEPLAN_SUCCESS_H
