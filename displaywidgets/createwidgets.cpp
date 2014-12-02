#include "createwidgets.h"

RunDataStats *CreateWidgets::rundatastats = 0 ;
QTableView *CreateWidgets::statTableView = 0;
StatsQGroupBox *CreateWidgets::statTableCompleteView = 0;

QHash<ATTRTYPE, HTableData *> *CreateWidgets::htables =0;

RunDataStats *CreateWidgets::getRunDataStats(){
    if(CreateWidgets::rundatastats == 0)
        CreateWidgets::rundatastats =  new RunDataStats;
    return CreateWidgets::rundatastats;
}


QTableView *CreateWidgets::getStatsTableView() {
    if(CreateWidgets::statTableView != 0) {
        //qDebug() << "Fix This : Handle deallocation carefully ";
      //  delete  CreateWidgets::statTableView;
      // CreateWidgets::statTableView->setModel(0);
       CreateWidgets::statTableView = 0;
    }

    if(CreateWidgets::statTableView == 0) {
        CreateWidgets::statTableView =  new QTableView;

        CreateWidgets::statTableView->horizontalHeader()->setStretchLastSection(true);
        CreateWidgets::statTableView->setAlternatingRowColors(true);

#ifdef SECTION
        statTableView->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
        statTableView->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
#else
        statTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
        statTableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
        statTableView->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

    }
    return CreateWidgets::statTableView;
}

StatsQGroupBox *CreateWidgets::getStatsTableCompleteView() {
    if(CreateWidgets::statTableCompleteView != 0) {
       delete  CreateWidgets::statTableCompleteView;
       CreateWidgets::statTableCompleteView = 0;
    }

    if(CreateWidgets::statTableCompleteView == 0) {
        CreateWidgets::statTableCompleteView =  new StatsQGroupBox;
    }

    // create export button
    QPushButton *exportButton = new QPushButton("Export");
    exportButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    // spacer next to the button
    QSpacerItem *spaceitem = new QSpacerItem(200, 20,QSizePolicy::Expanding, QSizePolicy::Maximum);

    //create the layout
    QHBoxLayout *hboxlayout = new QHBoxLayout;
    hboxlayout->addItem(spaceitem);
    hboxlayout->addWidget(exportButton);

    // create the export and spacer item holder groupbox
    QGroupBox *exportButtonBox = new QGroupBox();
    exportButtonBox->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    exportButtonBox->setStyleSheet("QGroupBox{border:0px solid gray;border:0px;margin: 0ex;}");
    exportButtonBox->setLayout(hboxlayout);


    //create the table
    QTableView *statTableView = CreateWidgets::getStatsTableView();
    statTableView->setStyleSheet("QTableView{border:0px solid gray;border:0px;margin: 0ex;}");

    // create the entire layout to hold
    QVBoxLayout *vboxlayout = new QVBoxLayout;
    statTableView->setStyleSheet("QGroupBox{border:0px solid gray;border-radius:0px;margin-top: 0ex;}");

    vboxlayout->addWidget(statTableView);
    vboxlayout->addWidget(exportButtonBox);

    CreateWidgets::statTableCompleteView =  new StatsQGroupBox;
    CreateWidgets::statTableCompleteView->statsTableView =statTableView;
    CreateWidgets::statTableCompleteView->setLayout(vboxlayout);
    CreateWidgets::statTableCompleteView->setStyleSheet("QGroupBox{border:0px solid gray;border-radius:0px;margin-top: 0ex;}");

    connect(exportButton, SIGNAL(clicked()), CreateWidgets::statTableCompleteView, SLOT(exportTable()));
    return CreateWidgets::statTableCompleteView;
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
