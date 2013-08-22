#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"
#include "utilities.h"
#include "rundata.h"
#include "resultwindow.h"
#include <QProcess>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QMovie>
#include <QTextStream>
#include <QDir>

ProgressDialog::ProgressDialog(ParentWidget *pw, RunData *run, QWidget *parent) : QWidget(parent), ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    this->run = run;
    this->pw = pw;

    this->setWindowTitle("MetaPathways - Run Progress");
    cancelButton = this->findChild<QPushButton *>("cancelButton");
    logBrowser = this->findChild<QTextBrowser *>("logBrowser");
    logLabel = this->findChild<QLabel *>("logLabel");
    summaryTable = this->findChild<QTableWidget *>("summaryTable");
    summaryLabel = this->findChild<QLabel *>("summaryLabel");
    progressBar = this->findChild<QProgressBar *>("progressBar");
    globalProgressBar = this->findChild<QProgressBar *>("globalProgressBar");
    progressLabel = this->findChild<QLabel *>("progressLabel");
    hideButton = this->findChild<QPushButton *>("hideButton");

    summaryTable->setSortingEnabled(false);

    initProcess();
    initMapping();
    initProgressBar();
    checkFiles();

    currentFile = "";

    timer = new QTimer(this);
    timer->start(1000);

    connect(hideButton, SIGNAL(clicked()), this, SLOT(toggleDetails()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateText()));
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTable()));
    connect(myProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(changed(QProcess::ProcessState)));
}

void ProgressDialog::updateTable(){
    QString METAPATH = this->run->getConfig()->operator []("METAPATHWAYS_PATH");
    QString pathToLog = METAPATH + "/output/" + currentFile + "/metapathways_steps_log.txt";
    QFile inputFile(pathToLog);
    QHash<QString, QString> statusHash;
    QRegExp whiteSpace("\\t");
    QRegExp commentLine("#[^\"\\n\\r]*");


    if (inputFile.open(QIODevice::ReadOnly) && inputFile.exists())
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            if (!commentLine.exactMatch(line) && line.length()>0){
                //if not a comment line
                QStringList splitList = line.split(whiteSpace);
                QString step = splitList.at(0).trimmed();
                QString status = splitList.at(1).trimmed();
                QString stepName = step;
                if (!step.isEmpty())  statusHash[stepName] = status;
            }
       }
       foreach(const  QString key, statusHash.keys()) {
           colorRunConfig(key, statusHash[key]);
       }


    }
}

/*
 * There is no mapping from the table to the groupboxes. Therefore, this function will declare
 * the mapping of a row index to its groupbox by augmenting TABLE_MAPPING.
 */
void ProgressDialog::initMapping(){
    TABLE_MAPPING = new QHash<int,QString>();

    TABLE_MAPPING->operator [](0) = "PREPROCESS_FASTA";
    TABLE_MAPPING->operator [](1) = "ORF_PREDICTION";
    TABLE_MAPPING->operator [](2) = "GFF_TO_AMINO";
    TABLE_MAPPING->operator [](3) = "FILTERED_FASTA";
    TABLE_MAPPING->operator [](4) = "COMPUTE_REFSCORE";
    TABLE_MAPPING->operator [](5) = "BLAST_REFDB";
    TABLE_MAPPING->operator [](6) = "PARSE_BLAST";
    TABLE_MAPPING->operator [](7) = "SCAN_RRNA";
    TABLE_MAPPING->operator [](8) = "SCAN_TRNA";
    TABLE_MAPPING->operator [](9) = "STATS_RRNA";
    TABLE_MAPPING->operator [](10) = "ANNOTATE";
    TABLE_MAPPING->operator [](11) = "PATHOLOGIC_INPUT";
    TABLE_MAPPING->operator [](12) = "GENBANK_FILE";
    TABLE_MAPPING->operator [](13) = "CREATE_SEQUIN_FILE";
    TABLE_MAPPING->operator [](14) = "CREATE_REPORT_FILES";
    TABLE_MAPPING->operator [](15) = "MLTREEMAP_CALCULATION";
    TABLE_MAPPING->operator [](16) = "MLTREEMAP_IMAGEMAKER";
    TABLE_MAPPING->operator [](17) = "PATHOLOGIC";
}

void ProgressDialog::colorRunConfig(const QString stepName, const QString status){
    QImage *img;
    QTableWidgetItem *item = new QTableWidgetItem();
    QMovie *loading = new QMovie(":/images/loading.gif");
    QLabel *imageLabel = new QLabel();


    if( this->previousStatus.contains(stepName) && status == this->previousStatus[stepName]  ) return ;

    //clear old cell
    this->summaryTable->removeCellWidget(TABLE_MAPPING->key(stepName),0);
    this->summaryTable->setItem(TABLE_MAPPING->key(stepName),0, NULL);

    if (status.operator ==("FAILED")){
        img  = new QImage(":/images/cross.png");
        item->setData(Qt::DecorationRole, QPixmap::fromImage(*img).scaled(12,12));
        this->summaryTable->setItem(TABLE_MAPPING->key(stepName),0, item);
    }else if (status.operator ==("RUNNING")){
        imageLabel->setFixedSize(30,25);
        imageLabel->setMovie(loading);
        imageLabel->updateGeometry();
        loading->start();
        this->summaryTable->setCellWidget(TABLE_MAPPING->key(stepName),0,imageLabel);
    }else if (status.operator ==("SUCCESS") ||
              status.operator ==("ALREADY_COMPUTED") ||
              status.operator ==("SKIPPED")){
        img = new QImage(":/images/check.png");
        item->setData(Qt::DecorationRole, QPixmap::fromImage(*img).scaled(12,12));
        this->summaryTable->setItem(TABLE_MAPPING->key(stepName),0, item);
    }
    this->previousStatus.insert(stepName,status);
}

