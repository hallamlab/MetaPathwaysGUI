#ifndef CREATEWIDGETS_H
#define CREATEWIDGETS_H
#include <QTableView>
#include <QHash>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

#include "datamodel/rundatastats.h"
#include "datamodel/htabledata.h"
#include "datamodel/datamodeltypes.h"
#include "dataviews/statsqgroupbox.h"

class CreateWidgets: public QWidget
{
    Q_OBJECT

    static RunDataStats *rundatastats;
    static QTableView *statTableView;
    static StatsQGroupBox *statTableCompleteView;

    static QHash<ATTRTYPE, HTableData *> *htables;
public:
    CreateWidgets();
    static RunDataStats *getRunDataStats();
    static QTableView *getStatsTableView() ;
    static HTableData *getHtableData(ATTRTYPE);
    static StatsQGroupBox *getStatsTableCompleteView();

};

#endif // CREATEWIDGETS_H
