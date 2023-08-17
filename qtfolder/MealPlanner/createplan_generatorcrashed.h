#ifndef CREATEPLAN_GENERATORCRASHED_H
#define CREATEPLAN_GENERATORCRASHED_H

#include <QDialog>

namespace Ui {
class CreatePlan_GeneratorCrashed;
}

class CreatePlan_GeneratorCrashed : public QDialog
{
    Q_OBJECT

public:
    explicit CreatePlan_GeneratorCrashed(QWidget *parent = nullptr);
    ~CreatePlan_GeneratorCrashed();

private slots:
    void on_pushButton_OK_clicked();

private:
    Ui::CreatePlan_GeneratorCrashed *ui;
};

#endif // CREATEPLAN_GENERATORCRASHED_H
