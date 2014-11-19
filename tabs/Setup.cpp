
#include "ui_Setup.h"

#include "helper/utilities.h"
#include "tabs/Setup.h"
#include "popupviews/setupstatus.h"


/*
 * Setups connections for all the buttons and checking mechanisms.
 */
Setup::Setup(QWidget *parent) : QWidget(parent), ui(new Ui::Setup)
{
    ui->setupUi(this);

    pythonBrowseButton = this->findChild<QPushButton *>("pythonBrowseButton");
    pgdbFolderBrowseButton = this->findChild<QPushButton *>("pgdbFolderBrowseButton");
    metapathwaysBrowseButton = this->findChild<QPushButton *>("metapathwaysBrowseButton");
    saveButton = this->findChild<QPushButton *>("saveButton");
    databaseButton = this->findChild<QPushButton *>("databaseButton");
    pathologicButton = this->findChild<QPushButton *>("pathologicButton");

    pythonExecTxt = this->findChild<QLineEdit *>("pythonExecTxt");
    executablesCombo = this->findChild<QComboBox *>("executablesCombo");
    pgdbFolderTxt = this->findChild<QLineEdit *>("pgdbFolderTxt");
    dbDirectoryTxt = this->findChild<QLineEdit *>("dbDirectoryTxt");
    pathMetaPathwaysTxt = this->findChild<QLineEdit *>("pathMetaPathwaysTxt");
    pathologicTxt = this->findChild<QLineEdit *>("pathologicTxt");

    QStringList labelNames = QStringList() << "check0" << "check1" << "check2" << "check3" << "check4" << "check5";
    foreach(QString labelName, labelNames) {
       QLabel *qlabel = this->findChild<QLabel *>(labelName);
       qlabel->hide();
    }


    // sets up values from the hash, if they exist

    connect(pythonBrowseButton, SIGNAL(clicked()), this, SLOT(pythonBrowse()));
    connect(pgdbFolderBrowseButton, SIGNAL(clicked()), this, SLOT(pgdbFolderBrowse()));
    connect(metapathwaysBrowseButton, SIGNAL(clicked()), this, SLOT(metapathwaysBrowse()));
    connect(databaseButton, SIGNAL(clicked()), this, SLOT(databaseBrowse()));
    connect(pathologicButton,SIGNAL(clicked()), this, SLOT(pathologicBrowse()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveconfig()));

    QSignalMapper *signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SIGNAL(editSlotChanged(int)));
    connect(this, SIGNAL(editSlotChanged(int)), this, SLOT(canSave(int)));

    signalMapper->setMapping(pathMetaPathwaysTxt, 0);
    signalMapper->setMapping(dbDirectoryTxt, 1);
    signalMapper->setMapping(executablesCombo, 2);
    signalMapper->setMapping(pythonExecTxt, 3);
    signalMapper->setMapping(pgdbFolderTxt, 4);
    signalMapper->setMapping(pathologicTxt, 5);

    connect(pythonExecTxt,  SIGNAL( textChanged(QString)), signalMapper, SLOT(map()));
    connect(executablesCombo, SIGNAL( activated(QString)), signalMapper, SLOT(map()));
    connect(executablesCombo, SIGNAL(currentIndexChanged(int)), signalMapper, SLOT(map()));
    connect(pgdbFolderTxt, SIGNAL( textChanged(QString)), signalMapper, SLOT(map()));
    connect(dbDirectoryTxt, SIGNAL( textChanged(QString)), signalMapper, SLOT(map()));
    connect(pathMetaPathwaysTxt, SIGNAL( textChanged(QString)), signalMapper, SLOT(map()));
    connect(pathMetaPathwaysTxt, SIGNAL(textChanged(QString)), this, SLOT(updateExecutablesFolderList()));
    connect(pathologicTxt, SIGNAL(textChanged(QString)), signalMapper, SLOT(map()));

    this->updateValues();
}

/*
 * Sets up initial values based off of what was loaded into the CONFIG at startup (see
 * RunData's private constructor).
 */
void Setup::updateValues(){
    RunData *rundata = RunData::getRunData();
    pythonExecTxt->setText(rundata->getValueFromHash("PYTHON_EXECUTABLE",_CONFIG));
    pgdbFolderTxt->setText(rundata->getValueFromHash("PGDB_FOLDER",_CONFIG));
    dbDirectoryTxt->setText(rundata->getValueFromHash("REFDBS",_CONFIG));
    pathMetaPathwaysTxt->setText(rundata->getValueFromHash("METAPATHWAYS_PATH",_CONFIG));
    pathologicTxt->setText(rundata->getValueFromHash("PATHOLOGIC_EXECUTABLE",_CONFIG));
    this->updateExecutablesFolderList();
}


