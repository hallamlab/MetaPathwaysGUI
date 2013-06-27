#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"
#include "qprocess.h"
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QFile>
#include "utilities.h"
#include "rundata.h"

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

            colorRunConfig(line);
       }
    }
}

void ProgressDialog::colorRunConfig(QString line){
    QRegExp whiteSpace("\\s");
    QRegExp commentLine("#[^\"\\n\\r]*");

    if (!commentLine.exactMatch(line) && line.length()>0){
        //if not a comment line
        QStringList splitList = line.split(whiteSpace);
        QString step = splitList.at(0).trimmed();
        QString operation = splitList.at(1).trimmed();

        QString status;
        if (splitList.size()==3) status = splitList.at(2).trimmed();

        //find the step equivalent on the chart
        QString stepName = run->getConfigMapping()->operator [](step);
        QGroupBox *group = pw->findChild<QGroupBox *>(stepName);
        QString radioName = stepName.remove(QRegExp("[a-z-]+_?")).toLower();

        if (!stepName.isEmpty()){
            QRadioButton *yesRadioButton = group->findChild<QRadioButton *>(radioName+"YES");
            QRadioButton *redoRadioButton = group->findChild<QRadioButton *>(radioName+"REDO");
            QRadioButton *skipRadioButton = group->findChild<QRadioButton *>(radioName+"SKIP");

            qDebug() << radioName+"YES";

            if (status.operator ==("Failed")){
                group->setStyleSheet("background-color:red");
            }else if (status.operator ==("Skipping")){
                group->setStyleSheet("background-color:grey");
            }else if (step.operator ==("Running")){
                group->setStyleSheet("background-color:blue");
            }else{
                group->setStyleSheet("background-color:green");
            }
            yesRadioButton->setStyleSheet("background-color:none");
            redoRadioButton->setStyleSheet("background-color:none");
            skipRadioButton->setStyleSheet("background-color:none");
        }
    }


}

void ProgressDialog::closeEvent ( QCloseEvent * event ){
    timer->stop();
    this->destroy();
}

void ProgressDialog::terminateRun(){
    timer->stop();
    this->destroy();
}


ProgressDialog::~ProgressDialog()
{
    myProcess->kill();
    timer->stop();
    delete ui;
}
