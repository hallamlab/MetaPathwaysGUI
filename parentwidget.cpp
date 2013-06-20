#include "parentwidget.h"
#include "ui_parentwidget.h"
#include "utilities.h"
#include "mainwindow.h"

ParentWidget::ParentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParentWidget)
{
    ui->setupUi(this);

    settingsTab = new SettingsTab();
    runConfigTab = new RunConfig();
    resultTab = new ResultWidget();

    continueButton = this->findChild<QPushButton *>("continueButton");
    cancelButton = this->findChild<QPushButton *>("cancelButton");
    backButton = this->findChild<QPushButton *>("backButton");

    this->setFixedSize(569, 891);

    tab = ui->parentTabWidget;
    tab->removeTab(0);
    tab->removeTab(0);

    tab->addTab(settingsTab,"Run Parameters");
    tab->addTab(runConfigTab,"Run Stages");
    tab->addTab(resultTab,"Run Results");

    tab->setTabEnabled(1,false);
    tab->setTabEnabled(2,false);
    backButton->hide();

    connect(continueButton, SIGNAL(clicked()), this, SLOT(continueButtonPressed()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));
    connect(backButton, SIGNAL(clicked()), this, SLOT(backButtonPressed()));
    connect(tab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged()));
}

void ParentWidget::tabChanged(){
    if (tab->currentWidget() == runConfigTab || tab->currentWidget() == resultTab){
        backButton->show();
    }
    else backButton->hide();
}

void ParentWidget::backButtonPressed(){
    if(tab->currentWidget() == runConfigTab){
        tab->setCurrentWidget(settingsTab);
    }
    else if (tab->currentWidget() == resultTab){
        tab->setCurrentWidget(runConfigTab);
    }
}

void ParentWidget::continueButtonPressed(){
    if(tab->currentWidget() == settingsTab){
        tab->setCurrentWidget(runConfigTab);
        tab->setTabEnabled(1,true);
    }
    else{
        QList<QGroupBox *> *groupBoxes = runConfigTab->groupBoxes;

        QList<QGroupBox *>::iterator i;
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

    //        //write to hash the changes
            if (yesRadioButton->isChecked()) MainWindow::PARAMS->operator [](configKey) = "yes";
            else if (redoRadioButton->isChecked()) MainWindow::PARAMS->operator [](configKey) = "redo";
            else MainWindow::PARAMS->operator [](configKey) = "skip";

            //write to file the changes
            Utilities::writeSettingToFile(MainWindow::TEMPLATE_PARAM, configKey, MainWindow::PARAMS->operator [](configKey));
        }

    }
}

void ParentWidget::cancelButtonPressed(){
    close();
}

ParentWidget::~ParentWidget()
{
    delete ui;
}
