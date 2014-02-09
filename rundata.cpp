#include "rundata.h"
#include <QMessageBox>
/*
 * This class contains the data for a run. It is an internal object class that represents all the data
 * about this instance of MetaPathways running.
 */

const QString RunData::TEMPLATE_PARAM = "template_param.txt";
const QString RunData::DEFAULT_TEMPLATE_PARAM = "default_template_param.txt";
const QString RunData::TEMPLATE_CONFIG = "template_config.txt";
const QString RunData::DEFAULT_TEMPLATE_CONFIG = "default_template_config.txt";
RunData* RunData::runData = 0;


RunData::RunData(){
    CONFIG.insert("METAPATHWAYS_PATH", QString());
    CONFIG.insert("PYTHON_EXECUTABLE", QString());
    CONFIG.insert("PERL_EXECUTABLE", QString());
    CONFIG.insert("PATHOLOGIC_EXECUTABLE", QString());
    CONFIG.insert("REFDBS", QString());
    CONFIG.insert("EXECUTABLES_DIR",QString());
}

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

QString RunData::getValueFromHash(QString key,SETTING_TYPE type){
    if (type==_CONFIG)
        return this->CONFIG.contains(key)  ?  this->CONFIG[key]: QString("");
    if (type==_PARAMS)
        return this->PARAMS.contains(key)  ?  this->PARAMS[key]: QString("");
}

void RunData::setValue(QString key, QString value, SETTING_TYPE type){
    if(type==_CONFIG) {
        this->CONFIG[key] = value;
    }
    if(type==_PARAMS)
        this->PARAMS[key]=value;
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

QProcess *RunData::getProcess(){
    return this->process;
}

void RunData::setPythonExecutablePath(QString pythonPath) {
    this->CONFIG["PYTHON_PATH"] = pythonPath;
}

void RunData::setPerlExecutablePath(QString perlPath) {
    this->CONFIG["PERL_PATH"]  = perlPath;
}

void RunData::setMetaPathwaysPath(QString mpPath) {
    this->CONFIG["METAPATHWAYS_PATH"] = mpPath;
}

void RunData::setDatabasesPath(QString databasePath) {
    this->CONFIG["REFDBS"] = databasePath;
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
 * Setups default configs. We can assume sometimes the exe may have been moved to a different location rather than as a sibling file
 * with the metapathways python code. Therefore, sometimes there may be a case where on initial startup, there are no config or param files
 * since the METAPATHWAYS_PATH will not be set. So we should then generate template configs and param files for wherever the executable sits to
 * alleviate this ambiguitity. Why are we even doing this?
 */
void RunData::setupDefaultConfig(){

    bool reWriteConfig = false;
    QString path = this->CONFIG["METAPATHWAYS_PATH"];
    // qDebug() << "path with CONFIG is " << path;
    if (path.isEmpty()) path = QDir::currentPath();
    // qDebug() << "path with CURRENT PATH IS is " << path;
    QFileInfo config_file( path + "/" + this->TEMPLATE_CONFIG);
    // qDebug() << "config file " << config_file.exists();

    QStringList atrList;
    atrList<< "METAPATHWAYS_PATH" << "PYTHON_EXECUTABLE" << "PERL_EXECUTABLE" << "REFDBS" << "PATHOLOGIC_EXECUTABLE" << "SYSTEM";

    if (config_file.exists()){
        QHash<QString, QString> config = Utilities::parseFile(config_file.filePath());
      //  qDebug() << "(" << this->CONFIG["METAPATHWAYS_PATH"] <<")"  << "(" <<  config["METAPATHWAYS_PATH"] <<")";

        foreach( QString atr, atrList ) {
           if(this->CONFIG[atr].compare(config[atr])!=0 ){
               reWriteConfig = true;
             //    qDebug() << "Xxxxxxxxxxxxxxxxxxxxxx  metapathways dir true";
           }
        }
        if(!reWriteConfig) this->CONFIG = config;

        /*
        qDebug() << this->CONFIG;
        qDebug()<< "\n--------------------\n";
        qDebug()<< config;
        */

    }
    else {
        reWriteConfig = true;
    }

    // if we need to reWrite the config
    if( reWriteConfig ){
        QFile outFile( path + "/" + this->TEMPLATE_CONFIG);
        QFile inputFile(QString(":/text/")  + "/" + this->DEFAULT_TEMPLATE_CONFIG);

        if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
            QTextStream out(&outFile);

            if (inputFile.open(QIODevice::ReadOnly) ) {
                QTextStream in(&inputFile);
                //qDebug() << "writing config, input and output files could be opened";
                while ( !in.atEnd() )  {
                    QString line = in.readLine().trimmed();
                    bool matched = false;
                    foreach( QString atr, atrList ) {
                       if( line.indexOf(atr) != -1) {
                           if(!this->CONFIG.contains(atr) || this->CONFIG[atr].length()==0 ) {
                               if(line.indexOf("METAPATHWAYS_PATH") != -1) {
                                  this->CONFIG["METAPATHWAYS_PATH"] = path;
                               }
                               else {
                                   this->CONFIG[atr] = Utilities::extractAttribute(line);
                                 //  qDebug() << "AAA " << line;
                                 //  qDebug() << "ZZZZZZZZZZZ " << atr << "  " << this->CONFIG[atr];
                               }
                           }
                           out << atr <<"  " <<  CONFIG[atr] << "\n";
                           matched = true;
                           break;
                       }
                    }
                    if( matched==false) {
                        out << line << "\n";
                    }

                }
                inputFile.close();
            }
            outFile.close();
         //   qDebug() << "both file handlers closed, wrote to " << path + "/" + this->TEMPLATE_CONFIG;
        }

//        QMessageBox::warning(0,"title","infin loop in setup default config",QMessageBox::Ok);

        setupDefaultConfig();
    }
   // MainFrame::CONFIG = Utilities::parseFile(this->pathMetaPathways + "/" +TEMPLATE_CONFIG);
}


/*
 * Should setup config file.
 * If it doesn't exist, it will copy over a default template.
 * In both instances, a default parameter settings list will be loaded.
 */
void RunData::setupDefaultParams(){
    QString path = this->CONFIG["METAPATHWAYS_PATH"];
    if (path.isEmpty()) path = QDir::currentPath();
    QFileInfo defaultParamFile( QString(":/text/")  + "/" + this->DEFAULT_TEMPLATE_PARAM);
    QFileInfo paramFile( path + "/" + this->TEMPLATE_PARAM);
    bool reWriteParam = false;
    if( paramFile.exists() ) {
        QHash<QString, QString> params = Utilities::parseFile(paramFile.filePath());
        QHash<QString, QString> defaultParams = Utilities::parseFile(defaultParamFile.filePath());

        this->PARAMS = params;

        foreach( QString key, defaultParams.keys()) {
            if( !params.contains(key))  reWriteParam = true;
        }

    } else {
        reWriteParam = true;
        // qDebug() << "couldn't find file";

    }

    if (reWriteParam){
        QString path = this->CONFIG["METAPATHWAYS_PATH"];
        if (path.isEmpty()) path = QDir::currentPath();
        QFile outFile( path + "/" + this->TEMPLATE_PARAM);
        QFile inputFile(QString(":/text/")  + "/" + this->DEFAULT_TEMPLATE_PARAM);

        if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
            QTextStream out(&outFile);

            if (inputFile.open(QIODevice::ReadOnly) ) {
                QTextStream in(&inputFile);

                while ( !in.atEnd() )  {
                    QString line = in.readLine().trimmed();
                    out << line + "\n";
                }
                inputFile.close();
            }
            outFile.close();
        }
        setupDefaultParams();
    }
}
void RunData::setCurrentSample(QString currentSample) {
    this->currentSample = currentSample;
}

