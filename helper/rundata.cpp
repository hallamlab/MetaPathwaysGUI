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

   // QMessageBox::warning(0, QString("reading "), settings.value("METAPATHWAYS_PATH").toString());
    this->setValue(QString("METAPATHWAYS_PATH"), settings.value("METAPATHWAYS_PATH").toString(), _CONFIG);
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
QString RunData::getValueFromHash(QString key, SETTING_TYPE type){
    if (type==_CONFIG)
        return this->CONFIG.contains(key)  ?  this->CONFIG.value(key): QString("");
    if (type==_PARAMS)
        return this->PARAMS.contains(key)  ?  this->PARAMS.value(key): QString("");
    if (type==_PARAMS_DEFAULT)
        return this->PARAMS_DEFAULT.contains(key)  ?  this->PARAMS_DEFAULT.value(key): QString("");
    if (type==_TEMP_CONFIG)
        return this->TEMP_CONFIG.contains(key)  ?  this->TEMP_CONFIG.value(key): QString("");
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

void RunData::setConfig(QHash<QString,QString> TEMP_CONFIG){
    this->CONFIG = TEMP_CONFIG;
}

void RunData::triggerParameterFileRead() {
    emit loadParameters();
}


void RunData::setTempConfig(QHash<QString,QString> CONFIG){
    this->TEMP_CONFIG = CONFIG;
}

void RunData::setConfigMapping(QHash<QString,QString> CONFIG_MAPPING){
    this->CONFIG_MAPPING =  CONFIG_MAPPING;
}

void RunData::setProcess(QProcess *process){
    this->process = process;
}

bool RunData::checkPGDBFolderPath(QString &message, QString pgdbFolderPath) {
    bool error = false;

    QDir dir( pgdbFolderPath);

    if(!dir.exists()) {
        message = QString("Folder ") + pgdbFolderPath + QString(" does not exist!");
        return false;
    }


    if( !pgdbFolderPath.contains(QString("user"), Qt::CaseInsensitive) || !pgdbFolderPath.contains(QString("pgdb") ,Qt::CaseInsensitive)) {
        message = QString("Folder ") + pgdbFolderPath + QString(" does not appear to be a PGDB folder!");
        return false;
    }

    message = QString("PGDB folder") + pgdbFolderPath + QString(" found!");
    return !error;

}

bool RunData::checkPathwayToolsExecutable(QString &message, QString executable) {
    bool error = false;

    QFileInfo file(executable);
    if(!file.exists()) {
        message = QString("Pathway-Tools executable ") + executable + QString(" does not exist!");
        return false;
    }

    message = QString("Pathway-Tools executable ") + executable + QString(" found!");
    return !error;

}




bool RunData::checkRefDatabasesPath(QString &message, QString refdbsDir) {

    bool error = false;

    QDir dir(refdbsDir);

    if(!dir.exists()) {
        message = QString("Folder ") +refdbsDir + QString(" does not exist!");
        return false;
    }

    QStringList expectedSubFolders;
    expectedSubFolders << "functional" << QString("functional") + QDir::separator() + QString("formatted") <<\
                          "taxonomic" << QString("taxonomic") + QDir::separator() + QString("formatted") << "ncbi_tree" << "functional_categories";

    message="";
    foreach(QString subfolder, expectedSubFolders) {
        dir.setPath( refdbsDir + QDir::separator() + subfolder);
        if( !dir.exists()) {
            message += QString("Expected folder : ") + dir.absoluteFilePath(refdbsDir + QDir::separator() + subfolder) + QString( " not found\n");
            error = true;
        }
    }

    message = QString("Referenced database folder ") + refdbsDir + QString(" found!");
    return !error;
}


bool RunData::checkMetaPathwaysPath(QString &message, QString metapathwaysDir) {

    bool error = false;
    QDir dir(metapathwaysDir);

    if(!dir.exists()) {
        message = QString("Folder ") + metapathwaysDir + QString(" does not exist!");
        return false;
    }

    QStringList expectedSubFolders;
    expectedSubFolders << "libs" << "executables" << "utilities" ;

    message="";
    foreach(QString subfolder, expectedSubFolders) {
        dir.setPath( metapathwaysDir + QDir::separator() + subfolder);
        if( !dir.exists()) {
            message += QString("Expected folder : ") + dir.absoluteFilePath(metapathwaysDir + QDir::separator() + subfolder) + QString( " not found\n");
            error = true;
        }
    }

    if(!error) {
       message = QString("MetaPathways Python code folder ") + metapathwaysDir + QString(" found!");
    }
    return !error;
}



/**
 * @brief RunData::checkPythonBinary, checks if the python executable is working
 * @param message
 * @return
 */
bool RunData::checkPythonBinary(QString &message, QString executable) {
    bool error = false;

    QFileInfo file(executable);
    if(!file.exists()) {
        message = QString("Python executable ") + executable + QString(" does not exist!");
        return false;
    }

    QProcess binaryTester;
    binaryTester.start(executable + QString(" -h") );
    binaryTester.waitForStarted();
    binaryTester.closeWriteChannel();
    binaryTester.waitForFinished();

    int exitCode=  binaryTester.exitCode();

    if( exitCode!=0) {
        message=QString("INVALID BINARY");
        error= true;
    }
    else {
        message = QString("VALID BINARY");
    }

    return !error;

}


/**
 * @brief RunData::checkBinaries, checks the binaries in the executables folder for the OS
 *  reports errors if it finds any missing, unspecified or invalid binaries
 * @param executablesDir
 */
bool RunData::checkBinaries(QString &message, QString executablesDir) {

    bool error = false;

    QDir dir(executablesDir);
    if(!dir.exists()) {
        message = QString("Folder ") + executablesDir + QString(" does not exist!");
        return false;
    }


    QHash<QString, QStringList> binaries;


    binaries["LASTDB_EXECUTABLE"] = (QStringList() << "-h");
    binaries["LAST_EXECUTABLE"] = (QStringList() << "-h");
    binaries["FORMATDB_EXECUTABLE"] = QStringList("-help");

    binaries["BLASTP_EXECUTABLE"] = QStringList("-h");
    binaries["BLASTN_EXECUTABLE"] = QStringList("-h");

    binaries["PRODIGAL_EXECUTABLE"]= (QStringList() << "-h");
    binaries["SCAN_tRNA_EXECUTABLE"] =QStringList("-h");

#define RPKM_EXECUTABLE
#ifndef RPKM_EXECUTABLE
    binaries["RPKM_EXECUTABLE"] = QStringList("-h");
#endif

    QProcess binaryTester;

    QHash<QString, QString> status;

    foreach(QString name, binaries.keys() ) {
        QString executable = this->getValueFromHash(name, _CONFIG);

        if( executable.isEmpty() ) {
            status[name] = QString("BINARY UNSPECIFIED");
            error = true;
            continue;
        }

        QFileInfo file;
        file.setFile(executablesDir + QDir::separator() + executable);
        if( !file.exists() ) {
            status[name] = QString("BINARY MISSING");
            error = true;
            continue;
        }

        binaryTester.start(executablesDir + QDir::separator() + executable, binaries[name]);
        binaryTester.waitForStarted();
        binaryTester.closeWriteChannel();
        binaryTester.waitForFinished();

        int exitCode=  binaryTester.exitCode();

        if( exitCode!=0) {
            status[name]=QString("INVALID BINARY");
            error= true;
            continue;
        }

        status[name]=QString("VALID BINARY");
    }

    message = "";

    if( error ) {
        message += QString("FOLDER : ") + executablesDir + "\n";
        message += QString("FIX    : Please correct the location for \"OS Specific Executables\" in the Setup tab\n");
        message += QString("       : Alternatively,  you can update the EXECUTABLES_DIR key in the config file \"config/template_config.txt\"\n\n" );
    }


    foreach(QString name, status.keys()) {
        message +=  name +  "  :  " +   status[name]  + "\n";
    }


    return !error;

}

/**
 * @brief RunData::getProcessEnvironment, returns the environment for the process for the libs
 * @param METAPATH
 * @return
 */

QProcessEnvironment RunData::getProcessEnvironment(QString METAPATH) {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("CURRDIR", QDir::toNativeSeparators(METAPATH));
    env.insert("METAPATH", QDir::toNativeSeparators(METAPATH));
    env.insert("METAPATHLIB", QDir::toNativeSeparators(METAPATH + QDir::separator() + "libs"));
   // env.insert("STARCLUSTERLIB", QDir::toNativeSeparators(METAPATH + "libs" + QDir::separator() + "starcluster"));
    env.insert("PYTHONPATH", QDir::toNativeSeparators(METAPATH + QDir::separator() +  "libs:" + METAPATH + QDir::separator() + "libs" + QDir::separator() + "starcluster"));
    return env;
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

QHash<QString,QString> RunData::getTempConfig(){
    return this->TEMP_CONFIG;
}

QHash<QString,QString> RunData::getConfigMapping(){
    return this->CONFIG_MAPPING;
}



void RunData::setPythonExecutablePath(QString pythonPath) {
    this->CONFIG["PYTHON_EXECUTABLE"] = pythonPath;
}

void RunData::setPgdbFolderPath(QString pgdbFolderPath) {
    this->CONFIG["PGDB_FOLDER"]  = pgdbFolderPath;
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

   // qDebug() << "parsed results " << parsedResults;
    this->setConfig(parsedResults);
    this->setTempConfig(parsedResults);

}


void RunData::loadDefaultParams(){
    QString path = QString(":/text/")  + this->DEFAULT_TEMPLATE_PARAM;
    this->PARAMS_DEFAULT = Utilities::parseFile(path,"PARAMS");
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


/**
 * @brief RunData::addToProcessedSamples, sets the sample name that is currently or last running
 * @param currentSample
 */
void RunData::addToProcessedSamples(QString sampleName) {
    this->processedSamples[sampleName]= true;
}

/**
 * @brief RunData::clearProcessedSamples, pretends the processed samples
 */
void RunData::clearProcessedSamples() {
    this->processedSamples.clear();
}

/**
 * @brief RunData::numberOfProcessedSamples, pretends the processed samples
 */
int RunData::numberOfProcessedSamples() {
    return this->processedSamples.size();
}




/**
 * @brief RunData::isAlreadyProcessedSample, checks if the sample is already  in the list of processed sample
 * @param sampleName
 * @return
 */
bool RunData::isAlreadyProcessedSample(QString sampleName) {
    return this->processedSamples.contains(sampleName);
}

/**
 * @brief RunData::setCurrentSample, set the current sample  to show the status from
 * @param currentSample
 */
void RunData::setCurrentSample(QString currentSample) {
    this->currentSample = currentSample;
}
/**
 * @brief RunData::getCurrentSample, gets the current sample running from
 * @return  currentSample name
 */
QString RunData::getCurrentSample() {
    return this->currentSample;
}

/**
 * @brief RunData::getSampleFolder, get the folder that contains the sample
 * @param sampleName
 * @return
 */
QString RunData::getSampleFolder(QString sampleName) {
    if(! this->outputFolders.contains(sampleName)) return QString();

    return this->outputFolders[sampleName];

}

/**
 * @brief RunData::getSampleFolderMap, retuns the sample to the folder map
 * @return
 */
QHash<QString, QString> RunData::getSampleFolderMap() {

    return this->outputFolders;
}


/**
 * @brief RunData::getCurrentInputFormat gets the current input format, such as, fasta, gbk, etc
 * @return
 */
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

/**
 * @brief RunData::getNumRRNADB, gets the number of rRNA gene databases used
 * @return
 */
unsigned int RunData::getNumRRNADB(){
    return this->nRRNADB;
}

unsigned int RunData::getNumADB(){
    return this->nADB;
}

/**
 * @brief RunData::getrNADBNames, gets the name of the rRNA SSU or LSU databese names
 * @return  name of the database
 */
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

/**
 * @brief RunData::getFileList, returns the list of sample or input file list
 * @param fileType
 * @return
 */
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
void RunData::loadInputFiles(const QString &folder){
//    QDir currentDir(this->getParams()["fileInput"]);

    QDir currentDir(folder);

    if( !currentDir.exists() ) {
        Utilities::showInfo(0, QString("Input folder ") + folder + QString(" not found!")) ;
        return;
    }
    currentDir.setFilter(QDir::Files);
    QStringList entries = currentDir.entryList();

    QList<QRegExp> regList;

    // for fasta type only
    regList << QRegExp("[.][fF][aA][sS][tT][aA]$") << QRegExp("[.][fF][aA]$") << QRegExp("[.][fF][aA][aA]$") << QRegExp("[.][fF][aA][sS]") << QRegExp("[.][fF][nN][aA]$") << QRegExp("[.][gG][bB][kK]$");


    QStringList filesDetected;
    foreach( QString entry, entries )
    {
        QString temp = entry;
        QStringList file = temp.split(".");

        QString suffix = QString(".") + file.last();

     //   if (fileType.compare(QString("fasta"))== 0){
            foreach(QRegExp reg, regList ) {
               if(suffix.indexOf(reg,0) != -1 ) {
                   filesDetected.append( temp.remove(reg) );
                   break;
               }
            }
    }


    this->setFileList(filesDetected);
}


void RunData::setResultFolders(QStringList folders) {

    this->resultFolders = folders;

}

/**
 * @brief RunData::getOutputFolders, returns the list of outputfolders
 * @return list of output folders
 */
QStringList RunData::getOutputFolders() {

   if(this->outputFolders.isEmpty()) this->loadOutputFolders();

   QStringList tempList = this->outputFolders.keys();
   qSort(tempList.begin(), tempList.end());
   return tempList;
}


/**
 * @brief RunData::loadOutputFolders, loads the list output folders in the
 * outoutFolder variable.
 */
void RunData::loadOutputFolders(){

    this->outputFolders.clear();

    foreach(QString folder, this->resultFolders) {
        QDir currentDir(folder);

        if( !currentDir.exists() ) return;

        QStringList sampleFolders = currentDir.entryList(QDir::AllDirs);

        foreach(QString sampleFolder, sampleFolders) {
            if( sampleFolder.compare(QString(".")) ==0  || sampleFolder.compare(QString("..")) ==0 ) continue;

            if( this->isOutputFolderValid(currentDir.absolutePath() + QDir::separator() +  sampleFolder )) {
                this->outputFolders.insert(sampleFolder, folder);
            }
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
  //  qDebug()<< "ABCD selection " << selection;
   this->selectSamplesToRun = selection;
   qSort(this->selectSamplesToRun.begin(), this->selectSamplesToRun.end());
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


/**
 * @brief RunData::setDirtyBit, sets the dirty bit
 * @param key
 */
void RunData::setDirtyBit(QString key) {
    this->dirtybits[key] = true;
}


/**
 * @brief RunData::clearDirtyBit, clears the dirty bit
 * @param key
 */
void RunData::clearsDirtyBit(QString key) {
    if( this->dirtybits.contains(key)) this->dirtybits[key] = false;
}

/**
 * @brief isBitDirty, is the bit dirty
 * @return
 */
bool RunData::isBitDirty(QString key) {
    if(!this->dirtybits.contains(key)) return false;
    return this->dirtybits[key];
}


/**
 * @brief RunData::saveContext, Saves the context against the key
 * @param key, the key
 * @param value, the value against the key
 * @return
 */
bool RunData::saveContext(const QString &key, QVariant value) {
     QSettings settings("HallamLab", "MetaPathways");
     settings.setValue(key, value);
     return true;
}

/**
 * @brief RunData::hasContext Checks if the Qsettings has a context by the name of the key
 * @param key : key for the value
 * @return true
 */
bool RunData::hasContext(const QString &key) {
     QSettings settings("HallamLab", "MetaPathways");
     if( settings.value(key) == QVariant() ) return false;
     return true;
}

bool RunData::createContext(const QString &key) {
     QSettings settings("HallamLab", "MetaPathways");
     settings.setValue(key, QString(""));
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
