#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"
#include "qprocess.h"
#include <QPushButton>
#include <QDebug>
#include <QString>

ProgressDialog::ProgressDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    cancelButton = this->findChild<QPushButton *>("cancelButton");

    QString program = "/usr/bin/python";
    QStringList arguments;
    arguments <<  "/Users/michaelwu/workspace/MetaPathways2-build-Desktop-Release/MetaPathways2.app/Contents/MacOS/test.py";
    qDebug() << "declaring program";

    myProcess = new QProcess();

    qDebug() << "starting program";

    myProcess->start(program, arguments);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
}

void ProgressDialog::terminateRun(){
    myProcess->kill();
    close();
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}
