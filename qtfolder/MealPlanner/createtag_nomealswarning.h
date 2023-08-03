#ifndef CREATETAG_NOMEALSWARNING_H
#define CREATETAG_NOMEALSWARNING_H

#include <QDialog>

namespace Ui {
class CreateTag_NoMealsWarning;
}

class CreateTag_NoMealsWarning : public QDialog
{
    Q_OBJECT

public:
    explicit CreateTag_NoMealsWarning(QWidget *parent = nullptr);
    ~CreateTag_NoMealsWarning();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateTag_NoMealsWarning *ui;
};

#endif // CREATETAG_NOMEALSWARNING_H
