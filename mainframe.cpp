/*
 * The main encompassing frame of the entire application. This class is responsible for coordinating
 * all the interactions between all activities of config, parameter setup.
 *
 */
#include "mainframe.h"
#include "ui_mainframe.h"

/*
 * Sleep function used for displaying the logo at startup.
 */
void MainFrame::qSleep(int ms)
{
    #ifdef Q_OS_WIN
        Sleep(uint(ms));
    #else
        struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
        nanosleep(&ts, NULL);
    #endif
}

/*
 * Default constructor of MainFrame. Makes important calls to make sure config and param
 * files will exist at startup through a series of function calls.
 */
MainFrame::MainFrame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainFrame)
{
    ui->setupUi(this);

    centralWidget = this->findChild<QWidget *>("centralWidget");
    actionSetup = this->findChild<QAction *>("actionSetup");
    actionRunSettings = this->findChild<QAction *>("actionRunSettings");
    actionProgress = this->findChild<QAction *>("actionRun");
    actionResults = this->findChild<QAction *>("actionResults");
    actionRunParams = this->findChild<QAction *>("actionRunParams");
    actionRunStages = this->findChild<QAction *>("actionStages");

    toolBar = this->findChild<QToolBar *>("toolBar");
    stackedWidget = this->findChild<QStackedWidget *>("stackedWidget");

    setupWidget = 0;
    resultWindow = 0;
    welcomeWindow = 0;
    stages = 0;
    settings = 0;
    gridProgress = 0;

    this->setWindowTitle("MetaPathways 3.0");
    welcomeWindow = new Welcome();
    welcomeWindow->show();
    qSleep(2500);
    welcomeWindow->close();
    delete welcomeWindow;
    // shows the welcome window, closes it after a sleep

    rundata = RunData::getRunData();
    rundata->setConfigMapping(Utilities::createMapping());
    // initialize our singleton of rundata, encompasses a virtualization of the modified
    // configuration file and parameter file; also sets up a mapping required later

    rundata->setupDefaultConfig();

    setupWidget = new Setup();

  //  setupWidget->loadPathVariables();
    // populates the setup form with values previously used, if they exist
    rundata->setupDefaultParams();
    rundata->loadDefaultParams();
    // for the config and param file, check to see if pre-existing config or parameter files
    // are defined and exist. see their function heads for clarification.


    // databases are agnostic to what the param file has written from a previous run
    // so we clear the values in the hash picked up from the config file to ensure
    // the user has to always specify a new set to go with


    // rundata->setValue("rRNA:refdbs","",_PARAMS);
   // rundata->setValue("annotation:dbs","",_PARAMS);



    stackedWidget->addWidget(setupWidget);
    this->addRemainingTabs();
    this->greyTabs(true);
    connect(this, SIGNAL(markConfigInSetup(bool) ), setupWidget, SLOT(validateSetup(bool)));

    connect(rundata, SIGNAL(loadParameters()), settings, SLOT(loadParameters()));
    setupWidget->validateSetup(false, true);

    /*
    if( !rundata->validate(warningStr) ) {
       // validate(string) will check to see if values exist and return  warning string if not
       this->showSetupError(warningStr);
    }else{  */
  //   validateSetup();
    stackedWidget->setCurrentWidget(settingsScroll);
    emit markConfigInSetup(false);

      // connect(rundata, SIGNAL(loadParameters()), settings, SLOT(loadParameters()));
    ///}


}

void MainFrame::showSetupError(QString warningStr) {
    warning->warning(0,"Incomplete Configuration !\n", QString("INCOMPLETE CONFIGURATION MAY DISABLE SOME FEATURES  \n\n") + warningStr + "Please check Setup!.", QMessageBox::Ok);
  //  greyTabs(false);
    this->openSetup();
    setupWidget->updateValues();
}

/*
 * On startup, the user can only select the setup tab if there are fields missing. This validates
 * the config and parameters given and enables them if the user should be able to proceed to
 * further steps.
 */
