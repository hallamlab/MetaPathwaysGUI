#ifndef SIMPLETABGROUPS_H
#define SIMPLETABGROUPS_H
#include <QHash>
#include <QString>

#include "dataviews/tabledata.h"
#include "dataviews/graphdata.h"


class TableData;
class GraphData;

template <class T>
class SimpleTabGroups
{
public:
    SimpleTabGroups<T>();

    bool tableExists(QString groupName, QString fileName);


    void addTable(T *t, const QString &groupName, const QString &fileName);
    T* getTable(const QString &groupName, const QString &fileName);


    void removeAGroupExcept(const QString &groupName ) ;
    void removeGroup(const QString &groupName);


private:
    QHash<QString, QHash< QString, T *> *> simpleTableGroups;
    unsigned int maxSize, count ;

};

/**
 * @brief SimpleTabGroups::SimpleTabGroups, constructor for the simpleTabGroups
 *  maxSize is the maximum numter of allowed tabs in total, across all groups
 *  count is a variable that refers to the current number of tables
 */
template <class T>
SimpleTabGroups<T>::SimpleTabGroups()
{
    this->maxSize  = 20;
    this->count = 0;
}


/**
 * @brief SimpleTabGroups::tableExists , check if table table exists in groupName, fileName
 * @param groupName, usually refers to the sample
 * @param fileName, refers to the inputfile
 * @return
 */
template <class T>
bool SimpleTabGroups<T>::tableExists(QString groupName, QString fileName) {
    if( !this->simpleTableGroups.contains(groupName )) return false;
    if( !this->simpleTableGroups[groupName]->contains(fileName)) return false;
    return true;
}

/**
 * @brief simpleTabGroups::addTable
 * @param t TableData to add
 * @param groupName, group of TableData, usually refers to a sample
 * @param fileName, name of the file the input comes from
 */
template <class T>
void SimpleTabGroups<T>::addTable(T *t, const QString &groupName, const QString &fileName) {

    if( this->maxSize < this->count)
        this->removeAGroupExcept(groupName);

    if( !this->simpleTableGroups.contains(groupName )) {
        this->simpleTableGroups[groupName] = new QHash<QString, T *>;
    }

    this->count++;
    this->simpleTableGroups[groupName]->insert(fileName, t);
}
/**
 * @brief simpleTabGroups::getTable, retrieves a ble from the groupName, loaded from the file fileName
 * @param groupName, corresponds to a sample
 * @param fileName, corresponds to a file
 * @return
 */
template <class T>
T *SimpleTabGroups<T>::getTable(const QString &groupName, const QString &fileName) {
    if( !this->simpleTableGroups.contains(groupName )) {
        return 0;
    }
    if( !this->simpleTableGroups[groupName]->contains(fileName) ) {
        return 0;
    }

    return this->simpleTableGroups[groupName]->value(fileName);
}

/**
 * @brief simpleTabGroups::removeAGroupExcept, removes any group other than groupName, until
 *        the count falls below the maxcount;
 * @param groupName, name of the group usually a sample
 */
template <class T>
void SimpleTabGroups<T>::removeAGroupExcept(const QString &groupName ) {
    foreach(QString candidateGroupToRemove, this->simpleTableGroups.keys() ) {
        if( candidateGroupToRemove!= groupName) {
            this->removeGroup(candidateGroupToRemove);
            if( this->count < this->maxSize) return;
        }
    }

}

/**
 * @brief simpleTabGroups::removeGroup, removes a group of TableData
 * @param groupName, usually a sample
 */
template <class T>
void SimpleTabGroups<T>::removeGroup(const QString &groupName) {
    foreach(T *t, this->simpleTableGroups[groupName]->values()) {
         delete t;
         this->count--;
    }
    delete this->simpleTableGroups[groupName];
    this->simpleTableGroups.remove(groupName);
}



#endif // SIMPLETABGROUPS_H
