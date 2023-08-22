#ifndef MANUAL_SUMMARYINFO_H
#define MANUAL_SUMMARYINFO_H

#include "qlabel.h"
#include <QDialog>

namespace Ui {
class Manual_SummaryInfo;
}

class Manual_SummaryInfo : public QDialog
{
    Q_OBJECT

public:
    explicit Manual_SummaryInfo(QWidget *parent = nullptr);
    ~Manual_SummaryInfo();

private slots:
    void on_pushButton_done_clicked();

    void on_pushButton_previousPage_clicked();

    void on_pushButton_nextPage_clicked();

private:
    Ui::Manual_SummaryInfo *ui;
    int currentPage;
    QList<QLabel*> pages;

    const QString PAGENUMBER_PREAMBLE = "<html><head/><body><p align=\"center\"><span style=\" font-size:10pt; font-weight:700;\">";
    const QString PAGENUMBER_ENDTAGS = "</span></p></body></html>";

    // helper functions
    void showPage(const int &pageNumber);

};

#endif // MANUAL_SUMMARYINFO_H
