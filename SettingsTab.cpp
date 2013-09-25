#include "SettingsTab.h"
#include "ui_SettingsTab.h"
#include "mainwindow.h"
#include <QObject>
#include <QDebug>
#include <QString>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>

QList<QWidget *> *SettingsTab::allWidgets = NULL;

SettingsTab::SettingsTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsTab)
{
    ui->setupUi(this);
    SettingsTab::allWidgets = new QList<QWidget *>();

    RunConfigWindow = NULL;

    getAllWidgets();
    initWidgetValues();

    annotationDBSButton = this->findChild<QPushButton *>("annotationDBSButton");
    annotationDBSWarning = this->findChild<QLabel *>("annotationDBSWarning");
    rrnaREFDBSButton = this->findChild<QPushButton *>("rrnaREFDBSButton");
    annotationDBS = this->findChild<QTextEdit *>("annotationDBS");
    rrnaREFDBS = this->findChild<QTextEdit *>("rrnaREFDBS");
    rrnaREFDBSWarning = this->findChild<QLabel *>("rrnaREFDBSWarning");

    annotationFileList = new QString("");
    rrnaFileList = new QString("");

    connect(this, SIGNAL(setContinueButton()), this->parent(), SLOT(enableContinueButton()));

    if (!MainWindow::PARAMS->value("annotation:dbs").isEmpty()){
        annotationDBSWarning->hide();
    }
    if (!MainWindow::PARAMS->value("rRNA:refdbs").isEmpty()){
        rrnaREFDBSWarning->hide();
    }
    if (annotationDBSWarning->isHidden() && rrnaREFDBSWarning->isHidden()){
        emit setContinueButton();
    }

    setStyling();

    connect(annotationDBSButton, SIGNAL(clicked()), this, SLOT(annotationDBSPressed()));
    connect(rrnaREFDBSButton, SIGNAL(clicked()), this, SLOT(rrnaREFDBSPressed()));
}

void SettingsTab::setStyling(){
    rrnaREFDBSWarning->setStyleSheet("color:red");
    annotationDBSWarning->setStyleSheet("color:red");
}

void SettingsTab::annotationDBSPressed(){
    annotationFiles = new QStringList(QFileDialog::getOpenFileNames(this, tr("Select files to use as databases."), MainWindow::CONFIG->operator []("REFDBS")));
    for (int i=0;i < annotationFiles->size(); i++){
        QString file = annotationFiles->at(i).split("/").last();
        annotationFileList->append(file);
        annotationFileList->append(",");
    }
    annotationDBS->setText(*annotationFileList);
    if (annotationFileList->length() > 0){
        annotationDBSWarning->hide();
    }
    if (annotationFileList->length() > 0 && rrnaFileList->length() > 0){
        emit setContinueButton();
    }
}

void SettingsTab::rrnaREFDBSPressed(){
    rrnaFiles = new QStringList(QFileDialog::getOpenFileNames(this, tr("Select files to use as databases."), MainWindow::CONFIG->operator []("REFDBS")));
    for (int i=0;i < rrnaFiles->size(); i++){
        QString file = rrnaFiles->at(i).split("/").last();
        rrnaFileList->append(file);
        rrnaFileList->append(",");
    }
    rrnaREFDBS->setText(*rrnaFileList);
    if (rrnaFileList->length() > 0){
        rrnaREFDBSWarning->hide();
    }
    if (annotationFileList->length() > 0 && rrnaFileList->length() > 0){
        emit setContinueButton();
    }
}

void SettingsTab::closeWindow(){
    close();
}

/*
 * Save params to file.
 */
void SettingsTab::openParameterSetup(){
    if (RunConfigWindow == NULL) RunConfigWindow = new RunConfig();
    RunConfigWindow->show();
    close();
}

void SettingsTab::initWidgetValues(){
    QList<QWidget *>::iterator i;
    for (i = SettingsTab::allWidgets->begin();i != SettingsTab::allWidgets->end(); ++i){
        QWidget *widget = *i;

        QString objectName = widget->objectName();
        QString configName = MainWindow::CONFIG_MAPPING->key(objectName);
        QString value = MainWindow::PARAMS->value(configName);



        if (qobject_cast<QComboBox *>(widget)!=NULL){
            QComboBox *temp = qobject_cast<QComboBox *>(widget);
            temp->setCurrentIndex(temp->findText(value));
            if (temp->currentIndex()==-1){
                temp->addItem(value);
            }
        }
        else if (qobject_cast<QSpinBox *>(widget)!=NULL){
            QSpinBox *temp = qobject_cast<QSpinBox *>(widget);
            temp->setValue(value.toInt());
        }
        else if (qobject_cast<QDoubleSpinBox *>(widget)!=NULL){
            QDoubleSpinBox *temp = qobject_cast<QDoubleSpinBox *>(widget);
            temp->setValue(value.toInt());
        }
        else if (qobject_cast<QTextEdit *>(widget)!=NULL){
            QTextEdit *temp = qobject_cast<QTextEdit *>(widget);
            temp->setText(value);
        }
    }
}

void SettingsTab::getAllWidgets(){
    qcWidgets = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^quality_control*")));
    orfWidgets = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^orf_prediction*")));
    annotationWidgets = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^annotation*")));
    rrnaWidgets = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^rrna*")));
    pathwaysWidgets = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^ptools_settings*")));
    QList<QWidget *>::iterator i;

    for (i = qcWidgets->begin();i != qcWidgets->end(); ++i){
        QWidget *widget = *i;
        SettingsTab::allWidgets->append(widget);
    }
    for (i = orfWidgets->begin();i != orfWidgets->end(); ++i){
        QWidget *widget = *i;
        SettingsTab::allWidgets->append(widget);
    }
    for (i = annotationWidgets->begin();i != annotationWidgets->end(); ++i){
        QWidget *widget = *i;
        SettingsTab::allWidgets->append(widget);
    }
    for (i = rrnaWidgets->begin();i != rrnaWidgets->end(); ++i){
        QWidget *widget = *i;
        SettingsTab::allWidgets->append(widget);
    }
    for (i = pathwaysWidgets->begin(); i != pathwaysWidgets->end(); ++i){
        QWidget *widget = *i;
        SettingsTab::allWidgets->append(widget);
    }
}

SettingsTab::~SettingsTab()
{
    delete ui;
}
