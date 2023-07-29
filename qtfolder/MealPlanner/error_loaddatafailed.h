#ifndef ERROR_LOADDATAFAILED_H
#define ERROR_LOADDATAFAILED_H

#include <QDialog>

namespace Ui {
class Error_LoadDataFailed;
}

class Error_LoadDataFailed : public QDialog
{
    Q_OBJECT

public:
    explicit Error_LoadDataFailed(QWidget *parent = nullptr,
                                  const bool &isCaughtException = false,
                                  const QString &exception = QString());
    ~Error_LoadDataFailed();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Error_LoadDataFailed *ui;
};

#endif // ERROR_LOADDATAFAILED_H
