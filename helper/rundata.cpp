#include "rundata.h"
#include <QMessageBox>
#include <QSettings>
/*
 * This class contains the data for a run. It is an internal object class that represents all the data
 * about this instance of MetaPathways running. It has
 */

const QString RunData::TEMPLATE_PARAM = "template_param.txt";
const QString RunData::DEFAULT_TEMPLATE_PARAM = "default_template_param.txt";
const QString RunData::TEMPLATE_CONFIG = "template_config.txt";
const QString RunData::DEFAULT_TEMPLATE_CONFIG = "default_template_config.txt";
RunData* RunData::runData = 0;



/**
 * @brief RunData::RunData
 * Private constructor, loads settings values into the CONFIG hash if they already exist.
 *
*/

RunData::RunData(){
    QSettings settings("HallamLab", "MetaPathways");
    this->setValue("METAPATHWAYS_PATH", settings.value("METAPATHWAYS_PATH").toString(),_CONFIG);
    this->setValue("PYTHON_EXECUTABLE", settings.value("PYTHON_EXECUTABLE").toString(),_CONFIG);
    this->setValue("PERL_EXECUTABLE", settings.value("PERL_EXECUTABLE").toString(),_CONFIG);
    this->setValue("PATHOLOGIC_EXECUTABLE", settings.value("PATHOLOGIC_EXECUTABLE").toString(),_CONFIG);
    this->setValue("REFDBS", settings.value("REFDBS").toString(),_CONFIG);
}



/**
 * @brief RunData::getRunData, Initializes and returns our singleton.
 * @return
 */
RunData* RunData::getRunData(){
    if (runData ==0){
        runData = new RunData();
        runData->setCurrentSample(QString());
        runData->initVartoNULL();
        runData->nADB = 0;
        runData->nRRNADB = 0;
        runData->system = "UNKNOWN";
        #ifdef Q_OS_MAC64
        runData->system = "mac";
        #endif
        #ifdef Q_OS_LINUX
        runData->system = "linux";
        #endif
        #ifdef  Q_OS_WIN
        runData->system = "win";
        #endif
    }
    return runData;
}

/*
 * Returns value from
 */

/**
 * @brief RunData::getValueFromHash from the CONFIG or PARAM
 * @param key, key to get from the config
 * @param type, the type of avlue
 * @return
 */
QString RunData::getValueFromHash(QString key,SETTING_TYPE type){
    if (type==_CONFIG)
        return this->CONFIG.contains(key)  ?  this->CONFIG.value(key): QString("");
    if (type==_PARAMS)
        return this->PARAMS.contains(key)  ?  this->PARAMS.value(key): QString("");
}

void RunData::setValue(QString key, QString value, SETTING_TYPE type){
    if(type==_CONFIG) {
        this->CONFIG.insert(key,value);
    }
    if(type==_PARAMS)
        this->PARAMS.insert(key,value);
}

void RunData::initVartoNULL() {
   this->process = 0;
}

void RunData::setParams(QHash<QString,QString> PARAMS){
    this->PARAMS = PARAMS;
}

void RunData::setConfig(QHash<QString,QString> CONFIG){
    this->CONFIG = CONFIG;
}

void RunData::setConfigMapping(QHash<QString,QString> CONFIG_MAPPING){
    this->CONFIG_MAPPING =  CONFIG_MAPPING;
}

void RunData::setProcess(QProcess *process){
    this->process = process;
}

/**
 * @brief RunData::getProcess
 *
 * @return  the pointer to the current process
 */
QProcess * RunData::getProcess(){
    return this->process;
}

void RunData::setRRNADBS(QStringList rrnaDBS){
    this->rrnaDBS = rrnaDBS;
}

void RunData::setAnnotationDBS(QStringList annotationDBS){
    this->annotationDBS = annotationDBS;
}

QHash<QString,QString> RunData::getParams(){
    return this->PARAMS;
}

QHash<QString,QString> RunData::getConfig(){
    return this->CONFIG;
}

QHash<QString,QString> RunData::getConfigMapping(){
    return this->CONFIG_MAPPING;
}



