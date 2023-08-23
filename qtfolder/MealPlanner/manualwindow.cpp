#include "manualwindow.h"
#include "ui_manualwindow.h"
#include "manual_summaryinfo.h"
#include "manual_mealinfo.h"
#include "manual_taginfo.h"
#include "manual_multitaginfo.h"
#include "manual_exampleinfo.h"

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
    Manual_MealInfo *mmi = new Manual_MealInfo(this);
    mmi->setAttribute(Qt::WA_DeleteOnClose);
    mmi->exec();
}

// tag info
void ManualWindow::on_explainTagsButton_clicked()
{
    Manual_TagInfo *mti = new Manual_TagInfo(this);
    mti->setAttribute(Qt::WA_DeleteOnClose);
    mti->exec();
}

// multitag info
void ManualWindow::on_explainMultiTagsButton_clicked()
{
    Manual_MultitagInfo *mmi = new Manual_MultitagInfo(this);
    mmi->setAttribute(Qt::WA_DeleteOnClose);
    mmi->exec();
}

// example use cases
void ManualWindow::on_explainExamplesButton_clicked()
{
    Manual_ExampleInfo *mei = new Manual_ExampleInfo(this);
    mei->setAttribute(Qt::WA_DeleteOnClose);
    mei->exec();
}