void Setup::updateExecutablesFolderList() {
    RunData *rundata = RunData::getRunData();
    QStringList executableDirs = this->getExecutablesPath(pathMetaPathwaysTxt->text());

    //executablesCombo->setText(rundata->getValueFromHash("EXECUTABLES_DIR",_CONFIG));
    QHash<QString, int> alreadyAdded;

    this->executablesCombo->clear();

    int i =0;
    foreach(QString _folder, executableDirs) {
        QString folder = QString("executables") + QDir::separator() + _folder;
        if( alreadyAdded.contains(folder)) continue;
        executablesCombo->addItem( folder);
       // if( this->hasValidBinaries(pathMetaPathwaysTxt->text(), folder) ) executablesCombo->setCurrentIndex(alreadyAdded.size());
        alreadyAdded[folder] = i++;
    }

    // set the value in the config file to the current
    QString valueFromHash =   rundata->getValueFromHash("EXECUTABLES_DIR",_CONFIG);
    if( !valueFromHash.isEmpty() &&  alreadyAdded.contains(valueFromHash) ) {
        if(!alreadyAdded.contains(valueFromHash)) {
            executablesCombo->addItem(valueFromHash);
            alreadyAdded[valueFromHash] = i;
        }
        this->executablesCombo->setCurrentIndex(alreadyAdded[valueFromHash]);
    }
}



bool Setup::hasValidBinaries(QString metapathways_dir, QString folder) {
    QString extended_dir = metapathways_dir + QDir::separator() + QString("executables")  ;
    QDir dir(extended_dir);
    return false;

}


/**
 * @brief Setup::getExecutablesPath, returns the list of folders that contains executables
 * for various OS
 * @param metapathways_dir
 * @return
 */

QStringList Setup::getExecutablesPath(QString metapathways_dir) {

    QString extended_dir = metapathways_dir + QDir::separator() + QString("executables");
    QDir dir(extended_dir);

    if( !dir.exists()) return QStringList();

    QStringList filterList;
    filterList << QString(".") << QString("..");

    QStringList _folders = dir.entryList();
    QStringList folders;

    foreach(QString _folder, _folders) {
        if( _folder.compare(QString(".."))==0  || _folder.compare(QString("."))==0 )  continue;
         dir.setPath(extended_dir + QDir::separator() + _folder);
         if(!dir.exists()) continue;

         folders << _folder;
    }

  //  qDebug() << "Check all binaries ";

  //  qDebug() << folders << metapathways_dir;


    return folders;
}

/*
 * Checks to see if any of the text fields are empty. Changes the state of the save button
 * accordingly.
 */
void Setup::canSave(int i){
    if (
          (!pythonExecTxt->text().isEmpty()) \
       && (!executablesCombo->currentText().isEmpty()) \
       && (!pgdbFolderTxt->text().isEmpty()) \
       && (!pathMetaPathwaysTxt->text().isEmpty()) \
       && (!dbDirectoryTxt->text().isEmpty() \
       && (!pathologicTxt->text().isEmpty()))
    )
    {
        saveButton->setEnabled(true);
        if(pythonPath.isEmpty())
           pythonPath = pythonExecTxt->text();
        if(executablesPath.isEmpty())
           executablesPath = executablesCombo->currentText();
        if(pgdbFolderPath.isEmpty())
           pgdbFolderPath = pgdbFolderTxt->text();
        if(mpPath.isEmpty())
           mpPath = pathMetaPathwaysTxt->text();
        if(databasePath.isEmpty())
           databasePath = dbDirectoryTxt->text();
        if(pathologicPath.isEmpty())
           pathologicPath = pathologicTxt->text();
    }

    this->validatePartialSetup(i);


}

void Setup::pathologicBrowse(){
    pathologicPath = QFileDialog::getOpenFileName(this,tr("Select Pathologic Executable"));
    pathologicTxt->setText(pathologicPath);
  //  canSave();
}

void Setup::databaseBrowse(){

    databasePath = QFileDialog::getExistingDirectory(this, tr("Select the directory where your databases are defined."));
    dbDirectoryTxt->setText(databasePath);
  //  canSave();

}

void Setup::pythonBrowse(){

    pythonPath = QFileDialog::getOpenFileName(this,tr("Select Python Executable"));
    pythonExecTxt->setText(pythonPath);
   // canSave();

}

