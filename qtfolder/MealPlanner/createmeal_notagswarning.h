#ifndef CREATEMEAL_NOTAGSWARNING_H
#define CREATEMEAL_NOTAGSWARNING_H

#include <QDialog>

namespace Ui {
class CreateMeal_NoTagsWarning;
}

class CreateMeal_NoTagsWarning : public QDialog
{
    Q_OBJECT

public:
    explicit CreateMeal_NoTagsWarning(QWidget *parent = nullptr);
    ~CreateMeal_NoTagsWarning();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateMeal_NoTagsWarning *ui;
};

#endif // CREATEMEAL_NOTAGSWARNING_H
