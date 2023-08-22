#include "manual_summaryinfo.h"
#include "ui_manual_summaryinfo.h"



Manual_SummaryInfo::Manual_SummaryInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Manual_SummaryInfo)
{
    ui->setupUi(this);

    currentPage = 1;

    // store all ui labels in pages, showPage will automatially update from here
    pages.push_back(ui->label_page1);
    pages.push_back(ui->label_page2);
    pages.push_back(ui->label_page3);
    pages.push_back(ui->label_page4);
    pages.push_back(ui->label_page5);

    // set active page
    showPage(currentPage);
}

Manual_SummaryInfo::~Manual_SummaryInfo()
{
    delete ui;
}

void Manual_SummaryInfo::showPage(const int &pageNumber)
{
    // check validity
    if (pageNumber-1 <= pages.size())
    {
        // hide all pages
        for (auto page : pages)
        {
            page->hide();
        }

        // show new page
        pages.at(pageNumber-1)->show();

        // create new label and set it
        QString setLabel = PAGENUMBER_PREAMBLE +
                           "Page " +
                           QString::number(pageNumber) +
                           "/" +
                           QString::number(pages.size()) +
                           PAGENUMBER_ENDTAGS;

        ui->label_pageNumber->setText(setLabel);
    }
}

// clicked done
void Manual_SummaryInfo::on_pushButton_done_clicked()
{
    close();
}

// show previous page
void Manual_SummaryInfo::on_pushButton_previousPage_clicked()
{
    if (currentPage > 1)
        showPage(--currentPage);
}

// show next page
void Manual_SummaryInfo::on_pushButton_nextPage_clicked()
{
    if (currentPage < pages.size())
        showPage(++currentPage);
}