void RunData::setPythonExecutablePath(QString pythonPath) {
    this->CONFIG["PYTHON_EXECUTABLE"] = pythonPath;
}

void RunData::setPerlExecutablePath(QString perlPath) {
    this->CONFIG["PERL_EXECUTABLE"]  = perlPath;
}

void RunData::setMetaPathwaysPath(QString mpPath) {
    this->CONFIG["METAPATHWAYS_PATH"] = mpPath;
}

void RunData::setDatabasesPath(QString databasePath) {
    this->CONFIG["REFDBS"] = databasePath;
}

void RunData::setPathologicPath(QString pathologicPath){
    this->CONFIG["PATHOLOGIC_EXECUTABLE"] = pathologicPath;
}

int RunData::getRunningStepNumber(){
    QHash<QString,QString>::iterator it;
    int count = 0;
    for (it=this->getParams().begin();it!=this->getParams().end();it++){
        QString k = it.key();
        QString v = it.value();

        if(v.contains("yes") || v.contains("grid") || v.contains("skip")){
            count++;
        }
    }
    return count;
}



/**
 * Setup our default configs - if have a path variable already, use it. Otherwise, use the default built in
 * config file from here. Let the user make the changes they need in the setup screen, then write out those
 * changes. We can't make the default_template copies yet, because we don't know the METAPATHWAYS_PATH
 * in the case that the user hasn't specified it. At this point RunData has already been initialized, so
 * we can safely use the config values for METAPATHWAYS_PATH.
* @brief RunData::setupDefaultConfig
 */

void RunData::setupDefaultConfig(){
    QString metapath = this->CONFIG["METAPATHWAYS_PATH"];


    QFileInfo configFile(metapath +  QDir::separator() +"config" + QDir::separator() + this->TEMPLATE_CONFIG);
    QString cpath;
    if (!configFile.exists()) {
        cpath = QString(":/text/")  + this->DEFAULT_TEMPLATE_CONFIG;
    }
    else {
        cpath = metapath + QDir::separator() +"config" + QDir::separator() + this->TEMPLATE_CONFIG;
    }


    QHash<QString, QString> parsedResults = Utilities::parseFileConfig(cpath);
    if (configFile.exists()) parsedResults["METAPATHWAYS_PATH"] = metapath;

    this->setConfig(parsedResults);

}


/**
 * Setup our default params - if have a path variable already, use it. Otherwise, use the default built in
 * param file from here. Let the user make the changes they need in the setup screen, then write out those
 * changes.
 * @brief RunData::setupDefaultParams
 */

void RunData::setupDefaultParams(){
    QString path = this->CONFIG["METAPATHWAYS_PATH"];
    QFileInfo paramFile(path + QDir::separator() +"config" + QDir::separator()  + this->TEMPLATE_PARAM);

    if (!paramFile.exists()) {
        path = QString(":/text/")  + this->DEFAULT_TEMPLATE_PARAM;
    }
    else {
        path = path + QDir::separator() +"config" + QDir::separator()  + this->TEMPLATE_PARAM;
    }



    QHash<QString,QString> parsed = Utilities::parseFile(path,"PARAMS");


    this->setParams(parsed);
}
void RunData::setCurrentSample(QString currentSample) {
    this->currentSample = currentSample;
}

QString RunData::getCurrentSample() {
    return this->currentSample;
}

QString RunData::getCurrentInputFormat() {

    return this->currentInputFormat;

}


void RunData::updateInputFileFormat(QString currentInputFileFormat) {
   this->currentInputFormat = currentInputFileFormat;
}

bool RunData::checkParams(){
    QFile paramFile( this->CONFIG["METAPATHWAYS_PATH"] +  QDir::separator() +"config" + QDir::separator() + this->TEMPLATE_PARAM);

    if (paramFile.exists()) return true;
    else return false;
}

unsigned int RunData::getNumRRNADB(){
    return this->nRRNADB;
}

unsigned int RunData::getNumADB(){
    return this->nADB;
}

QStringList RunData::getrNADBNames() {
   return this->rrnaDBS;
}

QStringList RunData::getADBNames() {
   return this->annotationDBS;
}

void RunData::setNumRRNADB(unsigned int n){
    this->nRRNADB = n;
}