void ProgressDialog::toggleDetails(){
    if (!logBrowser->isHidden()) {
        logBrowser->hide();
    }else logBrowser->show();

    if (!logLabel->isHidden()) {
        logLabel->hide();
    }else logLabel->show();

    if (!summaryTable->isHidden()) {
        summaryTable->hide();
    }else summaryTable->show();

    if (!summaryLabel->isHidden()) {
        summaryLabel->hide();
    }else summaryLabel->show();

    this->adjustSize();
}


void ProgressDialog::initProcess(){
    QString program = run->getConfig()->operator []("PYTHON_EXECUTABLE");
    QStringList arguments;
    METAPATH = this->run->getConfig()->operator []("METAPATHWAYS_PATH");

    arguments <<  METAPATH + "/MetaPathways.py";
    arguments << "-i" << this->run->getParams()->operator []("fileInput");

    if (this->run->getConfig()->value("metapaths_steps:BLAST_REFDB")=="grid"){
        arguments << "-o" << METAPATH + "/output/distribute";
    }else arguments << "-o" << METAPATH + "/output";

    arguments << "-p" << METAPATH + "/template_param.txt";
    arguments << "-c" << METAPATH + "/template_config.txt";
    arguments << "-r" << this->run->getParams()->operator []("overwrite");

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    env.insert("CURRDIR", METAPATH);
    env.insert("METAPATH", METAPATH);
    env.insert("METAPATHLIB", METAPATH + "/libs");
    env.insert("STARCLUSTERLIB", METAPATH + "/libs/starcluster");
    env.insert("PYTHONPATH", METAPATH + "/libs:" + METAPATH + "/libs/starcluster");

    myProcess = new QProcess();
    myProcess->setProcessEnvironment(env);
    myProcess->start(program, arguments);

    run->setProcess(myProcess);

    //qDebug() << env.toStringList();
    //qDebug() << program << arguments;
}

void ProgressDialog::changed(QProcess::ProcessState state){
    qDebug() << "process state changed to " << state;
    qDebug() << myProcess->readAllStandardOutput();
    qDebug() << myProcess->readAllStandardError();
}

void ProgressDialog::initProgressBar(){
    progressBar->setMaximum(Utilities::countRunSteps(run->getParams()) + run->nADB + run->nRRNADB);
    progressBar->setValue(stepsPassed + this->run->nADB + this->run->nRRNADB - 2);
    progressBar->setMinimum(0);
}

void ProgressDialog::updateText(){
    logBrowser->clear();
    QString sampleStepsLogPath = METAPATH + "/output/" + currentFile + "/metapathways_steps_log.txt";
    QFile inputFile(sampleStepsLogPath);

    if (inputFile.open(QIODevice::ReadOnly))
    {
       stepsPassed = 0;
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            logBrowser->append(line);
            stepsPassed++;
       }
    }
    progressBar->setValue(stepsPassed);
}

void ProgressDialog::selectedFileChanged(QString file){
    currentFile = file;
    progressLabel->setText("Progress - " + currentFile);
}

void ProgressDialog::checkFiles(){
    QDir currentDir(this->run->getParams()->operator []("fileInput"));
    QString fileType = this->run->getParams()->operator []("INPUT:format");
    filesDetected = new QStringList();

    currentDir.setFilter(QDir::Files);
    QStringList entries = currentDir.entryList();

    for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry )
    {
        QString temp = *entry;
        QStringList file = temp.split(".");
        if (fileType == "fasta"){
            if (file.last()=="fa" || file.last()=="fas" || file.last()=="fasta" ||
                file.last()=="faa" || file.last()=="f" ||file.last()=="fna"){
                filesDetected->append(file.first());
            }
        }
        else if (fileType == "gbk-annotated" || fileType == "gbk-unannotated"){
            if (file.last() == "gbk"){
                filesDetected->append(file.first());
            }
        }
        else if (fileType == "gff-annotated" || fileType == "gff-unannotated"){
            if (file.last() == "gff"){
                filesDetected->append(file.first());
            }
        }
    }
    this->run->files = filesDetected;
}

void ProgressDialog::terminateRun(){
    timer->stop();
    this->close();
    delete ui;
}

ProgressDialog::~ProgressDialog()
{
    myProcess->kill();
    timer->stop();
    delete ui;
}
