#include "tablemanager.h"


TableManager *TableManager::tableManager =0;


TableManager::TableManager()
{
    maxSize = 40;
    timeAccessed = 0;
}


TableManager* TableManager::getTableManager() {
   if( tableManager == 0 )
        tableManager = new TableManager();

    return tableManager;
}


LargeTable *TableManager::getTable(QString fileName) {
    if( tables.contains(fileName) ) {
        tables[fileName].accessed = timeAccessed;
        timeAccessed++;

        return tables[fileName].table;
    }
    return 0;
}

QString TableManager::getTableToDelete() {
    QHash<QString, TABLEACCESSINFO>::iterator it;

    QString fileName;
    unsigned int leastAccessed = timeAccessed;

    for( it = tables.begin(); it != tables.end(); ++it) {
        if( it.value().accessed < leastAccessed )  {
            it.value().accessed = leastAccessed;
            fileName = it.key();
        }
    }
    return fileName;
}


bool TableManager::setTable(QString fileName, LargeTable *table) {

    TABLEACCESSINFO s;
    s.fileName = fileName;
    s.table = table;
    s.accessed = timeAccessed;

   /* foreach( TABLEACCESSINFO t, tables) {
        qDebug() << "------" << t.fileName << "++++ " << sizeof(t.table);
    }
    */

    if(tables.size() > maxSize) {
        QString tableToDelete = getTableToDelete();
        LargeTable *table = tables[tableToDelete].table;
        delete table;
        tables.remove(tableToDelete);
    }

    tables.insert(s.fileName, s);
    timeAccessed++;

    return true;
}


void TableManager::deleteAllTables() {
    foreach( QString fileName, this->tables.keys()) {
        this->deleteTable(fileName);
    }
}

void TableManager::deleteTable(QString fileName) {
    if( tables.contains(fileName) ) {
        delete tables[fileName].table;
        this->tables[fileName].table = NULL;
        tables.remove(fileName);
    }
}


GRAPHDATA *TableManager::getGraphData(QString fileName) {
    if( graphData.contains(fileName) ) {
        graphData[fileName]->accessed = timeAccessed;
        timeAccessed++;
        return graphData[fileName];
    }
    return 0;

}

bool TableManager::setGraphData(QString fileName, GRAPHDATA *graphData) {

    if(this->graphData.size() > maxSize) {
        QString dataToDelete = getGraphDataToDelete();
        GRAPHDATA *data = this->graphData[dataToDelete];
        delete data;
        tables.remove(dataToDelete);
    }

}

void TableManager::deleteAllGraphData() {
    foreach( QString fileName, this->graphData.keys()) {
        this->deleteGraphData(fileName);
    }
}

void TableManager::deleteGraphData(QString fileName) {
    if( this->graphData.contains(fileName ) ) {
        delete this->graphData[fileName];
        this->graphData[fileName] = NULL;
        this->graphData.remove(fileName);
    }
}


QString TableManager::getGraphDataToDelete() {

    QHash<QString, GRAPHDATA *>::iterator it;

     QString fileName;
     unsigned int leastAccessed = timeAccessed;

     for( it = graphData.begin(); it != graphData.end(); ++it) {
            if( it.value()->accessed < leastAccessed )  {
                it.value()->accessed = leastAccessed;
                fileName = it.key();
            }
        }
     return fileName;
}
