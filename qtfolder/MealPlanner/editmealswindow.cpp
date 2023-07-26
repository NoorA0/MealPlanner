#include "editmealswindow.h"
#include "ui_editmealswindow.h"
#include <QDebug>

EditMealsWindow::EditMealsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditMealsWindow)
{
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

}
