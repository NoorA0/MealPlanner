#include "error_loaddatafailed.h"
#include "ui_error_loaddatafailed.h"

Error_LoadDataFailed::Error_LoadDataFailed(QWidget *parent,
                                           const bool &isCaughtException,
                                           const QString &exception) :
    QDialog(parent),
    ui(new Ui::Error_LoadDataFailed)
{
    ui->setupUi(this);

    // if no exception, hide the exception info (type2 label)
    if (!isCaughtException)
    {
        ui->label_type2->setVisible(false);
        ui->errorbox_type2->setVisible(false);
    }
    else
    {
        // hide type1 label, display exception info
        ui->label_type1->setVisible(false);
        ui->errorbox_type2->setText(exception);
    }
}

Error_LoadDataFailed::~Error_LoadDataFailed()
{
    delete ui;
}

// clicked OK
void Error_LoadDataFailed::on_pushButton_clicked()
{
    close();
}

