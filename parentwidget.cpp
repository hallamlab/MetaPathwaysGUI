#include "parentwidget.h"
#include "ui_parentwidget.h"
#include "utilities.h"
#include "mainwindow.h"
#include "ProgressDialog.h"
#include "tabfactory.h"
#include "resultwindow.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QDebug>
#include <QProcess>
#include <QDockWidget>

ParentWidget::ParentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParentWidget)
{
    ui->setupUi(this);

    settingsTab = new SettingsTab(this);
    runConfigTab = new RunConfig(this);

    continueButton = this->findChild<QPushButton *>("continueButton");
    cancelButton = this->findChild<QPushButton *>("cancelButton");
    backButton = this->findChild<QPushButton *>("backButton");
    sampleWarning = this->findChild<QLabel *>("sampleWarning");

    tab = ui->parentTabWidget;

    //delete initial
    tab->removeTab(0);
    tab->removeTab(0);

    tab->addTab(settingsTab,"Run Parameters");
    tab->addTab(runConfigTab,"Run Stages");

    backButton->hide();

    connect(continueButton, SIGNAL(clicked()), this, SLOT(continueButtonPressed()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));
    connect(backButton, SIGNAL(clicked()), this, SLOT(backButtonPressed()));
    connect(tab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged()));
}

void ParentWidget::enableContinueButton(){
    continueButton->setEnabled(true);
    sampleWarning->hide();
}

void ParentWidget::tabChanged(){
    if (tab->currentWidget() == runConfigTab){
        backButton->show();
    }else backButton->hide();
    if (tab->currentWidget() == runConfigTab && runConfigTab->selectedFiles==0){
        continueButton->setEnabled(false);
        sampleWarning->show();
    }else{
        sampleWarning->hide();
        continueButton->setEnabled(true);
    }
}

void ParentWidget::backButtonPressed(){
    if(tab->currentWidget() == runConfigTab){
        tab->setCurrentWidget(settingsTab);
    }
}

void ParentWidget::continueButtonPressed(){
    if(tab->currentWidget() == settingsTab){

        tab->setCurrentWidget(runConfigTab);
        tab->setTabEnabled(1,true);

        //write SettingsTab changes to file
        QList<QWidget *>::iterator i;
        QList<QWidget *> *allWidgets = SettingsTab::allWidgets;
        for (i = allWidgets->begin(); i != allWidgets->end(); ++i){
            QWidget *widget = *i;

            QString objectName = widget->objectName();
            QString configName = MainWindow::CONFIG_MAPPING->key(objectName);
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
            Utilities::writeSettingToFile(MainWindow::TEMPLATE_PARAM, configName, value);
        }
    }
    else{
        QList<QGroupBox *> *groupBoxes = runConfigTab->groupBoxes;

        QList<QGroupBox *>::iterator i;
        //write radio button changes to file
        for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){
            QGroupBox *temp = *i;
            QString stepName = temp->objectName();
            //get the name, look up in the hash the corresponding setting key value pair
            QString configKey = MainWindow::CONFIG_MAPPING->key(stepName);
            QString configValue = MainWindow::PARAMS->value(configKey);

            //get the name of the radiobutton on the form by isolating for caps from the step,
            //taking the step name, to lower
            QRegExp lowerCaseRegex("[a-z]+_?[a-z]+");
            QString radioButtonName = stepName.remove(lowerCaseRegex).toLower();

            QRadioButton *yesRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"YES");
            QRadioButton *redoRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"REDO");
            QRadioButton *skipRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"SKIP");

            //write to hash the changes
            if (yesRadioButton->isChecked()) MainWindow::PARAMS->operator [](configKey) = "yes";
            else if (redoRadioButton->isChecked()) MainWindow::PARAMS->operator [](configKey) = "redo";
            else MainWindow::PARAMS->operator [](configKey) = "skip";

            //write to file the changes
            Utilities::writeSettingToFile(MainWindow::TEMPLATE_PARAM, configKey, MainWindow::PARAMS->operator [](configKey));

            //disable all buttons now for the run
            yesRadioButton->setEnabled(false);
            redoRadioButton->setEnabled(false);
            skipRadioButton->setEnabled(false);
        }

        //set all applicable buttons to be disabled
        runConfigTab->runOptionsGroupBox->setEnabled(false);
        runConfigTab->fileBrowseButton->setEnabled(false);
        runConfigTab->fileInputFormat->setEnabled(false);
        continueButton->setEnabled(false);
        backButton->setEnabled(false);
        cancelButton->setEnabled(false);

        //write file format
        QString inputTypeKey = MainWindow::CONFIG_MAPPING->key(runConfigTab->fileInputFormat->objectName());
        Utilities::writeSettingToFile(MainWindow::TEMPLATE_PARAM, inputTypeKey, runConfigTab->fileInputFormat->currentText());
        MainWindow::PARAMS->operator [](inputTypeKey) = runConfigTab->fileInputFormat->currentText();

        //save file selected
        MainWindow::PARAMS->operator [](runConfigTab->filesSelected->objectName()) = runConfigTab->filesSelected->text();

        executionPrep();
    }
}

void ParentWidget::executionPrep(){
    //copy the current CONFIG, PARAMS pointers to a newly instantiated RunData instance
    run = new RunData(MainWindow::PARAMS,MainWindow::CONFIG,MainWindow::CONFIG_MAPPING);

    //reset MainWindow static instances
    MainWindow::CONFIG = Utilities::parseFile(MainWindow::DEFAULT_TEMPLATE_CONFIG);
    MainWindow::PARAMS = Utilities::parseFile(MainWindow::DEFAULT_TEMPLATE_PARAM);

    ProgressDialog *progress = new ProgressDialog(this, run);
    ResultWindow *rw = new ResultWindow(run);

    rw->show();
    progress->show();

    close();
    this->parentWidget()->close();
}

void ParentWidget::cancelButtonPressed(){
    close();
}

ParentWidget::~ParentWidget()
{
    delete ui;
}