void Setup::pgdbFolderBrowse(){
    pgdbFolderPath = QFileDialog::getExistingDirectory(this,tr("Select PGDB Folder"));
    pgdbFolderTxt->setText(pgdbFolderPath);
  //  canSave();

}

void Setup::metapathwaysBrowse(){

    mpPath = QFileDialog::getExistingDirectory(this,tr("Select MetaPathways Directory"));
    pathMetaPathwaysTxt->setText(mpPath);
   // canSave();

}

/*
 * Two scenarios are possible here : 1.) the user has not defined a METAPATHWAYS_PATH so we cannot
 * assume the existence of a config nor parameter file, or 2.) the user has, so we can locate the file.
 * In the first case, we must then use the default_template_config and default_template_param file to
 * fill in the CONFIG and PARAMS hash in rundata, and also copy over the files to the path as the template_config and template
 * _ param files once they have been defined. In this way it is crucial that the user first provides the METAPATHWAYS_PATH, otherwise nothing
 * else can proceed, since all other config settings cannot be written at all.
 */
void Setup::saveSetup(){
    RunData *rundata = RunData::getRunData();
    QHash<QString,QString> temp = rundata->getTempConfig();

    this->savePathVariables();
    // save the paths, if there are any provided beforehand

    //write to file only if the user has provided input
    if (!pathMetaPathwaysTxt->text().isEmpty()) {
        this->saveConfigFile();
        // do the same for template_param
   //     this->saveParamFile();
    }

    foreach(QString configKey,  temp.keys() ) {
       // qDebug() << configKey << "  " << temp[configKey];
        Utilities::writeSettingToFile(temp["METAPATHWAYS_PATH"] +  QDir::separator() +"config" + QDir::separator() +RunData::TEMPLATE_CONFIG, "CONFIG", configKey, temp[configKey], false, false);
    }
    rundata->setConfig(temp); // update our run config's representation with our (possibly) newly created configs
    //emit continueFromSetup(); // sends a signal back to the mainframe to perform checking with validateSetup()
    rundata->triggerParameterFileRead();
}


void Setup::saveConfigFile() {
    QString dirname = pathMetaPathwaysTxt->text() + QDir::separator() +"config" + QDir::separator();
    QDir dir(dirname);
    if( !dir.exists()) {
        Utilities::showInfo(this, QString("Reqired folder ") +  dirname+ QString("does not exist!"));
        return;
    }


    QString template_config = pathMetaPathwaysTxt->text() + QDir::separator() +"config" + QDir::separator() + RunData::TEMPLATE_CONFIG;
    QFile template_config_file(template_config);
    QFile default_template_config_file(":/text/" + RunData::DEFAULT_TEMPLATE_CONFIG);
    QFileInfo t_file(template_config);

    if (t_file.exists()){
        // see if it's empty - then we need to remove the old first

        if(Utilities::checkEmptyFile(template_config)){
            // delete old, copy default to the location where it needs to be
            template_config_file.remove();
            default_template_config_file.copy(template_config);
        }
    }else{
        // it doesn't exist, so copy it
      //  qDebug() << "Copyintg " << template_config;
        default_template_config_file.copy(template_config);
    }
    template_config_file.close();
    default_template_config_file.close();

}

void Setup::saveParamFile() {
    QString template_param = pathMetaPathwaysTxt->text() + QDir::separator() +"config" + QDir::separator() + RunData::TEMPLATE_PARAM;
    QFile template_param_file(template_param);
    QFile default_template_param_file(":/text/" + RunData::DEFAULT_TEMPLATE_PARAM);
    QFileInfo p_file(template_param);
    if (p_file.exists()){
        // see if it's empty - then we need to remove the old first

        if(Utilities::checkEmptyFile(template_param)){
            // delete old, copy default to the location where it needs to be
            template_param_file.remove();
            default_template_param_file.copy(template_param);
        }
    }else{
        // it doesn't exist, so copy it
        default_template_param_file.copy(template_param);
    }
    template_param_file.close();

}

