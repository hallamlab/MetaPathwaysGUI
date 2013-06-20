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

