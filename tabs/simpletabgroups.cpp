#include "simpletabgroups.h"

/**
 * @brief SimpleTabGroups::SimpleTabGroups, constructor for the simpleTabGroups
 *  maxSize is the maximum numter of allowed tabs in total, across all groups
 *  count is a variable that refers to the current number of tables
 */
SimpleTabGroups::SimpleTabGroups()
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
bool SimpleTabGroups::tableExists(QString groupName, QString fileName) {
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
void SimpleTabGroups::addTable(TableData *t, const QString &groupName, const QString &fileName) {

    if( this->maxSize < this->count)
    this->removeAGroupExcept(groupName);

    if( !this->simpleTableGroups.contains(groupName )) {
        this->simpleTableGroups[groupName] = new QHash<QString, TableData *>;
    }
    this->simpleTableGroups[groupName]->insert(fileName, t);
}
/**
 * @brief simpleTabGroups::getTable, retrieves a ble from the groupName, loaded from the file fileName
 * @param groupName, corresponds to a sample
 * @param fileName, corresponds to a file
 * @return
 */
TableData * SimpleTabGroups::getTable(const QString &groupName, const QString &fileName) {
    if( !this->simpleTableGroups.contains(groupName )) {
        return 0;
    }
    if( !this->simpleTableGroups[groupName]->contains(fileName) ) {
        return 0;
    }

    this->count++;
    return this->simpleTableGroups[groupName]->value(fileName);
}

/**
 * @brief simpleTabGroups::removeAGroupExcept, removes any group other than groupName, until
 *        the count falls below the maxcount;
 * @param groupName, name of the group usually a sample
 */
void SimpleTabGroups::removeAGroupExcept(const QString &groupName ) {
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
void SimpleTabGroups::removeGroup(const QString &groupName) {
    foreach(TableData *t, this->simpleTableGroups[groupName]->values()) {
         delete t;
         this->count--;
    }
    delete this->simpleTableGroups[groupName];
    this->simpleTableGroups.remove(groupName);
}