void MainFrame::validateSetup() {
/*
    RunData *rundata= RunData::getRunData();
     QString warningStr;

     if( !rundata->validate(warningStr)) {
          showSetupError(warningStr);
     }
*/
    addRemainingTabs();
    stackedWidget->setCurrentWidget(settingsScroll);
    //greyTabs(true);

}

/*
 * Enables/Disables actions in the menu.
 */
void MainFrame::greyTabs(bool enabled){
    actionProgress->setEnabled(enabled);
    actionResults->setEnabled(enabled);
    actionRunParams->setEnabled(enabled);
    actionRunStages->setEnabled(enabled);
}

/*
 * Instantiates the rest of the objects associated with each tab and sets up connections.
 */
void MainFrame::addRemainingTabs() {
    stages = new RunConfig();
    settings = new SettingsTab();
    resultWindow = ResultWindow::getResultWindow();
    progress = new ProgressDialog();

    stageScroll = new QScrollArea();
    settingsScroll = new QScrollArea();
    progressScroll = new QScrollArea();

    stageScroll->setWidget(stages);
    settingsScroll->setWidget(settings);
    progressScroll->setWidget(progress);

    stackedWidget->addWidget(stageScroll);
    stackedWidget->addWidget(settingsScroll);
    stackedWidget->addWidget(progressScroll);

    stackedWidget->addWidget(resultWindow);


    connect(actionSetup, SIGNAL(triggered()), this, SLOT(openSetup()));
    connect(actionProgress, SIGNAL(triggered()), this, SLOT(displayProgress()));
    connect(actionResults, SIGNAL(triggered()), this, SLOT(displayResults()));
    connect(actionRunStages, SIGNAL(triggered()), this, SLOT(displayStages()));
    connect(actionRunParams, SIGNAL(triggered()), this, SLOT(displayParams()));

}

void MainFrame::displayGridProgress(){
   // this->updateWidgets();
    stackedWidget->setCurrentWidget(gridProgress);
}

void MainFrame::displayStages(){
  //  this->updateWidgets();
    stackedWidget->setCurrentWidget(stageScroll);
}

void MainFrame::displayParams(){
  //  this->updateWidgets();
    stackedWidget->setCurrentWidget(settingsScroll);
}

/*
 * This function is called everytime the user switches widgets to make sure any changes
 * that need to be written out are done immediately. This is really only applicable to the
 * parameters widget and the stages widget.
 */
