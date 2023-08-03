#ifndef CREATETAG_NAMECONFLICT_H
#define CREATETAG_NAMECONFLICT_H

#include <QDialog>

namespace Ui {
class CreateTag_NameConflict;
}

class CreateTag_NameConflict : public QDialog
{
    Q_OBJECT

public:
    explicit CreateTag_NameConflict(QWidget *parent = nullptr);
    ~CreateTag_NameConflict();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateTag_NameConflict *ui;
};

#endif // CREATETAG_NAMECONFLICT_H