QString RunData::getCurrentSample() {
    return this->currentSample;
}


bool RunData::checkParams(){
    QFile paramFile( this->CONFIG["METAPATHWAYS_PATH"] + "/" + this->TEMPLATE_PARAM);

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
    QFile configFile(  this->CONFIG["METAPATHWAYS_PATH"] + "/" +this->TEMPLATE_CONFIG);

    if (configFile.exists()) return true;
    else return false;
}


bool RunData::validate(QString &warningMsg) {

    bool correct = true;
    QFileInfo file(this->getValueFromHash("PYTHON_EXECUTABLE", _CONFIG));

    if( !file.exists() ) {
        warningMsg = warningMsg + " -Python executable path\n";
        correct = false;
    }

    file.setFile(this->getValueFromHash("REFDBS", _CONFIG));
    if( !file.exists() ) {
        warningMsg = warningMsg + " -Database path\n";
        correct = false;
    }

    file.setFile(this->getValueFromHash("METAPATHWAYS_PATH", _CONFIG));
    QFileInfo  file1(this->getValueFromHash("METAPATHWAYS_PATH", _CONFIG) + "/MetaPathways.py");

    if( !file.exists() || !file1.exists() ) {
        warningMsg = warningMsg + " -MetaPathways.py is missing from your specified MetaPathways directory. Please verify it exists!\n";
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

        if (fileType == "fasta"){
            foreach(QRegExp reg, regList ) {
               if(temp.indexOf(reg,0) > -1 ) {
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

QString RunData::getSystem() {
    return this->system;
}
