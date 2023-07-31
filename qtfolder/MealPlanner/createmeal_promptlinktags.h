#ifndef CREATEMEAL_PROMPTLINKTAGS_H
#define CREATEMEAL_PROMPTLINKTAGS_H

#include <QDialog>

namespace Ui {
class CreateMeal_PromptLinkTags;
}

class CreateMeal_PromptLinkTags : public QDialog
{
    Q_OBJECT

public:
    explicit CreateMeal_PromptLinkTags(QWidget *parent = nullptr);
    ~CreateMeal_PromptLinkTags();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::CreateMeal_PromptLinkTags *ui;
};

#endif // CREATEMEAL_PROMPTLINKTAGS_H
