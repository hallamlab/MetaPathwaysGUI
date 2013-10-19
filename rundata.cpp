#include "rundata.h"
/*
 * This class contains the data for a run. It is an internal object class that represents all the data
 * about this instance of MetaPathways running.
 */

const QString RunData::TEMPLATE_PARAM = "template_param.txt";
const QString RunData::DEFAULT_TEMPLATE_PARAM = "default_template_param.txt";
const QString RunData::TEMPLATE_CONFIG = "template_config.txt";
const QString RunData::DEFAULT_TEMPLATE_CONFIG = "default_template_config.txt";
RunData* RunData::runData = 0;


RunData::RunData(){}

RunData* RunData::getRunData(){
    if (runData ==0){
        runData = new RunData();

        runData->initVartoNULL();
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
    if( type==_CONFIG)
        this->CONFIG[key] = value;
    if(type==_PARAMS)
        this->PARAMS[key]=value;
}

void RunData::initVartoNULL() {

    this->run =0;
    //this->rrnaDBS =0;
   // this->annotationDBS =0;

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

void RunData::setProcess(QProcess* run){
    this->run = run;
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

QProcess* RunData::getProcess(){
    return this->run;
}



void RunData::setPythonExecutablePath(QString pythonPath) {
    this->pythonPath = pythonPath;
}

void RunData::setPerlExecutablePath(QString perlPath) {
    this->perlPath  = perlPath;
}

void RunData::setMetaPathwaysPath(QString mpPath) {
    this->pathMetaPathways = mpPath;
}

void RunData::setDatabasesPath(QString databasePath) {
    this->databasePath  = databasePath;
}

/*
 * Should setup settings.
 * Usually this lives in a template_config file. So if it doesn't exist, we'll have to create it.
 */


void RunData::setupDefaultConfig(){
    std::ifstream config_file((this->pathMetaPathways + "/" + this->TEMPLATE_CONFIG).toStdString().c_str());
    if (!config_file){
        std::ifstream defaultConfig((QString(":/text/")  + "/" + this->DEFAULT_TEMPLATE_CONFIG).toStdString().c_str());
        std::ofstream configFile;

        configFile.open((this->pathMetaPathways + "/" + this->TEMPLATE_CONFIG).toStdString().c_str());
        configFile << defaultConfig.rdbuf();
        configFile.close();
    }

   // MainFrame::CONFIG = Utilities::parseFile(this->pathMetaPathways + "/" +TEMPLATE_CONFIG);
}


/*
 * Should setup config file.
 * If it doesn't exist, it will copy over a default template.
 * In both instances, a default parameter settings list will be loaded.
 */
void RunData::setupDefaultParams(){
    std::ifstream ifile((this->pathMetaPathways + "/" + this->TEMPLATE_PARAM).toStdString().c_str());
    if (!ifile){
        std::ofstream param_config;
        std::ifstream defaultConfig((QString(":/text/")  + this->DEFAULT_TEMPLATE_PARAM).toStdString().c_str());

        param_config.open((this->pathMetaPathways + "/" + this->TEMPLATE_PARAM).toStdString().c_str());
        param_config << defaultConfig.rdbuf();
        param_config.close();
    }

}

bool RunData::checkParams(){
    std::ifstream param_file( (this->pathMetaPathways + "/" + this->TEMPLATE_PARAM).toStdString().c_str());
//    qDebug() << "param file " << (this->pathMetaPathways + "/" + this->TEMPLATE_PARAM).toStdString().c_str();
    if (param_file) return true;
    else return false;
}

bool RunData::checkConfig(){
  //  qDebug() << " config file ";
    std::ifstream config_file( (this->pathMetaPathways + "/" +this->TEMPLATE_CONFIG).toStdString().c_str());

    if (config_file) return true;
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
        warningMsg = warningMsg + " -MetaPathways path\n";
        correct = false;
    }

    return correct;
}
