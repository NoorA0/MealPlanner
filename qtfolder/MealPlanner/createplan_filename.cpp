#include "createplan_filename.h"
#include "ui_createplan_filename.h"
#include<QDebug>

const QString DEFAULT_FILENAME = "New_Plan";

createPlan_Filename::createPlan_Filename(QWidget *parent,
                                         MealManager *mm) :
    QDialog(parent),
    ui(new Ui::createPlan_Filename)
{
    if (mm == nullptr)
        close();
    else
    {
        this->mm = mm;
    }
    ui->setupUi(this);

    // set lineEdit limits
    ui->lineEdit->setMaxLength(mm->getMaximumFileNameLength());
}

createPlan_Filename::~createPlan_Filename()
{
    delete ui;
}

// OK button clicked
void createPlan_Filename::on_pushButton_clicked()
{
    // get input text
    QString text = ui->lineEdit->displayText();
    bool isValid = true; // user did not cancel

    // check input length
    if (text.length() == 0)
        text = DEFAULT_FILENAME;

    // send file name over signal
    emit sendFileName(isValid, text);
    close();
}

// Cancel button clicked
void createPlan_Filename::on_pushButton_2_clicked()
{
    bool isValid = false; // user quit generation
    emit sendFileName(isValid, QString(""));
    close();
}

