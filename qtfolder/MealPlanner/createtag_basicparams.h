#ifndef CREATETAG_BASICPARAMS_H
#define CREATETAG_BASICPARAMS_H

#include <QDialog>
#include "mealmanager.h"

namespace Ui {
class CreateTag_BasicParams;
}

class CreateTag_BasicParams : public QDialog
{
    Q_OBJECT

public:
    explicit CreateTag_BasicParams(QWidget *parent = nullptr,
                                   MealManager *mm = nullptr);
    ~CreateTag_BasicParams();

signals:
    void sendNewTag(Tag *createdTag);

private slots:
    void on_pushButton_confirm_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::CreateTag_BasicParams *ui;
    MealManager *mm;
};

#endif // CREATETAG_BASICPARAMS_H