void RunData::setNumADB(unsigned int n){
    this->nADB = n;
}

/**
 * @brief RunData::checkConfig, checks if config file exits
 * @return bool
 */
bool RunData::checkConfig(){

    QFile configFile(  this->CONFIG["METAPATHWAYS_PATH"] +  QDir::separator() +"config" + QDir::separator()  +this->TEMPLATE_CONFIG);

    if (configFile.exists()) return true;
    else return false;
}

 QHash<QString, QString> RunData::getPARAMS() {
     return this->PARAMS;
 }

 /**
 * @brief RunData::validate, validates the config file for the python code
 * @param warningMsg, warning message to show
 * @return bool to say fail or success
 */

bool RunData::validate(QString &warningMsg) {
    bool correct = true;
    QFileInfo file(this->getValueFromHash("PYTHON_EXECUTABLE", _CONFIG));
    if( !file.exists() ) {
        warningMsg = warningMsg + " - Python executable path\n";
        correct = false;
    }

    file.setFile(this->getValueFromHash("REFDBS", _CONFIG));
    if( !file.exists() ) {
        warningMsg = warningMsg + " - Database path\n";
        correct = false;
    }

    file.setFile(this->getValueFromHash("METAPATHWAYS_PATH", _CONFIG));
    QFileInfo  file1(this->getValueFromHash("METAPATHWAYS_PATH", _CONFIG) + QDir::separator() + "MetaPathways.py");

    if( !file.exists() || !file1.exists() ) {
        warningMsg = warningMsg + " - MetaPathways.py is missing from your specified MetaPathways directory. Please verify it exists!\n";
        correct = false;
    }

    QFileInfo configFile(this->getValueFromHash("METAPATHWAYS_PATH",_CONFIG) +  QDir::separator() +"config" + QDir::separator() + this->TEMPLATE_CONFIG);
    if( !configFile.exists() ){
        warningMsg = warningMsg + "- template_config.txt does not exist in your specified MetaPathways Directory!\n";
        correct = false;
    }

    QFileInfo paramFile(this->getValueFromHash("METAPATHWAYS_PATH",_CONFIG) + QDir::separator() + "config" + QDir::separator() +  this->TEMPLATE_PARAM);
    if( !paramFile.exists() ){
        warningMsg = warningMsg + "- template_param.txt does not exist in your specified MetaPathways Directory!\n";
        correct = false;
    }

    return correct;
}


void RunData::addFileToList(QString file) {
    if(! this->files.contains(file))
        this->files.append(file);
}

void RunData::setFileList(QStringList files) {
    this->files  = files;

}

QStringList RunData::getFileList(const QString &fileType){
    this->loadInputFiles(fileType);
    return this->files;
}



void RunData::updateCurrentFileList(){
   this->currentfiles = this->files;
}

QStringList RunData::getCurrentFileList(){
    return this->currentfiles;
}

/**
 * @brief RunData::loadInputFiles, loading the names of the input file of a fileType
 * such as fasta, gbk-annotated, gbk-unannotated
 * @param fileType, the file type to pick
 */
void RunData::loadInputFiles(const QString &fileType){
    QDir currentDir(this->getParams()["fileInput"]);

    if( !currentDir.exists() ) return;

    currentDir.setFilter(QDir::Files);
    QStringList entries = currentDir.entryList();


    QList<QRegExp> regList;

    // for fasta type only
    regList << QRegExp("[.][fF][aA][sS][tT][aA]$") << QRegExp("[.][fF][aA]$") << QRegExp("[.][fF][aA][aA]$") << QRegExp("[.][fF][aA][sS]") << QRegExp("[.][fF][nN][aA]$");


    QStringList filesDetected;
    foreach( QString entry, entries )
    {
        QString temp = entry;
        QStringList file = temp.split(".");

        QString suffix = QString(".") + file.last();


        if (fileType.compare(QString("fasta"))== 0){
            foreach(QRegExp reg, regList ) {
               if(suffix.indexOf(reg,0) != -1 ) {
                   filesDetected.append( temp.remove(reg).replace('.','_') );
                   break;
               }
            }
        }
        else if (  fileType.compare( QString("gbk-annotated")) ==0 || fileType.compare(QString("gbk-unannotated"))==0){
            if (file.last().compare(QString("gbk"),Qt::CaseInsensitive) ==0){
                filesDetected.append(file.first());
            }
        }
    }


    this->setFileList(filesDetected);
}


