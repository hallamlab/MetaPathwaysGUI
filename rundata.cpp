#include "rundata.h"

RunData::RunData(){
    this->PARAMS = NULL;
    this->CONFIG = NULL;
    this->CONFIG_MAPPING = NULL;
}

RunData::RunData(QHash<QString,QString> *PARAMS,
                 QHash<QString,QString> *CONFIG,
                 QHash<QString,QString> *CONFIG_MAPPING)
{
    this->PARAMS = PARAMS;
    this->CONFIG = CONFIG;
    this->CONFIG_MAPPING = CONFIG_MAPPING;
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
