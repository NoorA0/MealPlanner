#include "createplan_confirmation.h"
#include "ui_createplan_confirmation.h"
#include <QDebug>

const QString PART_ONE = "<html><head/><body><p align=\"center\"><span style=\" font-weight:700;\">Please Confirm</span>:<br/></p><p align=\"center\">File Name: <span style=\" font-weight:700;\">";
const QString PART_TWO = "</span></p><p align=\"center\">Plan Length (weeks): <span style=\" font-weight:700;\">";
const QString PART_THREE = "</span></p><p align=\"center\">Plan Budget: <span style=\" font-weight:700;\">";
const QString PART_FOUR = "</span></p></body></html>";

createplan_confirmation::createplan_confirmation(QWidget *parent,
                                                 const QString &fileName,
                                                 const int &planLength,
                                                 const double &planBudget) :
    QDialog(parent),
    ui(new Ui::createplan_confirmation)
{
    ui->setupUi(this);    

    // gather values and prepare text
    QString newText = PART_ONE
                      + fileName
                      + PART_TWO
                      + QString::number(planLength)
                      + PART_THREE
                      + QString::number(planBudget, 'g', 8) // sets precision to 8 digits
                      + PART_FOUR;

    // set text
    ui->label->setText(newText);
}

createplan_confirmation::~createplan_confirmation()
{
    delete ui;
}

// OK clicked
void createplan_confirmation::on_pushButton_2_clicked()
{
    emit sendConfirmation(true);
    close();
}


// Cancel clicked
void createplan_confirmation::on_pushButton_clicked()
{
    emit sendConfirmation(false);
    close();
}

