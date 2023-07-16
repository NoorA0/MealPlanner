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

private:
    Ui::createPlan_Length *ui;
};

#endif // CREATEPLAN_LENGTH_H
