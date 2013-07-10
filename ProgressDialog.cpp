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

    cancelButton = this->findChild<QPushButton *>("cancelButton");
    textBrowser = this->findChild<QTextBrowser *>("textBrowser");
    progressBar = this->findChild<QProgressBar *>("progressBar");

    initProgressBar();

    timer = new QTimer(this);
    timer->start(1000);

//    QString program = "/usr/bin/python";
//    QStringList arguments;
//    arguments <<  "/Users/michaelwu/workspace/MetaPathways2-build-Desktop-Release/MetaPathways2.app/Contents/MacOS/test.py";
//    qDebug() << "declaring program";
//    myProcess = new QProcess();
//    qDebug() << "starting program";
//    myProcess->start(program, arguments);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateText()));
}

void ProgressDialog::initProgressBar(){
    progressBar->setMaximum(Utilities::countRunSteps(run->getParams()));
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
