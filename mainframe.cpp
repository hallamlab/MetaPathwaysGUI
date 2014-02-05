#include "mainframe.h"
#include "ui_mainframe.h"
#include <fstream>
#include <iostream>
#include <QSettings>
#include <QStylePainter>

MainFrame::MainFrame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainFrame)
{
    ui->setupUi(this);

    centralWidget = this->findChild<QWidget *>("centralWidget");
    actionSetup = this->findChild<QAction *>("actionSetup");
    actionRunSettings = this->findChild<QAction *>("actionRunSettings");
    actionProgress = this->findChild<QAction *>("actionRun");
    //actionGridProgress = this->findChild<QAction *>("actionGridProgress");
    actionResults = this->findChild<QAction *>("actionResults");
    actionRunParams = this->findChild<QAction *>("actionRunParams");
    actionRunStages = this->findChild<QAction *>("actionStages");

    toolBar = this->findChild<QToolBar *>("toolBar");
//    leftToolBar = this->findChild<QToolBar *>("leftToolBar");

//    setupLeftToolBar();

    stackedWidget = this->findChild<QStackedWidget *>("stackedWidget");
    actionSetupMenu = this->findChild<QAction *>("actionSetupMenu");
    actionAbout = this->findChild<QAction *>("actionAbout");

//    ToolBarManager *toolbarManager = ToolBarManager::getToolBarManager();
//    toolbarManager->setToolBar(leftToolBar);
//    leftToolBar->setStyleSheet("QToolBar{spacing: 2px;}");

    setupWidget = 0;
    parentWidget = 0;
    resultWindow = 0;
    welcomeWindow = 0;
    stages = 0;
    settings = 0;
    gridProgress = 0;

    this->setWindowTitle("MetaPathways 2.0");
    welcomeWindow = new Welcome();
    stackedWidget->addWidget(welcomeWindow);

    rundata = RunData::getRunData();
    rundata->setConfigMapping(Utilities::createMapping());

//    QSettings settings("HallamLab", "MetaPathways");
//    settings.clear();

    setupWidget = new Setup();
    setupWidget->loadPathVariables();

    rundata->setupDefaultConfig();
    rundata->setupDefaultParams();

    // databases are agnostic to what the param file has written from a previous run
    // so we clear the values in the hash picked up from the config file to ensure
    // the user has to always specify a new set to go with
    rundata->setValue("rRNA:refdbs","",_PARAMS);
    rundata->setValue("annotation:dbs","",_PARAMS);

    QString warningStr;
    stackedWidget->addWidget(setupWidget);
    connect(setupWidget, SIGNAL(continueFromSetup()), this, SLOT(validateSetup()));

    if( !rundata->validate(warningStr) ) {
       this->showSetupError(warningStr);
    }else{
        validateSetup();
    }

    // auto update code
    // run a separate process to check if there is a new file

    WidgetStacker *wStacker = WidgetStacker::getWidgetStacker();
    wStacker->setReferenceCoordinate(this->pos());
}

//void MainFrame::setup'LeftToolBar'(){
//    leftToolBar->setAllowedAreas(Qt::LeftToolBarArea);
//    leftToolBar->setGeometry(0, toolBar->height(),4,10);
//    leftToolBar->setLayoutDirection(Qt::LeftToRight);
//    leftToolBar->setOrientation(Qt::Vertical);
//    leftToolBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    leftToolBar->setMinimumSize(QSize(0,0));
//    leftToolBar->setBaseSize(QSize(0,0));
//    leftToolBar->setSizeIncrement(0,0);
//    leftToolBar->setFloatable(true);
//    leftToolBar->setMovable(true);
//    leftToolBar->setAutoFillBackground(false);
//    leftToolBar->setParent(this);
//}

void MainFrame::showSetupError(QString warningStr) {

    warning->warning(0,"Configuration Invalid!\n", QString("You are missing the following:\n") + warningStr + "Please check Setup!.", QMessageBox::Ok);
    greyTabs(false);
    this->openSetup();

}

void MainFrame::validateSetup() {

     RunData *rundata= RunData::getRunData();

     QString warningStr;
     if( rundata->validate(warningStr)) {
         //pass
         addRemainingTabs();
         stackedWidget->setCurrentWidget(settingsScroll);
         greyTabs(true);
     }
     else {
         //fails
         showSetupError(warningStr);
         greyTabs(false);
     }
}

