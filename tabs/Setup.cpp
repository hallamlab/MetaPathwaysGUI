
#include "ui_Setup.h"

#include "helper/utilities.h"
#include "tabs/Setup.h"


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
    pgdbFolderTxt = this->findChild<QLineEdit *>("pgdbFolderTxt");
    dbDirectoryTxt = this->findChild<QLineEdit *>("dbDirectoryTxt");
    pathMetaPathwaysTxt = this->findChild<QLineEdit *>("pathMetaPathwaysTxt");
    pathologicTxt = this->findChild<QLineEdit *>("pathologicTxt");

    updateValues();
    // sets up values from the hash, if they exist

    connect(pythonBrowseButton, SIGNAL(clicked()), this, SLOT(pythonBrowse()));
    connect(pgdbFolderBrowseButton, SIGNAL(clicked()), this, SLOT(pgdbFolderBrowse()));
    connect(metapathwaysBrowseButton, SIGNAL(clicked()), this, SLOT(metapathwaysBrowse()));
    connect(databaseButton, SIGNAL(clicked()), this, SLOT(databaseBrowse()));
    connect(pathologicButton,SIGNAL(clicked()), this, SLOT(pathologicBrowse()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveSetup()));
    connect(pythonExecTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave()));
    connect(pgdbFolderTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave()));
    connect(dbDirectoryTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave()));
    connect(pathMetaPathwaysTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave()));
    connect(pathologicTxt, SIGNAL(textEdited(QString)), this, SLOT(canSave()));
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
}

/*
 * Checks to see if any of the text fields are empty. Changes the state of the save button
 * accordingly.
 */
void Setup::canSave(){
    if (
          (!pythonExecTxt->text().isEmpty()) \
       && (!pgdbFolderTxt->text().isEmpty()) \
       && (!pathMetaPathwaysTxt->text().isEmpty()) \
       && (!dbDirectoryTxt->text().isEmpty() \
       && (!pathologicTxt->text().isEmpty()))
    )
    {
        saveButton->setEnabled(true);
        if(pythonPath.isEmpty())
           pythonPath = pythonExecTxt->text();
        if(pgdbFolderPath.isEmpty())
           pgdbFolderPath = pgdbFolderTxt->text();
        if(mpPath.isEmpty())
           mpPath = pathMetaPathwaysTxt->text();
        if(databasePath.isEmpty())
           databasePath = dbDirectoryTxt->text();
        if(pathologicPath.isEmpty())
           pathologicPath = pathologicTxt->text();
    }
    else {
        // Proceed with changes anyway without specifying the essential folders and executables
        ;//saveButton->setEnabled(false);
    }
}

void Setup::pathologicBrowse(){
    pathologicPath = QFileDialog::getOpenFileName(this,tr("Select Pathologic Executable"));
    pathologicTxt->setText(pathologicPath);
    canSave();
}

void Setup::databaseBrowse(){

    databasePath = QFileDialog::getExistingDirectory(this, tr("Select the directory where your databases are defined."));
    dbDirectoryTxt->setText(databasePath);
    canSave();

}

void Setup::pythonBrowse(){

    pythonPath = QFileDialog::getOpenFileName(this,tr("Select Python Executable"));
    pythonExecTxt->setText(pythonPath);
    canSave();

}

void Setup::pgdbFolderBrowse(){
    pgdbFolderPath = QFileDialog::getExistingDirectory(this,tr("Select PGDB Folder"));
    pgdbFolderTxt->setText(pgdbFolderPath);
    canSave();

}

void Setup::metapathwaysBrowse(){

    mpPath = QFileDialog::getExistingDirectory(this,tr("Select MetaPathways Directory"));
    pathMetaPathwaysTxt->setText(mpPath);
    canSave();

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
    QHash<QString,QString> temp = rundata->getConfig();

    this->savePathVariables();
    // save the paths, if there are any provided beforehand

    //write to file only if the user has provided input
    if (!pathMetaPathwaysTxt->text().isEmpty()) {
        temp["METAPATHWAYS_PATH"] = pathMetaPathwaysTxt->text();

        // at this point, we check if there's a template_config file in the METAPATHWAYS_PATH
        // if there isn't we need to copy the default_template_config file as it

        QString template_config = temp["METAPATHWAYS_PATH"] + QDir::separator() +"config" + QDir::separator() + RunData::TEMPLATE_CONFIG;
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


        // do the same for template_param

        QString template_param = temp["METAPATHWAYS_PATH"] + QDir::separator() +"config" + QDir::separator() + RunData::TEMPLATE_PARAM;
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
        default_template_config_file.close();

        Utilities::writeSettingToFile(temp["METAPATHWAYS_PATH"] +  QDir::separator() +"config" + QDir::separator() +RunData::TEMPLATE_CONFIG, "CONFIG", "METAPATHWAYS_PATH", pathMetaPathwaysTxt->text(), false, false);
    }
    if (!pythonExecTxt->text().isEmpty()) {
        temp["PYTHON_EXECUTABLE"] = pythonExecTxt->text();
        Utilities::writeSettingToFile(temp["METAPATHWAYS_PATH"] +  QDir::separator() +"config" + QDir::separator() +RunData::TEMPLATE_CONFIG, "CONFIG", "PYTHON_EXECUTABLE", pythonExecTxt->text(), false, false);
    }
    if (!pgdbFolderTxt->text().isEmpty()) {
        temp["PGDB_FOLDER"] = pgdbFolderTxt->text();
        Utilities::writeSettingToFile(temp["METAPATHWAYS_PATH"] +  QDir::separator() +"config" + QDir::separator() +RunData::TEMPLATE_CONFIG, "CONFIG", "PGDB_FOLDER", pgdbFolderTxt->text(), false, false);
    }
    if (!dbDirectoryTxt->text().isEmpty()) {
        temp["REFDBS"] = dbDirectoryTxt->text();
        Utilities::writeSettingToFile(temp["METAPATHWAYS_PATH"] +  QDir::separator() +"config" + QDir::separator() +RunData::TEMPLATE_CONFIG, "CONFIG", "REFDBS", dbDirectoryTxt->text(), false, false);
    }

    if( !pathologicTxt->text().isEmpty()) {
        temp["PATHOLOGIC_EXECUTABLE"] = pathologicTxt->text();
        Utilities::writeSettingToFile(temp["METAPATHWAYS_PATH"] +  QDir::separator() +"config" + QDir::separator() +RunData::TEMPLATE_CONFIG, "CONFIG", "PATHOLOGIC_EXECUTABLE", pathologicTxt->text(), false, false);
    }
    rundata->setConfig(temp); // update our run config's representation with our (possibly) newly created configs
    emit continueFromSetup(); // sends a signal back to the mainframe to perform checking with validateSetup()
}


void Setup::loadPathVariables(){
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
    canSave();
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
    }
}


void Setup::cancelSetup(){
    close();
}

Setup::~Setup()
{
    delete ui;
}
