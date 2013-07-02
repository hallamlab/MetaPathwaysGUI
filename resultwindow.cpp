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

    resultTabs->addTab(TabFactory::createTable("test_sample2.nuc.stats"),"Nuc. Data");
    resultTabs->addTab(TabFactory::createTable("test_sample2.nuc.stats"),"Amino Data");


}

ResultWindow::~ResultWindow()
{
    delete ui;
}
