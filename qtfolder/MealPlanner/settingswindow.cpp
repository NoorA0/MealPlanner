#include "settingswindow.h"
#include "ui_settingswindow.h"

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