void MainFrame::updateWidgets(){
    // if(stackedWidget->currentWidget() == settingsScroll)
    this->setupWidget->saveParamFile();
    {
        // the user was looking at the param setup screen

        QList<QWidget *>::iterator i;
        QList<QWidget *> *allWidgets = SettingsTab::allWidgets;

        if( allWidgets ==0 ) return;

        // go through each widget in the settings tab
        // and cast them appropriately
        // retrieve their values and write them out
       // qDebug() << "num widgets " << allWidgets->size();
        for (i = allWidgets->begin(); i != allWidgets->end(); ++i){
            QWidget *widget = *i;

            QString objectName = widget->objectName();
            QString configName = rundata->getConfigMapping().key(objectName);
            QString value;

           // qDebug() << configName;
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
              // qDebug() << "Before going through the annotation DBs";
               // qDebug() << "The number of databases that are already there "<< temp->count();
             //  qDebug() << "\n\n";
                for (int i=0;i<temp->count();i++){
                    if (temp->item(i)->checkState() == Qt::Checked){
                        if (temp->objectName() == "annotationDBS"){
                            adbs[temp->item(i)->text()] = "";
                        }
                        else if(temp->objectName() == "rrnaREFDBS"){
                            rdbs[temp->item(i)->text()] = "";
                        }
                       // qDebug() << temp->item(i) << " ::: " << "Checked" ;
                    }else{
                      //  qDebug() << temp->item(i) << " ::: " << "Unchecked" ;
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
                // concatenate all the databases together by commas
            }
            if( !rundata->isBitDirty(value))   rundata->setValue(configName, value, _PARAMS);
            rundata->clearsDirtyBit(value);
            // now write them to the parameter file
          //  qDebug() << " Writing out the file";
            Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] + QDir::separator() + "config" +  QDir::separator() + RunData::TEMPLATE_PARAM, "PARAMS", configName, value, false, false);
            // update rundata's param hash, and write out the setting accordingly
        }
    }
    // else {//if(stackedWidget->currentWidget()==stageScroll){ //else part
    {
        // if the user was looking at the stage selection screen

        QList<QGroupBox *> *groupBoxes = stages->groupBoxes;
        QList<QGroupBox *>::iterator i;
        //write radio button changes to file
        for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){

            QGroupBox *temp = *i;
            QString stepName = temp->objectName();
            //get the name, look up in the hash the corresponding setting key value pair
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
            Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] +  QDir::separator() + "config" + QDir::separator()  + RunData::TEMPLATE_PARAM, "PARAMS", configKey, rundata->getValueFromHash(configKey,_PARAMS), false, false);
        }

        //write file format
        QString inputTypeKey = rundata->getConfigMapping().key(stages->fileInputFormat->objectName());
        Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] +  QDir::separator() + "config" + QDir::separator()  + RunData::TEMPLATE_PARAM, "PARAMS", inputTypeKey, stages->fileInputFormat->currentText(), false, false);
        rundata->setValue(inputTypeKey,stages->fileInputFormat->currentText(),_PARAMS);

        //save file input and output selected

        if( this->rundata->isBitDirty("fileInput")) {
           this->rundata->clearsDirtyBit("fileInput");
            stages->selectedFiles = this->rundata->getValueFromHash("fileInput", _PARAMS);
        }
        else {
           this->rundata->setValue("fileInput", stages->selectedFiles,_PARAMS);
        }

        if( this->rundata->isBitDirty("folderOutput")) {
           this->rundata->clearsDirtyBit("folderOutput");
            stages->selectedFolder = this->rundata->getValueFromHash("folderOutput", _PARAMS);
        }
        else {
           this->rundata->setValue("folderOutput", stages->selectedFolder,_PARAMS);
        }

        //override grid choice - if the user chose redo or yes with this ticked, then the step param should be "grid"
        if (stages->gridBlastChoice->isChecked() && rundata->getParams().operator []("metapaths_steps:BLAST_REFDB")!="skip"){
            rundata->setValue("metapaths_steps:BLAST_REFDB","grid",_PARAMS);
            Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] +  QDir::separator() + "config" + QDir::separator()  + RunData::TEMPLATE_PARAM, "PARAMS", "metapaths_steps:BLAST_REFDB", "grid", false, false);
        }

        //trim off commas on refdbs for rRNA and annotation
        QString rRNArefdbs = rundata->getValueFromHash("rRNA:refdbs", _PARAMS);
        rRNArefdbs = rRNArefdbs.remove(QRegExp("[\\s,]*$"));
        rundata->setValue("rRNA:refdbs", rRNArefdbs, _PARAMS);

        QString annotationDBS = rundata->getValueFromHash("annotation:dbs", _PARAMS);
        annotationDBS = annotationDBS.remove(QRegExp("[\\s,]*$"));
        rundata->setValue("annotationDBS", annotationDBS, _PARAMS);

        //number of each - used to count the number of steps left
        rundata->setNumADB(annotationDBS.size());
        rundata->setNumRRNADB(rRNArefdbs.size());

        Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] +  QDir::separator() + "config" + QDir::separator()  + RunData::TEMPLATE_PARAM, "PARAMS", "rRNA:refdbs",rRNArefdbs, false,false);
        Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] +  QDir::separator() + "config" + QDir::separator()  + RunData::TEMPLATE_PARAM, "PARAMS", "annotation:dbs",annotationDBS, false,false);

        executionPrep();
    } //end of else
}

/*
 * Does a bit of cleanup, for extra spaces and dirty characters around our paths and fields.
 */
void MainFrame::executionPrep(){

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
    stackedWidget->setCurrentWidget(progressScroll);
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
