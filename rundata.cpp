#include "rundata.h"
/*
 * This class contains the data for a run. It is an internal object class that represents all the data
 * about this instance of MetaPathways running.
 */

RunData* RunData::runData = 0;

RunData::RunData(){}

RunData* RunData::getRunData(){
    if (runData ==0)
        runData = new RunData();

    return runData;
}

void RunData::setParams(QHash<QString,QString>* PARAMS){
    this->PARAMS = PARAMS;
}

void RunData::setConfig(QHash<QString,QString>* CONFIG){
    this->CONFIG = CONFIG;
}

void RunData::setConfigMapping(QHash<QString,QString>* CONFIG_MAPPING){
    this->CONFIG_MAPPING =  CONFIG_MAPPING;
}

void RunData::setProcess(QProcess* run){
    this->run = run;
}

void RunData::setRRNADBS(QStringList* rrnaDBS){
    this->rrnaDBS = rrnaDBS;
}

void RunData::setAnnotationDBS(QStringList* annotationDBS){
    this->annotationDBS = annotationDBS;
}

QHash<QString,QString>* RunData::getParams(){
    return this->PARAMS;
}

QHash<QString,QString>* RunData::getConfig(){
    return this->CONFIG;
}

QHash<QString,QString>* RunData::getConfigMapping(){
    return this->CONFIG_MAPPING;
}

QProcess* RunData::getProcess(){
    return this->run;
}




