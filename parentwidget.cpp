#include "parentwidget.h"
#include "ui_parentwidget.h"
#include "utilities.h"
#include "ProgressDialog.h"
#include "resultwindow.h"
#include "resultpage.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QDebug>
#include <QProcess>
#include <QDockWidget>
#include <QDebug>

ParentWidget::ParentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParentWidget)
{
    ui->setupUi(this);

    continueButton = this->findChild<QPushButton *>("continueButton");
    cancelButton = this->findChild<QPushButton *>("cancelButton");
    backButton = this->findChild<QPushButton *>("backButton");

    continueButton->setEnabled(false);
    backButton->hide();


    settingsTab = new SettingsTab(this);
    runConfigTab = new RunConfig(this);

    tab = ui->parentTabWidget;
    tab->clear();
    //delete initial
    QScrollArea *settings = new QScrollArea();
    QScrollArea *run = new QScrollArea();


    settings->setWidget(settingsTab);
    run->setWidget(runConfigTab);

    tab->addTab(settings,"Run Parameters");
    tab->addTab(run,"Run Stages");

    connect(continueButton, SIGNAL(clicked()), this, SLOT(continueButtonPressed()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));
    connect(backButton, SIGNAL(clicked()), this, SLOT(backButtonPressed()));
    connect(tab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged()));
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    qDebug() << "size " << this->size();

}

void ParentWidget::hideContinueButton(){
    continueButton->setDisabled(true);
}

void ParentWidget::enableContinueButton(){
    continueButton->setEnabled(true);
}

void ParentWidget::tabChanged(){
    if (tab->currentWidget() == runConfigTab){
        backButton->show();
    }else{
        backButton->hide();
    }
    if (tab->currentWidget() == runConfigTab && runConfigTab->selectedFiles == 0){
        continueButton->setEnabled(false);
    }else{
        continueButton->setEnabled(true);
    }
}

void ParentWidget::backButtonPressed(){
    if(tab->currentWidget() == runConfigTab){
        tab->setCurrentWidget(settingsTab);
    }
}

