#include "createwidgets.h"

RunDataStats *CreateWidgets::rundatastats = 0 ;
QTableView *CreateWidgets::statTableView = 0;
QHash<ATTRTYPE, HTableData *> *CreateWidgets::htables =0;

RunDataStats *CreateWidgets::getRunDataStats(){
    if(CreateWidgets::rundatastats == 0)
        CreateWidgets::rundatastats =  new RunDataStats;
    return CreateWidgets::rundatastats;
}


QTableView *CreateWidgets::getStatsTableView() {
    if(CreateWidgets::statTableView != 0) {
       delete  CreateWidgets::statTableView;
       CreateWidgets::statTableView = 0;
    }

    if(CreateWidgets::statTableView == 0) {
        CreateWidgets::statTableView =  new QTableView;
        CreateWidgets::statTableView->horizontalHeader()->setStretchLastSection(true);
    }
    return CreateWidgets::statTableView;
}

HTableData *CreateWidgets::getHtableData(ATTRTYPE attrtype) {
    if( htables==0 )
        htables  = new QHash<ATTRTYPE, HTableData *>();

    if(! htables->contains(attrtype))
        htables->insert(attrtype, new HTableData);
    return htables->value(attrtype);
}

CreateWidgets::CreateWidgets()
{
}
