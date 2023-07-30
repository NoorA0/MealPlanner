#include "editmealswindow.h"
#include "ui_editmealswindow.h"
#include "createmeal_basicparams.h"
#include <QDebug>

EditMealsWindow::EditMealsWindow(QWidget *parent, MealManager *mm) :
    QMainWindow(parent),
    ui(new Ui::EditMealsWindow)
{
    if (mm == nullptr)
        close();
    else
        this->mm = mm;

    ui->setupUi(this);
    QString itemStats = "\nName: Hamberders\nPrice: 2.05\nDuration: 1 days\nTags Assigned: 7\nEnabled On: [Mon, Tue, Wed, Thu, Fri, Sat, Sun]\n";
    QString itemStats2 = "\nName: Hamberders\nPrice: 2.05\nDuration: 1 days\nTags Assigned: 7\nEnabled On: [Mon, Tue, Wed, Thu, Fri, Sat, Sun]\n";
    ui->listWidget_meals->addItem(itemStats);
    ui->listWidget_meals->addItem(itemStats);
    ui->listWidget_meals->addItem(itemStats);
    ui->listWidget_meals->addItem(itemStats2);
    ui->listWidget_meals->addItem(itemStats2);
    ui->listWidget_meals->addItem(itemStats2);
    ui->listWidget_meals->addItem(itemStats);
    ui->listWidget_meals->addItem(itemStats2);
}

EditMealsWindow::~EditMealsWindow()
{
    delete ui;

    // unhide the previous window
    QMainWindow* mw = qobject_cast<QMainWindow*>(parent());
    if (mw != nullptr)
    {
        mw->show();
    }
}

// go back to settings menu
void EditMealsWindow::on_pushButton_clicked()
{
    close();
}

// edit selected item, same as double clicking. for convenience.
void EditMealsWindow::on_pushButton_2_clicked()
{
    // pass selected item to on_listWidget_itemDoubleClicked()
    on_listWidget_meals_itemDoubleClicked(ui->listWidget_meals->currentItem());
}

// item double clicked
void EditMealsWindow::on_listWidget_meals_itemDoubleClicked(QListWidgetItem *item)
{
    // TODO: this
    qDebug() << "item was double clicked: \n" << item->text();
}

// item changed, display it's assigned tags
void EditMealsWindow::on_listWidget_meals_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    qDebug() << "item changed to: \n" << current->text();
    QString placeholderTag = "\nName: Tag1\nDescription: no\nDepends on Multitag? No\nConsecutive occurrences allowed: 4\nEnabled on: [Mon, Wed]\nAssigned Meals: 3\n";
    ui->listWidget_tags->clear();
    ui->listWidget_tags->addItem(placeholderTag);
}

// edit tags assigned for selected meal
void EditMealsWindow::on_pushButton_3_clicked()
{
    // TODO: this

}

// create new meal
void EditMealsWindow::on_pushButton_4_clicked()
{
    // create meal by getting basic info
    createmeal_basicparams *window = new createmeal_basicparams(this, this->mm);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->exec();

    // prompt to link with tags
    // TODO: this

}

// delete selected meal
void EditMealsWindow::on_pushButton_5_clicked()
{
    // TODO: this
}

