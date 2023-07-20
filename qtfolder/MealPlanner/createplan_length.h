#ifndef CREATEPLAN_LENGTH_H
#define CREATEPLAN_LENGTH_H

#include <QDialog>

namespace Ui {
class createPlan_Length;
}

class createPlan_Length : public QDialog
{
    Q_OBJECT

public:
    explicit createPlan_Length(QWidget *parent = nullptr);
    ~createPlan_Length();

signals:
    void sendPlanLength(const bool &isValid, const int &length);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::createPlan_Length *ui;
};

#endif // CREATEPLAN_LENGTH_H
