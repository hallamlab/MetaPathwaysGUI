#ifndef DATAMANAGER_H
#define DATAMANAGER_H
#include "datamodel/datamodeltypes.h"
#include "datamodel/htree.h"
#include "datamodel/connector.h"
#include <QStack>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSettings>
#include <QStringList>

class DataManager
{
public:
    static DataManager *getDataManager();
    HTree *getHTree(ATTRTYPE name);
    QString getORFFaa(QString sampleName);
    QString getORFFna(QString sampleName);
    QString getORFFasta(QString sampleName);

    QList<ORF *> *getORFList(QString sampleName);

    void setIndexFileFaa(QString sampleName, QString fileName);
    void setIndexFileFna(QString sampleName, QString fileName);
    void setIndexFileFasta(QString sampleName, QString fileName);
    Connector *getConnector(QString sampleName, ATTRTYPE type);
    Connector *createConnector(QString sampleName, HTree *htree, ATTRTYPE attrType, QList<ORF *> *orfList);

    Connector *createSubConnector(HTree *tree, HNODE *hnode, Connector* connector, ATTRTYPE attrType);

    void createORFs(QString sampleName, QString fileName);

    void addNewAnnotationToORFs(QString sampleName, QString fileName);

    ORF* _createAnORF(QStringList &attributes);


    void createDataModel();
    CATEGORYNODE createCategoryNode(QString line);
    HTree *createHTree(QString refDB);

private:
    DataManager();
    QHash<QString, QList<ORF *> *> *ORFList;
    QHash<QString, QHash<ATTRTYPE, Connector *> > connectors;
    QHash<ATTRTYPE, HTree *>  htrees;

    static DataManager *datamanager;

    QHash<ATTRTYPE, QHash<QString, ATTRIBUTE *> > attributes;

};

#endif // DATAMANAGER_H