void MainFrame::greyTabs(bool enabled){
    actionProgress->setEnabled(enabled);
//    actionGridProgress->setEnabled(enabled);
    actionResults->setEnabled(enabled);
    actionRunParams->setEnabled(enabled);
    actionRunStages->setEnabled(enabled);
}

void MainFrame::addRemainingTabs() {
    stages = new RunConfig();
    settings = new SettingsTab();
    resultWindow = ResultWindow::getResultWindow();
    progress = new ProgressDialog();

    stageScroll = new QScrollArea();
    settingsScroll = new QScrollArea();
    gridProgress = new GridProgress();

    stageScroll->setWidget(stages);
    settingsScroll->setWidget(settings);
    stackedWidget->addWidget(stageScroll);
    stackedWidget->addWidget(settingsScroll);
    stackedWidget->addWidget(resultWindow);
    stackedWidget->addWidget(progress);
    stackedWidget->addWidget(gridProgress);

    stackedWidget->setCurrentWidget(welcomeWindow);

    connect(actionSetup, SIGNAL(triggered()), this, SLOT(openSetup()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(displayWelcome()));
    connect(actionSetupMenu, SIGNAL(triggered()), this, SLOT(openSetup()));
    connect(actionProgress, SIGNAL(triggered()), this, SLOT(displayProgress()));
    connect(actionResults, SIGNAL(triggered()), this, SLOT(displayResults()));
    connect(actionRunStages, SIGNAL(triggered()), this, SLOT(displayStages()));
    connect(actionRunParams, SIGNAL(triggered()), this, SLOT(displayParams()));
//    connect(actionGridProgress, SIGNAL(triggered()), this, SLOT(displayGridProgress()));
}

void MainFrame::displayGridProgress(){
    this->updateWidgets();
    stackedWidget->setCurrentWidget(gridProgress);
}

void MainFrame::displayStages(){
    this->updateWidgets();
    stackedWidget->setCurrentWidget(stageScroll);
}

void MainFrame::displayParams(){
    this->updateWidgets();
    stackedWidget->setCurrentWidget(settingsScroll);
}

void MainFrame::updateWidgets(){
    if(stackedWidget->currentWidget() == settingsScroll){
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
                QHash<QString,QString> adbs;
                QHash<QString,QString> rdbs;
                //use the power of hashes to only get unique strings

                adbs.clear();
                rdbs.clear();

                for (int i=0;i<temp->count();i++){
                    if (temp->item(i)->checkState() == Qt::Checked){
                        if (temp->objectName() == "annotationDBS"){
                            adbs[temp->item(i)->text()] = "";
                        }
                        else if(temp->objectName() == "rrnaREFDBS"){
                            rdbs[temp->item(i)->text()] = "";
                        }
                    }
                }
                if (temp->objectName()=="annotationDBS"){
                    QStringList rt(adbs.keys());
                    value = rt.join(",");
                }
                if (temp->objectName()=="rrnaREFDBS"){
                    QStringList rt(rdbs.keys());
                    value = rt.join(",");
                }
                //qDebug() << "keys " <<  adbs.keys() << rdbs.keys() << " value " << value << " cfname " << configName;
            }
            //qDebug() << " Setting the value " << configName << " val = " << value;
            rundata->setValue(configName, value,_PARAMS);
            Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] + "/" + RunData::TEMPLATE_PARAM, configName, value, false, false);
        }
    }
    else if(stackedWidget->currentWidget()==stageScroll){
        QList<QGroupBox *> *groupBoxes = stages->groupBoxes;

        QList<QGroupBox *>::iterator i;
        //write radio button changes to file
        for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){

            QGroupBox *temp = *i;
            QString stepName = temp->objectName();
            //get the name, look up in the hash the corresponding setting key value pair
          //  qDebug() <<  "stepName " << MainFrame::CONFIG_MAPPING;
            QString configKey = rundata->getConfigMapping().key(stepName);

            //get the name of the radiobutton on the form by isolating for caps from the step,
            //taking the step name, to lower
            QRegExp lowerCaseRegex("[a-z]+_?[a-z]+");
            QString radioButtonName = stepName.remove(lowerCaseRegex).toLower();

            QRadioButton *yesRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"YES");
            QRadioButton *redoRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"REDO");
            QRadioButton *skipRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"SKIP");

            //write to hash the changes
            if (yesRadioButton->isChecked()) rundata->setValue(configKey,"yes",_PARAMS);

            else if (redoRadioButton->isChecked()) rundata->setValue(configKey,"redo",_PARAMS);
            else rundata->setValue(configKey,"skip",_PARAMS);

            //write to file the changes
            Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] + "/" + RunData::TEMPLATE_PARAM, configKey, rundata->getValueFromHash(configKey,_PARAMS), false, false);

