#ifndef MANUAL_MULTITAGINFO_H
#define MANUAL_MULTITAGINFO_H

#include "qlabel.h"
#include <QDialog>

namespace Ui {
class Manual_MultitagInfo;
}

class Manual_MultitagInfo : public QDialog
{
    Q_OBJECT

public:
    explicit Manual_MultitagInfo(QWidget *parent = nullptr);
    ~Manual_MultitagInfo();

private slots:
    void on_pushButton_done_clicked();

    void on_pushButton_previousPage_clicked();

    void on_pushButton_nextPage_clicked();

private:
    Ui::Manual_MultitagInfo *ui;
    int currentPage;
    QList<QLabel*> pages;

    const QString PAGENUMBER_PREAMBLE = "<html><head/><body><p align=\"center\"><span style=\" font-size:10pt; font-weight:700;\">";
    const QString PAGENUMBER_ENDTAGS = "</span></p></body></html>";

    // helper functions
    void showPage(const int &pageNumber);
};

#endif // MANUAL_MULTITAGINFO_H
