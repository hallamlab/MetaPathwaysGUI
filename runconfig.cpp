#include "RunConfig.h"
#include "ui_RunConfig.h"
#include "qregexp.h"
#include "qobject.h"
#include "QDebug"
#include "qfiledialog.h"
#include "mainwindow.h"

RunConfig::RunConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunConfig)
{
    ui->setupUi(this);

    groupBoxes = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^metapaths_steps*")));

    runAllCheck = this->findChild<QCheckBox *>("runAllCheckBox");
    skipAllCheck = this->findChild<QCheckBox *>("skipAllCheckBox");
    redoAllCheck = this->findChild<QCheckBox *>("redoAllCheckBox");
    fileSelectedPath = this->findChild<QLabel *>("fileBrowsePath");
    cancelButton = this->findChild<QPushButton *>("cancelButton");
    runButton = this->findChild<QPushButton *>("runButton");

    fileBrowseButton = this->findChild<QPushButton *>("fileBrowseButton");

    getAllRadioButtons();
    setStyling();
    loadRunParams(MainWindow::PARAMS);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(runAllCheck, SIGNAL(clicked()), this, SLOT(toggleAllRun()));
    connect(skipAllCheck, SIGNAL(clicked()), this, SLOT(toggleAllSkip()));
    connect(redoAllCheck, SIGNAL(clicked()), this, SLOT(toggleAllRedo()));
    connect(fileBrowseButton, SIGNAL(clicked()), this, SLOT(browseFile()));
}

void RunConfig::getAllRadioButtons(){

}

void RunConfig::loadRunParams(QHash<QString,QString> *params){

}

void RunConfig::browseFile(){
    selectedFile = QFileDialog::getOpenFileName(this, tr("Select file run."),"/");
    fileSelectedPath->setText(selectedFile);
}

void RunConfig::toggleAllRun(){

}

void RunConfig::toggleAllSkip(){

}

void RunConfig::toggleAllRedo(){

}

void RunConfig::closeWindow(){
    close();
}

void RunConfig::setStyling(){
    QList<QWidget *>::iterator i;
    for (i = groupBoxes->begin(); i != groupBoxes->end(); ++i){
        QWidget *widget = *i;
        QGroupBox *temp = qobject_cast<QGroupBox *>(widget);
        if (temp!=NULL){
            temp->setStyleSheet("border:0;");
            temp->setTitle("");
        }
    }
}

RunConfig::~RunConfig()
{
    delete ui;
}
