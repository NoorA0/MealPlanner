#ifndef CREATEMULTITAG_PROMPTLINKTAGS_H
#define CREATEMULTITAG_PROMPTLINKTAGS_H

#include <QDialog>

namespace Ui {
class CreateMultitag_PromptLinkTags;
}

class CreateMultitag_PromptLinkTags : public QDialog
{
    Q_OBJECT

public:
    explicit CreateMultitag_PromptLinkTags(QWidget *parent = nullptr);
    ~CreateMultitag_PromptLinkTags();

signals:
    void sendUserResponse(bool userResponse);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::CreateMultitag_PromptLinkTags *ui;
};

#endif // CREATEMULTITAG_PROMPTLINKTAGS_H
