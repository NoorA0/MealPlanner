#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "editmealswindow.h"
#include "edittagswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent, MealManager *mm) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    if (mm == nullptr)
        close();
    else
        this->mm = mm;

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
    EditMealsWindow *emw = new EditMealsWindow(this, this->mm);
    emw->setAttribute(Qt::WA_DeleteOnClose);
    emw->show();
}

// edit tags clicked
void SettingsWindow::on_editTagsButton_clicked()
{
    hide();
    EditTagsWindow *etw = new EditTagsWindow(this, this->mm);
    etw->setAttribute(Qt::WA_DeleteOnClose);
    etw->show();
}

// edit multitags clicked
void SettingsWindow::on_editMultiTagsButton_clicked()
{
    /*
    hide();
    EditMultitagsWindow *emw = new EditMultitagsWindow(this, this->mm);
    emw->setAttribute(Qt::WA_DeleteOnClose);
    emw->show();
    */
}

