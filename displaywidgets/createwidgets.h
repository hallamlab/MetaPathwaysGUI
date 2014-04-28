#ifndef CREATEWIDGETS_H
#define CREATEWIDGETS_H
#include <QTableView>
#include <QHash>
#include "datamodel/rundatastats.h"
#include "datamodel/htabledata.h"
#include "datamodel/datamodeltypes.h"

class CreateWidgets
{

    static RunDataStats *rundatastats;
    static QTableView *statTableView;
    static QHash<ATTRTYPE, HTableData *> *htables;
public:
    CreateWidgets();
    static RunDataStats *getRunDataStats();
    static QTableView *getStatsTableView() ;
    static HTableData *getHtableData(ATTRTYPE);

};

#endif // CREATEWIDGETS_H
