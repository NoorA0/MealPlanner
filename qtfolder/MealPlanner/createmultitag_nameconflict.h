#ifndef CREATEMULTITAG_NAMECONFLICT_H
#define CREATEMULTITAG_NAMECONFLICT_H

#include <QDialog>

namespace Ui {
class CreateMultitag_NameConflict;
}

class CreateMultitag_NameConflict : public QDialog
{
    Q_OBJECT

public:
    explicit CreateMultitag_NameConflict(QWidget *parent = nullptr);
    ~CreateMultitag_NameConflict();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateMultitag_NameConflict *ui;
};

#endif // CREATEMULTITAG_NAMECONFLICT_H
