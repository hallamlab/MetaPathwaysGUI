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

    resultTabs->addTab(TabFactory::createTable(5,5, NULL),"Nuc. Data");
}

ResultWindow::~ResultWindow()
{
    delete ui;
}
