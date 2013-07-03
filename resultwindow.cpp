#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "tabfactory.h"

ResultWindow::ResultWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);

    resultTabs = this->findChild<QTabWidget *>("resultTabs");
    resultTabs->removeTab(0);
    resultTabs->removeTab(0);

    QString nucFile = "test_sample2.nuc.stats";
    QString aminoFile = "test_sample2.amino.stats";

    nucWidget = TabFactory::createTable(nucFile);
    aminoWidget = TabFactory::createTable(aminoFile);

    resultTabs->addTab(nucWidget,"Nuc. Data");
    resultTabs->addTab(aminoWidget,"Amino Data");

    nucTable = nucWidget->findChild<QTableWidget *>(nucFile);
    aminoTable = nucWidget->findChild<QTableWidget *>(aminoFile);
}


ResultWindow::~ResultWindow()
{
    delete ui;
}
