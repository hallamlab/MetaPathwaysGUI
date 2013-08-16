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

    timer = new QTimer(this);
    timer->start(1000);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateText()));

    connect(myProcess,SIGNAL(stateChanged(QProcess::ProcessState)),this,SLOT(changed(QProcess::ProcessState)));
}

void ProgressDialog::initProcess(){
    QString program = run->getConfig()->operator []("PYTHON_EXECUTABLE");
    QStringList arguments;
    QString METAPATH = this->run->getConfig()->operator []("METAPATHWAYS_PATH");

    arguments <<  METAPATH + "/MetaPathways.py";
    arguments << "-i" << this->run->getParams()->operator []("fileInput");

    if (this->run->getConfig()->value("metapaths_steps:BLAST_REFDB").compare("grid")==0){
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

    QByteArray pythonEnvValue = qgetenv("PYTHONPATH");
    qDebug() << pythonEnvValue.constData();

//    myProcess = new QProcess();
//    myProcess->setProcessEnvironment(env);
//    myProcess->start(program, arguments);
//    run->setProcess(myProcess);
}

void ProgressDialog::changed(QProcess::ProcessState state){
    qDebug() << myProcess->readAllStandardOutput() << myProcess->readAllStandardError() << state;
}

void ProgressDialog::initProgressBar(){
    progressBar->setMaximum(Utilities::countRunSteps(run->getParams()));
    progressBar->setValue(4);
    progressBar->setMinimum(0);
}

void ProgressDialog::updateText(){
    textBrowser->clear();
    QFile inputFile("metapathways_steps_log.txt");

    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            textBrowser->append(line);
       }
    }
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
