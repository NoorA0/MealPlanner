#ifndef CREATEPLAN_NOMEALSORTAGSWARNING_H
#define CREATEPLAN_NOMEALSORTAGSWARNING_H

#include <QDialog>

namespace Ui {
class CreatePlan_NoMealsOrTagsWarning;
}

class CreatePlan_NoMealsOrTagsWarning : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlan_NoMealsOrTagsWarning(QWidget *parent = nullptr);
    ~CreatePlan_NoMealsOrTagsWarning();

private slots:
    void on_pushButton_OK_clicked();

private:
    Ui::CreatePlan_NoMealsOrTagsWarning *ui;
};

#endif // CREATEPLAN_NOMEALSORTAGSWARNING_H
