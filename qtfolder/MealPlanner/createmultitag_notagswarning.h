#ifndef CREATEMULTITAG_NOTAGSWARNING_H
#define CREATEMULTITAG_NOTAGSWARNING_H

#include <QDialog>

namespace Ui {
class CreateMultitag_NoTagsWarning;
}

class CreateMultitag_NoTagsWarning : public QDialog
{
    Q_OBJECT

public:
    explicit CreateMultitag_NoTagsWarning(QWidget *parent = nullptr);
    ~CreateMultitag_NoTagsWarning();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateMultitag_NoTagsWarning *ui;
};

#endif // CREATEMULTITAG_NOTAGSWARNING_H