void Setup::validatePartialSetup(int i) {
     RunData *rundata= RunData::getRunData();

     QLabel *qlabel;
     bool success;

     if(i==3) {
         QString message0;
         success = rundata->checkPythonBinary(message0,  pythonExecTxt->text());
         qlabel = this->findChild<QLabel *>("check3");
         success ? qlabel->show() : qlabel->hide();
     }

     if(i==0) {
         QString message1;
         success = rundata->checkMetaPathwaysPath(message1,  pathMetaPathwaysTxt->text());
         qlabel = this->findChild<QLabel *>("check0");
         success ? qlabel->show() : qlabel->hide();
    }

     if(i==2) {
         QString message2;
         success = rundata->checkBinaries(message2, pathMetaPathwaysTxt->text() + QDir::separator() + executablesCombo->currentText());
         qlabel = this->findChild<QLabel *>("check2");
         success ? qlabel->show() : qlabel->hide();
     }

     if(i==1) {
         QString message3;
         success = rundata->checkRefDatabasesPath(message3,  dbDirectoryTxt->text());
         qlabel = this->findChild<QLabel *>("check1");
         success ? qlabel->show() : qlabel->hide();
         rundata->triggerParameterFileRead();
      }

     if(i==4) {
         QString message4;
         success =rundata->checkPGDBFolderPath(message4, pgdbFolderTxt->text());
         qlabel = this->findChild<QLabel *>("check4");
         success ? qlabel->show() : qlabel->hide();
     }

     if(i==5) {
         QString message5;
         success= rundata->checkPathwayToolsExecutable(message5, pathologicTxt->text());
         qlabel = this->findChild<QLabel *>("check5");
         success ? qlabel->show() : qlabel->hide();
     }
     /*
     if( !rundata->validate(warningStr)) {
          this->showSetupError(warningStr);
     }
*/
   // addRemainingTabs();
   // stackedWidget->setCurrentWidget(settingsScroll);
   // greyTabs(true);

}


void Setup::validateSetup(bool interactive, bool triggerLoad) {
     RunData *rundata= RunData::getRunData();

     QLabel *qlabel;
     bool success;

     QString message0;
     QString executable =rundata->getValueFromHash("PYTHON_EXECUTABLE", _TEMP_CONFIG);
     success = rundata->checkPythonBinary(message0, executable);
     qlabel = this->findChild<QLabel *>("check3");
     success ? qlabel->show() : qlabel->hide();


     QString message1;
     QString metapathwaysDir = rundata->getValueFromHash("METAPATHWAYS_PATH", _TEMP_CONFIG);
     success = rundata->checkMetaPathwaysPath(message1, metapathwaysDir);
     qlabel = this->findChild<QLabel *>("check0");
     success ? qlabel->show() : qlabel->hide();


     QString message2;
     QString executablesDir = rundata->getValueFromHash("METAPATHWAYS_PATH", _TEMP_CONFIG) +\
     QDir::separator() + rundata->getValueFromHash("EXECUTABLES_DIR", _TEMP_CONFIG);
     success = rundata->checkBinaries(message2, executablesDir);
     qlabel = this->findChild<QLabel *>("check2");
     success ? qlabel->show() : qlabel->hide();

     QString message3;
     QString refdbsDir = rundata->getValueFromHash("REFDBS", _TEMP_CONFIG);
     success = rundata->checkRefDatabasesPath(message3, refdbsDir);
     qlabel = this->findChild<QLabel *>("check1");
     success ? qlabel->show() : qlabel->hide();

     if( triggerLoad )rundata->triggerParameterFileRead();


     QString message4;
     QString pgdbFolderPath = rundata->getValueFromHash("PGDB_FOLDER", _TEMP_CONFIG);
     success =rundata->checkPGDBFolderPath(message4, pgdbFolderPath);
     qlabel = this->findChild<QLabel *>("check4");
     success ? qlabel->show() : qlabel->hide();


     QString message5;
     QString pathwayexecutable = rundata->getValueFromHash("PATHOLOGIC_EXECUTABLE", _TEMP_CONFIG);
     success= rundata->checkPathwayToolsExecutable(message5, pathwayexecutable);
     qlabel = this->findChild<QLabel *>("check5");
     success ? qlabel->show() : qlabel->hide();


     if( interactive ) {
         QStringList messages;
         messages <<  message1 <<  message3 << message2 << message0 << message4 << message5;
         SetUpStatus *status = new SetUpStatus(messages, this);
         connect(status, SIGNAL(saveConfigFile()), this, SLOT(saveSetup()) );
         status->show();
     }

}

