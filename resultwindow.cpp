#include "resultwindow.h"
#include "ui_resultwindow.h"
#include "graphdata.h"
#include "tabledata.h"
#include <QFileDialog>
#include <QDebug>

ResultWindow::ResultWindow(ProgressDialog *prog, RunData *run, QWidget *parent) :
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
    connect(getFileNames, SIGNAL(timeout()),this,SLOT(updateFileNames()));
    connect(this,SIGNAL(fileChanged(QString)),this->progress,SLOT(selectedFileChanged(QString)));

}

void ResultWindow::updateFileNames(){
    files = *this->run->files;

    QStringList existing;
    for (int i=0;i<sampleSelect->count();i++){
        existing.append(sampleSelect->itemText(i));
    }

    foreach (QString f, files){
        if (!existing.contains(f)){
            sampleSelect->addItem(f);
        }
    }
}


void ResultWindow::sampleChanged(QString changed){
    QString METAPATH = this->run->getConfig()->operator []("METAPATHWAYS_PATH");

    const QString nucFile = METAPATH + "/output/" + changed + "/run_statistics/" + changed + ".nuc.stats";
    const QString aminoFile = METAPATH + "/output/" + changed + "/run_statistics/" + changed + ".amino.stats";
    const QString funcFile = METAPATH + "/output/" + changed + "/results/annotation_table" + "/functional_and_taxonomic_table.txt";

    QFile nucStats(nucFile);
    QFile aminoStats(aminoFile);
    QFile funcTable(funcFile);

    resultTabs->clear();
    QList<enum TYPE> types;
    if (nucStats.exists()) resultTabs->addTab(new TableData(false, true, nucFile), "Nuclieotide Statistics");
    if (aminoStats.exists()) resultTabs->addTab(new TableData(false, true, aminoFile), "Amino Statistics");

    resultTabs->addTab(new GraphData("/Users/michaelwu/workspace/MetaPathways2-build-Desktop-Release/MetaPathways2.app/Contents/MacOS/contig_lengths.txt"),"Contig Lengths");
    resultTabs->addTab(new GraphData("/Users/michaelwu/workspace/MetaPathways2-build-Desktop-Release/MetaPathways2.app/Contents/MacOS/orf_lengths.txt"),"Orf Lengths");

    types.clear();
    types << STRING << STRING << INT << INT << STRING << STRING << STRING << STRING;
    resultTabs->addTab(new TableData(true, false, "/Users/michaelwu/workspace/MetaPathways2-build-Desktop-Release/MetaPathways2.app/Contents/MacOS/functional_and_taxonomic_table.txt", types),"functional and taxonomic table");

    emit fileChanged(changed);
}

RunData* ResultWindow::getRunData(){
    return this->run;
}


ResultWindow::~ResultWindow()
{
    delete ui;
}
