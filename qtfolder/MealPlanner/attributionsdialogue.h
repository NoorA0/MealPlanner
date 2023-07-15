#ifndef ATTRIBUTIONSDIALOGUE_H
#define ATTRIBUTIONSDIALOGUE_H

#include <QDialog>

namespace Ui {
class AttributionsDialogue;
}

class AttributionsDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit AttributionsDialogue(QWidget *parent = nullptr);
    ~AttributionsDialogue();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AttributionsDialogue *ui;
};

#endif // ATTRIBUTIONSDIALOGUE_H
