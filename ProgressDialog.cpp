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

ProgressDialog::ProgressDialog(ParentWidget *pw, QWidget *parent) : QWidget(parent), ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    this->run = RunData::getRunData();
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
    standardOut = this->findChild<QTextEdit *>("standardOut");

    summaryTable->setSortingEnabled(false);

    blastFailed = false;
    parseBlastFailed = false;
    scanRRNAFailed = false;
    statsFailed = false;

    stepsPassed = new QHash<QString, int>();

    initProcess();
    initMapping();
    checkFiles();

    currentFile = "";

    timer = new QTimer(this);
    timer->start(10000);

    connect(hideButton, SIGNAL(clicked()), this, SLOT(toggleDetails()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
    connect(timer, SIGNAL(timeout()), this, SLOT(readStepsLog()));
}

void ProgressDialog::readStepsLog(){
    QString OUTPUTPATH = this->run->getParams()->operator []("folderOutput");
    QString pathToLog = OUTPUTPATH + "/" + currentFile + "/metapathways_steps_log.txt";

    QFile inputFile(pathToLog);
    QHash<QString, QString> statusHash;
    QRegExp whiteSpace("\\t");
    QRegExp commentLine("#[^\"\\n\\r]*");

    logBrowser->clear();

    blastCount = 0;
    parseBlastCount = 0;
    scanRRNACount = 0;
    statsCount = 0;
    if (inputFile.open(QIODevice::ReadOnly) && inputFile.exists())
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();

            if (!commentLine.exactMatch(line) && line.length()>0){
                //if not a comment line
                logBrowser->append(line);

                QStringList splitList = line.split(whiteSpace);
                QString step = splitList.at(0).trimmed();
                QString status = splitList.at(1).trimmed();
                QString stepName = step;
                if (!step.isEmpty())  statusHash[stepName] = status;
            }
       }
       inputFile.close();
    }

    foreach(const QString key, statusHash.keys()) {
        colorRunConfig(key, statusHash[key]);
    }

    initProgressBar();

    QByteArray read = myProcess->readAll();
    if (!read.isEmpty()) standardOut->append(QString(read));
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
    TABLE_MAPPING->operator [](7) = "SCAN_rRNA";
    TABLE_MAPPING->operator [](8) = "STATS_rRNA";
    TABLE_MAPPING->operator [](9) = "SCAN_tRNA";
    TABLE_MAPPING->operator [](10) = "ANNOTATE";
    TABLE_MAPPING->operator [](11) = "PATHOLOGIC_INPUT";
    TABLE_MAPPING->operator [](12) = "GENBANK_FILE";
    TABLE_MAPPING->operator [](13) = "CREATE_SEQUIN_FILE";
    TABLE_MAPPING->operator [](14) = "CREATE_REPORT_FILES";
    TABLE_MAPPING->operator [](15) = "MLTREEMAP_CALCULATION";
    TABLE_MAPPING->operator [](16) = "MLTREEMAP_IMAGEMAKER";
    TABLE_MAPPING->operator [](17) = "PATHOLOGIC";
}

void ProgressDialog::multiStepCheck(QString *stepName, QString *status){
    QStringList subStep = stepName->split("_");
    if (subStep.at(0) == "BLAST" && subStep.at(1) == "REFDB"){
        if (*status == "FAILED"){
            blastFailed = true;
        }
        blastCount++;
        *stepName = "BLAST_REFDB";

        if (blastFailed) *status == "FAILED";
        else *status == "RUNNING";

        if (blastCount == run->nADB){
            *status = "SUCCESS";
        }
    }
    else if (subStep.at(0) == "PARSE" && subStep.at(1) == "BLAST"){
        if (*status == "FAILED"){
            parseBlastFailed = true;
        }
        parseBlastCount++;
        *stepName = "PARSE_BLAST";

        if (parseBlastFailed) *status == "FAILED";
        else *status == "RUNNING";

        if (parseBlastCount == run->nADB){
            *status = "SUCCESS";
        }
    }
    else if (subStep.at(0) == "SCAN" && subStep.at(1) == "rRNA"){
        if (*status == "FAILED"){
            scanRRNAFailed = true;
        }
        scanRRNACount++;
        *stepName = "SCAN_rRNA";

        if (scanRRNAFailed) *status == "FAILED";
        else *status == "RUNNING";

        if (scanRRNACount == run->nRRNADB){
            *status = "SUCCESS";
        }
    }
    else if (subStep.at(0) == "STATS"){
        if (*status == "FAILED"){
            statsFailed = true;
        }
        statsCount++;
        *stepName = "STATS_rRNA";

        if (statsFailed) *status == "FAILED";
        else *status == "RUNNING";

        if (statsCount == run->nRRNADB){
            *status = "SUCCESS";
        }
    }

    //qDebug() << blastCount << parseBlastCount << scanRRNACount << statsCount;
}

void ProgressDialog::colorRunConfig(QString stepName, QString status){
    QImage *img;
    QTableWidgetItem *item = new QTableWidgetItem();
    QMovie *loading = new QMovie(":/images/loading.gif");
    QLabel *imageLabel = new QLabel();

    if( this->previousStatus.contains(stepName) && status == this->previousStatus[stepName]  ) return ;

    multiStepCheck(&stepName, &status);

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

    if (!stepsPassed->contains(currentFile)){
        stepsPassed->operator [](currentFile) = 0;
    }

    stepsPassed->operator [](currentFile) = stepsPassed->operator [](currentFile)++;

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

    if (!standardOut->isHidden()){
        standardOut->hide();
    }else standardOut->show();

    this->adjustSize();
}

void ProgressDialog::initProcess(){
    QString program = run->getConfig()->operator []("PYTHON_EXECUTABLE");
    QStringList arguments;
    METAPATH = this->run->getConfig()->operator []("METAPATHWAYS_PATH");

    arguments <<  METAPATH + "/MetaPathways.py";
    arguments << "-i" << this->run->getParams()->operator []("fileInput");

  //  if (this->run->getConfig()->value("metapaths_steps:BLAST_REFDB")=="grid"){
    arguments << "-o" << this->run->getParams()->operator []("folderOutput");

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
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(program, arguments);

    run->setProcess(myProcess);

    //qDebug() << env.toStringList();
    qDebug() << program << arguments;
}

void ProgressDialog::initProgressBar(){
    progressBar->setMaximum(Utilities::countRunSteps(run->getParams()) + run->nADB + run->nRRNADB);
    progressBar->setValue(stepsPassed->value(currentFile));
    progressBar->setMinimum(0);

    int globalSum = 0;
    for (int i=0;i<stepsPassed->values().length();i++){
        globalSum += stepsPassed->values().at(i);
    }

    globalProgressBar->setMaximum(progressBar->maximum() * this->run->files->length());
    globalProgressBar->setValue(globalSum);
    globalProgressBar->setMinimum(0);

    //qDebug() << *stepsPassed << globalSum << globalProgressBar->maximum() << progressBar->value() << progressBar->maximum();
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

    QList<QRegExp> regList;
    regList << QRegExp("[.][fF][aA][sS][tT][aA]$") << QRegExp("[.][fF][aA]$") << QRegExp("[.][fF][aA][aA]$") << QRegExp("[.][fF][aA][sS]") << QRegExp("[.][fF][nN][aA]$");


    for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry )
    {
        QString temp = *entry;
        QStringList file = temp.split(".");

        if (fileType == "fasta"){
            foreach(QRegExp reg, regList ) {
               if(temp.indexOf(reg,0) > -1 ) {
                   filesDetected->append( temp.remove(reg).replace('.','_') );
                   break;
               }
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