void Setup::saveconfig() {
   RunData *rundata= RunData::getRunData();

   QHash<QString,QString> temp = rundata->getConfig();

   // save the paths, if there are any provided beforehand

   //write to file only if the user has provided input
   //if (!pathMetaPathwaysTxt->text().isEmpty()) {
       temp["METAPATHWAYS_PATH"] = pathMetaPathwaysTxt->text();


   //if (!pythonExecTxt->text().isEmpty()) {
       temp["PYTHON_EXECUTABLE"] = pythonExecTxt->text();


   //if (!executablesCombo->currentText().isEmpty()) {
       temp["EXECUTABLES_DIR"] = executablesCombo->currentText();


//   if (!pgdbFolderTxt->text().isEmpty())
       temp["PGDB_FOLDER"] = pgdbFolderTxt->text();


  // if (!dbDirectoryTxt->text().isEmpty()) {
   temp["REFDBS"] = dbDirectoryTxt->text();



   // if( !pathologicTxt->text().isEmpty())
       temp["PATHOLOGIC_EXECUTABLE"] = pathologicTxt->text();

   rundata->setTempConfig(temp); // update our run config's representation with our (possibly) newly created configs
   this->validateSetup(true);

  // rundata->

}

void Setup::showSetupError(QString warningStr) {
   /*
    warning->warning(0,"Incomplete Configuration !\n", QString("INCOMPLETE CONFIGURATION MAY DISABLE SOME FEATURES  \n\n") + warningStr + "Please check Setup!.", QMessageBox::Ok);


    greyTabs(false);
    this->openSetup();
    setupWidget->updateValues();
    */
}



void Setup::_loadPathVariables(){
    RunData *rundata = RunData::getRunData();
    QSettings settings("HallamLab", "MetaPathways");

    if( settings.allKeys().contains("METAPATHWAYS_PATH") ) {
        pathMetaPathwaysTxt->setText(settings.value("METAPATHWAYS_PATH").toString());
        rundata->setMetaPathwaysPath(settings.value("METAPATHWAYS_PATH").toString());
    }

    if( settings.allKeys().contains("PYTHON_EXECUTABLE") ) {
        pythonExecTxt->setText(settings.value("PYTHON_EXECUTABLE").toString());
        rundata->setPythonExecutablePath(settings.value("PYTHON_EXECUTABLE").toString());
    }

    if( settings.allKeys().contains("EXECUTABLES_DIR") ) {
        rundata->setPythonExecutablePath(settings.value("EXECUTABLES_DIR").toString());
    }

    if( settings.allKeys().contains("PGDB_FOLDER") ) {
        pgdbFolderTxt->setText(settings.value("PGDB_FOLDER").toString());
        rundata->setPgdbFolderPath(settings.value("PGDB_FOLDER").toString());
    }

    if( settings.allKeys().contains("REFDBS") ) {
        dbDirectoryTxt->setText(settings.value("REFDBS").toString());
        rundata->setDatabasesPath(settings.value("REFDBS").toString());
    }

    if( settings.allKeys().contains("PATHOLOGIC_EXECUTABLE") ) {
        pathologicTxt->setText(settings.value("PATHOLOGIC_EXECUTABLE").toString());
        rundata->setPathologicPath(settings.value("PATHOLOGIC_EXECUTABLE").toString());
    }
//    canSave();
    // call canSave here instead of in the constructor since the values will be populated only
    // after we construct this object anyways
}

/*
 * Saves path variables for each config value.
 */
void Setup::savePathVariables(){
    QSettings settings("HallamLab", "MetaPathways");
    RunData *run = RunData::getRunData();
    if( !pathMetaPathwaysTxt->text().isEmpty()){
        settings.setValue("METAPATHWAYS_PATH", pathMetaPathwaysTxt->text());
        run->setMetaPathwaysPath(pathMetaPathwaysTxt->text());
    }


    if( !pythonExecTxt->text().isEmpty()){
        settings.setValue("PYTHON_EXECUTABLE", pythonExecTxt->text());
        run->setPythonExecutablePath(pythonExecTxt->text());
    }

    /*
    if( !executablesCombo->currentText().isEmpty()){
        settings.setValue("EXECUTABLES_DIR", executablesCombo->currentText());
        run->setPythonExecutablePath( executablesCombo->currentText()  );
    }

    if( !pgdbFolderTxt->text().isEmpty()){
        settings.setValue("PGDB_FOLDER", pgdbFolderTxt->text());
        run->setPgdbFolderPath(pgdbFolderTxt->text());
    }
    if( !dbDirectoryTxt->text().isEmpty()) {
       settings.setValue("REFDBS",  dbDirectoryTxt->text());
       run->setDatabasesPath(dbDirectoryTxt->text());
    }
    if(!pathologicTxt->text().isEmpty()) {
        settings.setValue("PATHOLOGIC_EXECUTABLE", pathologicTxt->text());
        run->setPathologicPath(pathologicTxt->text());
    }*/
}


void Setup::cancelSetup(){
    close();
}




Setup::~Setup()
{
    delete ui;
}
