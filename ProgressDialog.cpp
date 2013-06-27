#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"
#include "qprocess.h"
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QFile>
#include "utilities.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    cancelButton = this->findChild<QPushButton *>("cancelButton");
    textBrowser = this->findChild<QTextBrowser *>("textBrowser");
    timer = new QTimer(this);
    timer->start(1000);

    QString program = "/usr/bin/python";
    QStringList arguments;
    arguments <<  "/Users/michaelwu/workspace/MetaPathways2-build-Desktop-Release/MetaPathways2.app/Contents/MacOS/test.py";
    qDebug() << "declaring program";

    myProcess = new QProcess();

    qDebug() << "starting program";

    myProcess->start(program, arguments);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateText()));
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
    myProcess->kill();
    close();
}


ProgressDialog::~ProgressDialog()
{
    delete ui;
}
