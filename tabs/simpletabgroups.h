#ifndef SIMPLETABGROUPS_H
#define SIMPLETABGROUPS_H
#include <QHash>
#include <QString>

#include "dataviews/tabledata.h"




class SimpleTabGroups
{
public:
    SimpleTabGroups();

    bool tableExists(QString groupName, QString fileName);


    void addTable(TableData *t, const QString &groupName, const QString &fileName);
    TableData * getTable(const QString &groupName, const QString &fileName);


    void removeAGroupExcept(const QString &groupName ) ;
    void removeGroup(const QString &groupName);


private:
    QHash<QString, QHash< QString, TableData *> *> simpleTableGroups;
    unsigned int maxSize, count ;

};

#endif // SIMPLETABGROUPS_H
