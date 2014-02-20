#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H
#include "dataviews/largetable.h"


typedef struct _TABLEACCESSINFO {
    LargeTable *table;
    unsigned int accessed ;
    QString fileName;

} TABLEACCESSINFO;

typedef struct _GRAPHDATA {
    QVector<double> x;
    QVector<double> y;
    double ymax, ymin, xmax, xmin;
    unsigned int numBuckets;
    double bucketSize;
    QString fileName;
    unsigned int accessed;
} GRAPHDATA;


class TableManager
{

public:
    static TableManager *getTableManager();

    LargeTable *getTable(QString fileName);
    bool setTable(QString fileName, LargeTable *table);
    QString getTableToDelete();
    void deleteAllTables() ;
    void deleteTable(QString fileName);

    GRAPHDATA *getGraphData(QString fileName);
    bool setGraphData(QString fileName, GRAPHDATA *graphData);
    QString getGraphDataToDelete();

    void deleteGraphData(QString fileName);
    void deleteAllGraphData() ;

private:
    TableManager();
    static TableManager *tableManager;
    unsigned int timeAccessed;
    QHash<QString, TABLEACCESSINFO> tables;
    unsigned int maxSize;

    QHash<QString, GRAPHDATA *> graphData;



};


#endif // TABLEMANAGER_H
