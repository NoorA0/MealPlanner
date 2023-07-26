#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "editmealswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    connect(ui->backMainMenuButton, &QPushButton::clicked, this, &SettingsWindow::on_backMainMenuButton_clicked);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;

    // unhide the main window
    QMainWindow* mw = qobject_cast<QMainWindow*>(parent());
    if (mw != nullptr)
    {
        mw->show();
    }
}

void SettingsWindow::on_backMainMenuButton_clicked()
{
    this->close();
}


// edit meals clicked
void SettingsWindow::on_editMealsButton_clicked()
{
    hide();
    EditMealsWindow *emw = new EditMealsWindow(this);
    emw->setAttribute(Qt::WA_DeleteOnClose);
    emw->show();
}

// edit tags clicked
void SettingsWindow::on_editTagsButton_clicked()
{

}

// edit multitags clicked
void SettingsWindow::on_editMultiTagsButton_clicked()
{

}

