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

/*
 * Private constructor, loads settings values into the CONFIG hash if they already exist.
 */
RunData::RunData(){
    QSettings settings("HallamLab", "MetaPathways");
    this->setValue("METAPATHWAYS_PATH", settings.value("METAPATHWAYS_PATH").toString(),_CONFIG);
    this->setValue("PYTHON_EXECUTABLE", settings.value("PYTHON_EXECUTABLE").toString(),_CONFIG);
    this->setValue("PERL_EXECUTABLE", settings.value("PERL_EXECUTABLE").toString(),_CONFIG);
    this->setValue("PATHOLOGIC_EXECUTABLE", settings.value("REFDBS").toString(),_CONFIG);
    this->setValue("REFDBS", settings.value("PATHOLOGIC_EXECUTABLE").toString(),_CONFIG);
}

/*
 * Initializes and returns our singleton.
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

/*
 * Setup our default configs - if have a path variable already, use it. Otherwise, use the default built in
 * config file from here. Let the user make the changes they need in the setup screen, then write out those
 * changes. We can't make the default_template copies yet, because we don't know the METAPATHWAYS_PATH
 * in the case that the user hasn't specified it. At this point RunData has already been initialized, so
 * we can safely use the config values for METAPATHWAYS_PATH.
 */
void RunData::setupDefaultConfig(){
    QString path = this->CONFIG["METAPATHWAYS_PATH"];
    QFileInfo configFile(path);
    if (!configFile.exists()) path = QString(":/text/")  + this->DEFAULT_TEMPLATE_CONFIG;
    else path = path + QDir::separator() +"config" + QDir::separator() + this->TEMPLATE_CONFIG;
    this->setConfig(Utilities::parseFile(path,"CONFIG"));
}

/*
 * Setup our default params - if have a path variable already, use it. Otherwise, use the default built in
 * param file from here. Let the user make the changes they need in the setup screen, then write out those
 * changes.
 */
void RunData::setupDefaultParams(){
    QString path = this->CONFIG["METAPATHWAYS_PATH"];
    QFileInfo paramFile(path);
    if (!paramFile.exists()) path = QString(":/text/")  + this->DEFAULT_TEMPLATE_PARAM;
    else path = path + QDir::separator() +"config" + QDir::separator()  + this->TEMPLATE_PARAM;

    QHash<QString,QString> parsed = Utilities::parseFile(path,"PARAMS");

    this->setParams(parsed);
}
void RunData::setCurrentSample(QString currentSample) {
    this->currentSample = currentSample;
}

QString RunData::getCurrentSample() {
    return this->currentSample;
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

void RunData::setNumRRNADB(unsigned int n){
    this->nRRNADB = n;
}

void RunData::setNumADB(unsigned int n){
    this->nADB = n;
}

bool RunData::checkConfig(){
  //  qDebug() << " config file ";
    QFile configFile(  this->CONFIG["METAPATHWAYS_PATH"] +  QDir::separator() +"config" + QDir::separator()  +this->TEMPLATE_CONFIG);

    if (configFile.exists()) return true;
    else return false;
}

 QHash<QString, QString> RunData::getPARAMS() {
     return this->PARAMS;
 }

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
    QFileInfo  file1(this->getValueFromHash("METAPATHWAYS_PATH", _CONFIG) + QDir::separator() + "bin" + QDir::separator() + "MetaPathways.py");

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

QStringList RunData::getFileList(){
    this->loadInputFiles();
    return this->files;

}



void RunData::loadInputFiles(){
    QDir currentDir(this->getParams()["fileInput"]);
    QString fileType = this->getParams()["INPUT:format"];

    if( !currentDir.exists() ) return;

    currentDir.setFilter(QDir::Files);
    QStringList entries = currentDir.entryList();

    QList<QRegExp> regList;
    regList << QRegExp("[.][fF][aA][sS][tT][aA]$") << QRegExp("[.][fF][aA]$") << QRegExp("[.][fF][aA][aA]$") << QRegExp("[.][fF][aA][sS]") << QRegExp("[.][fF][nN][aA]$");

    QStringList filesDetected;
    for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry )
    {
        QString temp = *entry;
        QStringList file = temp.split(".");

        QString suffix = QString(".") + file.last();

        if (fileType == "fasta"){
            foreach(QRegExp reg, regList ) {
               if(suffix.indexOf(reg,0) > -1 ) {
                   filesDetected.append( temp.remove(reg).replace('.','_') );
                   break;
               }
            }
        }
        else if (fileType == "gbk-annotated" || fileType == "gbk-unannotated"){
            if (file.last() == "gbk"){
                filesDetected.append(file.first());
            }
        }
        else if (fileType == "gff-annotated" || fileType == "gff-unannotated"){
            if (file.last() == "gff"){
                filesDetected.append(file.first());
            }
        }
    }

    this->setFileList(filesDetected);
}

void RunData::setSamplesSubsetToRun(QList<QString> &selection) {
   this->selectSamplesToRun = selection;
}

QList<QString> RunData::getSamplesSubsetToRun() {
    return this->selectSamplesToRun;
}

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
QVariant RunData::getContext(const QString &key) {
     QSettings settings("HallamLab", "MetaPathways");
     return settings.value(key);
}
