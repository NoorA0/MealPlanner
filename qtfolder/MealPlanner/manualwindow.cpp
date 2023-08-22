#include "manualwindow.h"
#include "ui_manualwindow.h"
#include "manual_summaryinfo.h"

ManualWindow::ManualWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ManualWindow)
{
    ui->setupUi(this);
}

ManualWindow::~ManualWindow()
{
    delete ui;

    // unhide the previous window
    QMainWindow* mw = qobject_cast<QMainWindow*>(parent());
    if (mw != nullptr)
    {
        mw->show();
    }
}

// back to main menu
void ManualWindow::on_returnMenuButton_clicked()
{
    this->close();
}

// general overview
void ManualWindow::on_explainUseButton_clicked()
{
    Manual_SummaryInfo *msi = new Manual_SummaryInfo(this);
    msi->setAttribute(Qt::WA_DeleteOnClose);
    msi->exec();
}

// meal info
void ManualWindow::on_explainMealsButton_clicked()
{

}

// tag info
void ManualWindow::on_explainTagsButton_clicked()
{

}

// multitag info
void ManualWindow::on_explainMultiTagsButton_clicked()
{

}

// example use cases
void ManualWindow::on_explainExamplesButton_clicked()
{

}

