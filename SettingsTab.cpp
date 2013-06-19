#include "SettingsTab.h"
#include "ui_SettingsTab.h"
#include "qobject.h"
#include "qdebug"
#include "qstring.h"
#include "mainwindow.h"


SettingsTab::SettingsTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsTab)
{
    ui->setupUi(this);

    allWidgets = new QList<QWidget *>();

    RunConfigWindow = NULL;
    cancelButton = this->findChild<QPushButton *>("cancelButton");
    continueButton = this->findChild<QPushButton *>("continueButton");

    getAllWidgets();
    initWidgetValues();

    connect(continueButton, SIGNAL(clicked()), this, SLOT(openParameterSetup()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
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
    for (i = allWidgets->begin();i != allWidgets->end(); ++i){
        QWidget *widget = *i;

        QString objectName = widget->objectName();
        QString configName = MainWindow::CONFIG_MAPPING->key(objectName);
        QString value = MainWindow::PARAMS->value(configName);

        if (qobject_cast<QComboBox *>(widget)!=NULL){
            QComboBox *temp = qobject_cast<QComboBox *>(widget);
            temp->setCurrentIndex(temp->findText(value));
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

    QList<QWidget *>::iterator i;

    for (i = qcWidgets->begin();i != qcWidgets->end(); ++i){
        QWidget *widget = *i;
        allWidgets->append(widget);
    }
    for (i = orfWidgets->begin();i != orfWidgets->end(); ++i){
        QWidget *widget = *i;
        allWidgets->append(widget);
    }
    for (i = annotationWidgets->begin();i != annotationWidgets->end(); ++i){
        QWidget *widget = *i;
        allWidgets->append(widget);
    }
    for (i = rrnaWidgets->begin();i != rrnaWidgets->end(); ++i){
        QWidget *widget = *i;
        allWidgets->append(widget);
    }
}

SettingsTab::~SettingsTab()
{
    delete ui;
}
