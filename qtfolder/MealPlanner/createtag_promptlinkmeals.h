#ifndef CREATETAG_PROMPTLINKMEALS_H
#define CREATETAG_PROMPTLINKMEALS_H

#include <QDialog>

namespace Ui {
class CreateTag_PromptLinkMeals;
}

class CreateTag_PromptLinkMeals : public QDialog
{
    Q_OBJECT

public:
    explicit CreateTag_PromptLinkMeals(QWidget *parent = nullptr);
    ~CreateTag_PromptLinkMeals();

signals:
    void sendUserResponse(bool userResponse);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::CreateTag_PromptLinkMeals *ui;
};

#endif // CREATETAG_PROMPTLINKMEALS_H
