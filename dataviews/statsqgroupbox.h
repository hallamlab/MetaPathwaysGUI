#ifndef STATSQGROUPBOX_H
#define STATSQGROUPBOX_H

#include <QGroupBox>
#include <QTableView>
#include <QDebug>

#include "datamodel/rundatastats.h"

class StatsQGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit StatsQGroupBox(QWidget *parent = 0);
    void setModel(RunDataStats *model);

private:
    RunDataStats *model;
public:
    QTableView *statsTableView;
signals:
    
public slots:
   void exportTable();
    
};

#endif // STATSQGROUPBOX_H
