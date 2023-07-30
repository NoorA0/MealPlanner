#ifndef CREATEMEAL_NAMECONFLICT_H
#define CREATEMEAL_NAMECONFLICT_H

#include <QDialog>

namespace Ui {
class CreateMeal_NameConflict;
}

class CreateMeal_NameConflict : public QDialog
{
    Q_OBJECT

public:
    explicit CreateMeal_NameConflict(QWidget *parent = nullptr);
    ~CreateMeal_NameConflict();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateMeal_NameConflict *ui;
};

#endif // CREATEMEAL_NAMECONFLICT_H
