#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "graphdata.h"
#include "tabledata.h"
#include <QFileDialog>
#include <QDebug>

ResultWindow::ResultWindow(RunData *run, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MetaPathways - Results and Data");
    this->run = run;

    resultTabs = this->findChild<QTabWidget *>("resultTabs");
    sampleSelect = this->findChild<QComboBox *>("sampleSelect");
    resultTabs->removeTab(0);
    resultTabs->removeTab(0);

    QString nucFile = "test_sample2.nuc.stats";
    QString aminoFile = "test_sample2.amino.stats";

    resultTabs->addTab(new TableData(true, nucFile), "data");
    resultTabs->addTab(new TableData(false, "fandt.txt"), "table");
    resultTabs->addTab(new GraphData(nucFile), "graph");


    QStringList samples = run->getParams()->value("filesSelected").split(",");
    samples.removeAll("");
    sampleSelect->addItems(samples);

    connect(sampleSelect,SIGNAL(currentIndexChanged(QString)),this,SLOT(sampleChanged(QString)));
}


void ResultWindow::sampleChanged(QString changed){
    qDebug() << changed;
}

RunData* ResultWindow::getRunData(){
    return this->run;
}


ResultWindow::~ResultWindow()
{
    delete ui;
}