/*
void ParentWidget::continueButtonPressed(){

    RunData *rundata = RunData::getRunData();

    qDebug() << "continue button pressed ";
    qDebug() << "current widget " << tab->currentWidget();

    if(tab->currentWidget() == settingsTab){

        qDebug() << "settings tabe";
        tab->setCurrentWidget(runConfigTab);
        tab->setTabEnabled(1,true);
        qDebug() << " before keys";
  //      qDebug() << "keys " << MainFrame::CONFIG_MAPPING.keys();
        //write SettingsTab changes to file
        QList<QWidget *>::iterator i;
        QList<QWidget *> *allWidgets = SettingsTab::allWidgets;
        for (i = allWidgets->begin(); i != allWidgets->end(); ++i){
            QWidget *widget = *i;

            QString objectName = widget->objectName();
            QString configName = rundata->getConfigMapping().key(objectName);
            QString value;

            if (qobject_cast<QComboBox *>(widget)){
                QComboBox *temp = qobject_cast<QComboBox *>(widget);
                value = temp->currentText();
            }
            else if (qobject_cast<QSpinBox *>(widget)){
                QSpinBox *temp = qobject_cast<QSpinBox *>(widget);
                value = QString::number(temp->value());
            }
            else if (qobject_cast<QDoubleSpinBox *>(widget)){
                QDoubleSpinBox *temp = qobject_cast<QDoubleSpinBox *>(widget);
                value = QString::number(temp->value());
            }
            else if (qobject_cast<QTextEdit *>(widget)){
                QTextEdit *temp = qobject_cast<QTextEdit *>(widget);
                value = temp->toPlainText();
            }
            else if (qobject_cast<QListWidget *>(widget)){
                QListWidget *temp = qobject_cast<QListWidget *>(widget);
                for (int i=0;i<temp->count();i++){
                    if (temp->item(i)->checkState() == Qt::Checked){
                        if (temp->objectName() == "annotationDBS"){
                            value = MainFrame::PARAMS[configName];
                            if (value.isEmpty()) value = temp->item(i)->text();
                            else value = value + "," + temp->item(i)->text();
                            MainFrame::PARAMS[configName] = value;
                        }

                        else if(temp->objectName() == "rrnaREFDBS"){
                            value = MainFrame::PARAMS[configName];
                            if (value.isEmpty()) value = temp->item(i)->text();
                            else value = value + "," + temp->item(i)->text();
                            MainFrame::PARAMS[configName] = value;
                        }
                    }
                }
            }
            MainFrame::PARAMS[configName] = value;
            Utilities::writeSettingToFile(MainFrame::TEMPLATE_PARAM, configName, value, false, false);
        }
    }
    else{
                qDebug() << "non settings tabe";
        QList<QGroupBox *> *groupBoxes = runConfigTab->groupBoxes;

        QList<QGroupBox *>::iterator i;
        //write radio button changes to file
        for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){
            qDebug() << "group boxes " << groupBoxes->size();
            QGroupBox *temp = *i;
            QString stepName = temp->objectName();
            //get the name, look up in the hash the corresponding setting key value pair
  //          qDebug() <<  "stepName " << MainFrame::CONFIG_MAPPING;
            QString configKey = rundata->getConfigMapping()[stepName];

            qDebug() << "config key " << configKey;
            //get the name of the radiobutton on the form by isolating for caps from the step,
            //taking the step name, to lower
            QRegExp lowerCaseRegex("[a-z]+_?[a-z]+");
            QString radioButtonName = stepName.remove(lowerCaseRegex).toLower();

            QRadioButton *yesRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"YES");
            QRadioButton *redoRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"REDO");
            QRadioButton *skipRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"SKIP");

            //write to hash the changes
            if (yesRadioButton->isChecked()) rundata->setValue(configKey, "yes", _PARAMS);
            else if (redoRadioButton->isChecked()) rundata->setValue(configKey, "redo", _PARAMS);
            else rundata->setValue(configKey, "skip", _PARAMS);

            //write to file the changes
            Utilities::writeSettingToFile(RunData::TEMPLATE_PARAM, configKey, rundata->getValueFromHash(configKey, _PARAMS), false, false);

            //disable all buttons now for the run
            yesRadioButton->setEnabled(false);
            redoRadioButton->setEnabled(false);
            skipRadioButton->setEnabled(false);
        }

        //set all applicable buttons to be disabled
        runConfigTab->runOptionsGroupBox->setEnabled(false);
        runConfigTab->fileBrowseButton->setEnabled(false);
        runConfigTab->folderBrowseButton->setEnabled(false);
        runConfigTab->fileInputFormat->setEnabled(false);
        continueButton->setEnabled(false);
        backButton->setEnabled(false);
        cancelButton->setEnabled(false);

        //write file format
        QString inputTypeKey = rundata->getConfigMapping()[runConfigTab->fileInputFormat->objectName()];
        Utilities::writeSettingToFile(RunData::TEMPLATE_PARAM, inputTypeKey, runConfigTab->fileInputFormat->currentText(), false, false);
        rundata->setValue(inputTypeKey, = runConfigTab->fileInputFormat->currentText();

        //save file selected
        MainFrame::PARAMS["fileInput"] = runConfigTab->filesSelected->text();
        MainFrame::PARAMS["folderOutput"] = runConfigTab->folderSelected->text();

        //override grid choice - if the user chose redo or yes with this ticked, then the step param should be "grid"
        if (runConfigTab->gridBlastChoice->isChecked() && MainFrame::PARAMS.value("metapaths_steps:BLAST_REFDB")!="skip"){
            MainFrame::PARAMS["metapaths_steps:BLAST_REFDB"] = "grid";
            Utilities::writeSettingToFile(MainFrame::TEMPLATE_PARAM, "metapaths_steps:BLAST_REFDB", "grid", false, false);
        }

        //trim off commas on refdbs for rRNA and annotation
        QString rRNArefdbs = MainFrame::PARAMS["rRNA:refdbs"];
        qDebug() << rRNArefdbs;
        rRNArefdbs = rRNArefdbs.remove(QRegExp("[\\s,]*$"));
        MainFrame::PARAMS["rRNA:refdbs"] = rRNArefdbs;

        QString annotationDBS = MainFrame::PARAMS["annotation:dbs"];

        annotationDBS = annotationDBS.remove(QRegExp("[\\s,]*$"));

        MainFrame::PARAMS["annotationDBS"] = annotationDBS;

        Utilities::writeSettingToFile(MainFrame::TEMPLATE_PARAM, "rRNA:refdbs",rRNArefdbs, false,false);
        Utilities::writeSettingToFile(MainFrame::TEMPLATE_PARAM, "annotation:dbs",annotationDBS, false,false);

        //use overwrite settings?
        if (runConfigTab->overwrite->isChecked()){
            MainFrame::PARAMS["overwrite"] = "overwrite";
        }else MainFrame::PARAMS["overwrite"] = "overlay";

    }        executionPrep();

}
*/

/*
void ParentWidget::executionPrep(){
    //copy the current CONFIG, PARAMS pointers to a newly instantiated RunData instance
    run = RunData::getRunData();


    //set the rrnaDB and annotationDBS so we know how many to look for in the log
    QStringList* rrnaDBS;
    QString rdbs = MainFrame::PARAMS["rRNA:refdbs"];
    rrnaDBS = new QStringList(rdbs.split(","));
    rrnaDBS->removeAll("");

    QStringList* annotationDBS;
    QString adbs = MainFrame::PARAMS["annotation:dbs"];
    annotationDBS = new QStringList(adbs.split(","));
    annotationDBS->removeAll("");

    QStringList *uniqueRRNADBS = new QStringList();
    QStringList *uniqueADBS = new QStringList();

    Utilities::getUniqueDBS(*rrnaDBS, uniqueRRNADBS);
    Utilities::getUniqueDBS(*annotationDBS, uniqueADBS);

    rrnaDBS = uniqueRRNADBS;
    annotationDBS = uniqueADBS;

    run->nADB = annotationDBS->size();
    run->nRRNADB = rrnaDBS->size();

    run->setAnnotationDBS(annotationDBS);
    run->setRRNADBS(rrnaDBS);

    emit continueFromParentSettings();
    //mit showResultsFromParentSettings();

//    ProgressDialog *progress = new ProgressDialog(this);
//    ResultWindow *rw = new ResultWindow(progress);

    //reset MainFrame static instances
//    MainFrame::CONFIG = Utilities::parseFile(MainFrame::DEFAULT_TEMPLATE_CONFIG);
//    MainFrame::PARAMS = Utilities::parseFile(MainFrame::DEFAULT_TEMPLATE_PARAM);
}
*/

void ParentWidget::cancelButtonPressed(){
//    close();
//    this->parentWidget()->close();
}

ParentWidget::~ParentWidget()
{
    delete ui;
}
