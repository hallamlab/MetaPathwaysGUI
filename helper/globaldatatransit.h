#ifndef GLOBALDATATRANSIT_H
#define GLOBALDATATRANSIT_H

#include <QList>

#include "helper/types.h"
#include "datamodel/datamodeltypes.h"


class GlobalDataTransit
{
public:
    static GlobalDataTransit *getGlobalDataTransit();

    QList<ORF *> orfList;

private:
    static GlobalDataTransit *globalDataTransit;
    GlobalDataTransit();
};

#endif // GLOBALDATATRANSIT_H
