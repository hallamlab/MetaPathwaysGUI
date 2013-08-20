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
#include <QDir>

ProgressDialog::ProgressDialog(ParentWidget *pw, RunData *run, QWidget *parent) : QWidget(parent), ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    this->run = run;
    this->pw = pw;

    this->setWindowTitle("MetaPathways - Run Progress");
    cancelButton = this->findChild<QPushButton *>("cancelButton");
    textBrowser = this->findChild<QTextBrowser *>("textBrowser");
    progressBar = this->findChild<QProgressBar *>("progressBar");

    initProcess();
    initProgressBar();

    currentFile = "";

    timer = new QTimer(this);
    timer->start(1000);

    fileCheckTimer = new QTimer(this);
    fileCheckTimer->start(500);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateText()));
    connect(fileCheckTimer, SIGNAL(timeout()),this,SLOT(checkFiles()));
    connect(myProcess, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(changed(QProcess::ProcessState)));

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
    arguments << "-r" << "overwrite";

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
}

void ProgressDialog::changed(QProcess::ProcessState state){
    qDebug() << "process state changed to " << state;
    qDebug() << myProcess->readAllStandardOutput();
    qDebug() << myProcess->readAllStandardError();
}

void ProgressDialog::initProgressBar(){
    progressBar->setMaximum(Utilities::countRunSteps(run->getParams()) + run->nADB + run->nRRNADB);
    progressBar->setValue(stepsPassed);
    progressBar->setMinimum(0);
}

void ProgressDialog::updateText(){
    textBrowser->clear();
    QString sampleStepsLogPath = METAPATH + "/output/" + currentFile + "/metapathways_steps_log.txt";
    QFile inputFile(sampleStepsLogPath);

    if (inputFile.open(QIODevice::ReadOnly))
    {
       stepsPassed = 0;
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            textBrowser->append(line);
            stepsPassed++;
       }
    }
    progressBar->setValue(stepsPassed);
}

void ProgressDialog::selectedFileChanged(QString file){
    currentFile = file;
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
