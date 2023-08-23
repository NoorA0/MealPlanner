#include "manual_exampleinfo.h"
#include "ui_manual_exampleinfo.h"

Manual_ExampleInfo::Manual_ExampleInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Manual_ExampleInfo)
{
    ui->setupUi(this);

    currentPage = 1;

    // store all ui labels in pages, showPage will manage them
    pages.push_back(ui->label_page1);
    pages.push_back(ui->label_page2);
    pages.push_back(ui->label_page3);
    pages.push_back(ui->label_page4);

    // set active page
    showPage(currentPage);
}

Manual_ExampleInfo::~Manual_ExampleInfo()
{
    delete ui;
}

void Manual_ExampleInfo::showPage(const int& pageNumber)
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

// said done
void Manual_ExampleInfo::on_pushButton_done_clicked()
{
    close();
}

// show previous page
void Manual_ExampleInfo::on_pushButton_previousPage_clicked()
{
    if (currentPage > 1)
        showPage(--currentPage);
}

// show next page
void Manual_ExampleInfo::on_pushButton_nextPage_clicked()
{
    if (currentPage < pages.size())
        showPage(++currentPage);
}

