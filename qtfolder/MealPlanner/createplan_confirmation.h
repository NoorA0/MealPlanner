#ifndef CREATEPLAN_CONFIRMATION_H
#define CREATEPLAN_CONFIRMATION_H

#include <QDialog>

namespace Ui {
class createplan_confirmation;
}

class createplan_confirmation : public QDialog
{
    Q_OBJECT

public:
    explicit createplan_confirmation(QWidget *parent = nullptr,
                                     const QString &fileName = QString(""),
                                     const int &planLength = 0,
                                     const double &planBudge = 0.00);
    ~createplan_confirmation();

signals:
    void sendConfirmation(const bool &isValid);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::createplan_confirmation *ui;
};

#endif // CREATEPLAN_CONFIRMATION_H
