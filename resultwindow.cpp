#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "graphdata.h"
#include "tabledata.h"
#include <QFileDialog>
#include <QDebug>

ResultWindow::ResultWindow(ResultPage *rp, ProgressDialog *prog, RunData *run, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MetaPathways - Results and Data");
    this->run = run;
    this->progress = prog;

    resultTabs = this->findChild<QTabWidget *>("resultTabs");
    sampleSelect = this->findChild<QComboBox *>("sampleSelect");
    resultTabs->clear();

    getFileNames = new QTimer();
    getFileNames->start(500);

    connect(sampleSelect,SIGNAL(currentIndexChanged(QString)),this,SLOT(sampleChanged(QString)));
    connect(sampleSelect, SIGNAL(currentIndexChanged(QString)), rp, SLOT(updateFile(QString)));
    connect(getFileNames, SIGNAL(timeout()),this,SLOT(updateFileNames()));
    connect(this,SIGNAL(fileChanged(QString)),this->progress,SLOT(selectedFileChanged(QString)));

}

void ResultWindow::updateFileNames(){
    files = *this->run->files;

    QStringList existing;
    for (int i=0;i<sampleSelect->count();i++){
        existing.append(sampleSelect->itemText(i));
    }

    sampleSelect->blockSignals(true);
    foreach (QString f, files){
        if (!existing.contains(f)){
            sampleSelect->addItem(f);
        }
    }
    sampleSelect->blockSignals(false);
}


void ResultWindow::sampleChanged(QString changed){
    QString METAPATH = this->run->getConfig()->operator []("METAPATHWAYS_PATH");

    QString nucFile = METAPATH + "/output/" + changed + "/run_statistics/" + changed + ".nuc.stats";
    QString aminoFile = METAPATH + "/output/" + changed + "/run_statistics/" + changed + ".amino.stats";

    QFile nucStats(nucFile);
    QFile aminoStats(aminoFile);

    resultTabs->clear();
    //if (nucStats.exists()) resultTabs->addTab(new TableData(true, nucFile), "Nuclieotide Statistics");
    //if (aminoStats.exists()) resultTabs->addTab(new TableData(true, aminoFile), "Amino Statistics");

    emit fileChanged(changed);
}

RunData* ResultWindow::getRunData(){
    return this->run;
}


ResultWindow::~ResultWindow()
{
    delete ui;
}