//            //disable all buttons now for the run
//            yesRadioButton->setEnabled(false);
//            redoRadioButton->setEnabled(false);
//            skipRadioButton->setEnabled(false);
        }

        //write file format
        QString inputTypeKey = rundata->getConfigMapping().key(stages->fileInputFormat->objectName());
        Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] + "/" + RunData::TEMPLATE_PARAM, inputTypeKey, stages->fileInputFormat->currentText(), false, false);
        rundata->setValue(inputTypeKey,stages->fileInputFormat->currentText(),_PARAMS);

        //save file selected
        rundata->setValue("fileInput", stages->selectedFiles,_PARAMS);
        rundata->setValue("folderOutput", stages->selectedFolder,_PARAMS);

        //override grid choice - if the user chose redo or yes with this ticked, then the step param should be "grid"
//        if (stages->gridBlastChoice->isChecked() && MainFrame::PARAMS.value("metapaths_steps:BLAST_REFDB")!="skip"){
//            MainFrame::PARAMS["metapaths_steps:BLAST_REFDB"] = "grid";
//            Utilities::writeSettingToFile(MainFrame::TEMPLATE_PARAM, "metapaths_steps:BLAST_REFDB", "grid", false, false);
//        }

        //trim off commas on refdbs for rRNA and annotation
        QString rRNArefdbs = rundata->getValueFromHash("rRNA:refdbs", _PARAMS);

        rRNArefdbs = rRNArefdbs.remove(QRegExp("[\\s,]*$"));
        rundata->setValue("rRNA:refdbs", rRNArefdbs, _PARAMS); //trim ending comma
        QString annotationDBS = rundata->getValueFromHash("annotation:dbs", _PARAMS);
        annotationDBS = annotationDBS.remove(QRegExp("[\\s,]*$"));
        rundata->setValue("annotationDBS", annotationDBS, _PARAMS);

        //number of each
        rundata->setNumADB(annotationDBS.size());
        rundata->setNumRRNADB(rRNArefdbs.size());

        Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] + "/" + RunData::TEMPLATE_PARAM, "rRNA:refdbs",rRNArefdbs, false,false);
        Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] + "/" + RunData::TEMPLATE_PARAM, "annotation:dbs",annotationDBS, false,false);

        //use overwrite settings?
        if (stages->overwrite->isChecked()){
            rundata->setValue("overwrite", "overwrite", _PARAMS);
        }else rundata->setValue("overwrite", "overlay", _PARAMS);

        executionPrep();
    }
}

void MainFrame::executionPrep(){
    //copy the current CONFIG, PARAMS pointers to a newly instantiated RunData instance

    //set the rrnaDB and annotationDBS so we know how many to look for in the log
    QStringList rrnaDBS;
    QString rdbs = rundata->getValueFromHash("rRNA:refdbs", _PARAMS);
    rrnaDBS = rdbs.split(",");
    rrnaDBS.removeAll("");


    QString adbs = rundata->getValueFromHash("annotation:dbs", _PARAMS);

    QStringList annotationDBS = adbs.split(",");
    annotationDBS.removeAll("");

    rrnaDBS  = Utilities::getUniqueDBS(rrnaDBS);
    annotationDBS = Utilities::getUniqueDBS(annotationDBS);


    if( annotationDBS.size() > 0) {
        rundata->setNumADB(annotationDBS.size());
        rundata->setNumRRNADB(rrnaDBS.size());
        rundata->setAnnotationDBS(annotationDBS);
        rundata->setRRNADBS(rrnaDBS);
    }

}

void MainFrame::displayProgress(){
    this->updateWidgets();
    stackedWidget->setCurrentWidget(progress);
}

void MainFrame::displayWelcome(){
    stackedWidget->setCurrentWidget(welcomeWindow);
    updateWidgets();
}

void MainFrame::openSetup(){
    this->updateWidgets();
    stackedWidget->setCurrentWidget(setupWidget);
}


void MainFrame::displayResults(){
    this->updateWidgets();
    stackedWidget->setCurrentWidget(resultWindow);
}


MainFrame::~MainFrame()
{
    delete ui;
}
