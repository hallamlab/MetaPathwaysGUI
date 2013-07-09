#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "resultpage.h"
#include "tabledata.h"
#include <QFileDialog>
#include <QDebug>

ResultWindow::ResultWindow(RunData *run, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    this->run = run;

    resultTabs = this->findChild<QTabWidget *>("resultTabs");

    resultTabs->removeTab(0);
    resultTabs->removeTab(0);

    QString nucFile = "test_sample2.nuc.stats";
    QString aminoFile = "test_sample2.amino.stats";

    ResultPage *rp = new ResultPage(this->run);
    TableData *td = new TableData(nucFile);

    resultTabs->addTab(rp, "Execution Summary");
    resultTabs->addTab(td, "Nuc. Data");
}

RunData* ResultWindow::getRunData(){
    return this->run;
}

void ResultWindow::exportData(){

}

ResultWindow::~ResultWindow()
{
    delete ui;
}
