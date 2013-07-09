#include "resultpage.h"
#include "ui_resultpage.h"

ResultPage::ResultPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultPage)
{
    ui->setupUi(this);
}

ResultPage::~ResultPage()
{
    delete ui;
}
