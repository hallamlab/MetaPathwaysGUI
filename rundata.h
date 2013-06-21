#ifndef RUNDATA_H
#define RUNDATA_H

#include "qhash.h"
#include "qstring.h"

class RunData
{
public:
    RunData();
    RunData(QHash<QString,QString> *PARAMS,
            QHash<QString,QString> *CONFIG,
            QHash<QString,QString> *CONFIG_MAPPING);

    QHash<QString,QString>* getParams();
    QHash<QString,QString>* getConfig();
    QHash<QString,QString>* getConfigMapping();

private:
    QHash<QString,QString> *PARAMS;
    QHash<QString,QString> *CONFIG;
    QHash<QString,QString> *CONFIG_MAPPING;

};


#endif // RUNDATA_H
