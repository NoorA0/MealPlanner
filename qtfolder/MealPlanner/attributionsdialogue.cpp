#include "attributionsdialogue.h"
#include "ui_attributionsdialogue.h"
#include "mainwindow.h"

AttributionsDialogue::AttributionsDialogue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttributionsDialogue)
{
    ui->setupUi(this);
}

AttributionsDialogue::~AttributionsDialogue()
{
    delete ui;

    // unhide the main window
    QMainWindow* mw = qobject_cast<QMainWindow*>(parent());
    if (mw != nullptr)
    {
        mw->show();
    }
}

void AttributionsDialogue::on_pushButton_clicked()
{
    this->close();
}