/**
 * @brief RunData::getOutputFolders, returns the list of outputfolders
 * @return list of output folders
 */
QStringList RunData::getOutputFolders() {

   if(this->outputFolders.isEmpty()) this->loadOutputFolders();
   return this->outputFolders;
}


/**
 * @brief RunData::loadOutputFolders, loads the list output folders in the
 * outoutFolder variable.
 */
void RunData::loadOutputFolders(){
    QDir currentDir(this->getParams()["folderOutput"]);

    if( !currentDir.exists() ) return;

    QStringList sampleFolders = currentDir.entryList(QDir::AllDirs);

    this->outputFolders.clear();
    foreach(QString sampleFolder, sampleFolders) {
        if( sampleFolder.compare(QString(".")) ==0  || sampleFolder.compare(QString("..")) ==0 ) continue;

        if( this->isOutputFolderValid(currentDir.absolutePath() + QDir::separator() +  sampleFolder )) {
            this->outputFolders.append(sampleFolder);
        }
    }

}

/**
 * @brief RunData::isOutputFolderValid, checks if the folder contains
 * the right structure of a MetaPathways run output corresponding to a sample
 * @param folder, the sample folder
 * @return
 */
bool RunData::isOutputFolderValid(const QString &folder) {
    QDir qDir(folder);

    if( !qDir.exists()) return false;
    QStringList subFolders = this->getSubFolders(folder);

    foreach(QString subFolder, subFolders) {
        qDir.setPath(folder + QDir::separator() + subFolder);
        if( !qDir.exists() ) return false;
    }
    return true;

}

/**
 * @brief RunData::getSubFolders, get the immediate subfolders of the
 * folder
 * @param folder, the containing folder
 * @return list of folders
 */

QStringList RunData::getSubFolders(const QString & folder) {
    QStringList subFolders ;
    subFolders<<  QString("blast_results")
                            <<    QString("ptools")
                            <<  QString("mltreemap_calculations")
                            <<     QString("results")
                            <<    QString("genbank")
                            <<    QString("orf_prediction")
                            <<    QString("run_statistics")
                            <<    QString("preprocessed");

    return subFolders;

}

/**
 * @brief RunData::setSamplesSubsetToRun, sets the selection of samples to
 * run
 * @param selection
 */
void RunData::setSamplesSubsetToRun(QList<QString> &selection) {
   this->selectSamplesToRun = selection;
}

/**
 * @brief RunData::getSamplesSubsetToRun, returns
 * the list of samples to run
 * @return the list of samples to run
 */

QList<QString> RunData::getSamplesSubsetToRun() {
    return this->selectSamplesToRun;
}



/**
 * @brief RunData::emitloadSampleList, emits a single to the drop down menu in
 * the Progress tab to load the selected samples
 */
void RunData::emitloadSampleList() {
    emit loadSampleList();
}

QString RunData::getSystem() {
    return this->system;
}


/* Saves the context against the key
 * \param key: the value for the key
 * \return the value of the key as a QVariant
 */
bool RunData::saveContext(const QString &key, QVariant value) {
     QSettings settings("HallamLab", "MetaPathways");
     settings.setValue(key, value);
     return true;
}

/*
 * Checks if the Qsettings has a context by the name of the key
 * \param key : key for the value
 * \return true
 */
bool RunData::hasContext(const QString &key) {
     QSettings settings("HallamLab", "MetaPathways");
     if( settings.value(key) == QVariant() ) return false;
     return true;
}

/* Gets the context against the key
 * \param key: the value for the key
 * \return the value of the key as a QVariant
 */

/**
 * @brief RunData::getContext  Gets the context against the key
 * @param  key
 * @return the value of the key as a QVariant
 */
QVariant RunData::getContext(const QString &key) {
     QSettings settings("HallamLab", "MetaPathways");
     return settings.value(key);
}
