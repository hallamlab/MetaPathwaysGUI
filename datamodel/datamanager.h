#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QStack>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSettings>
#include <QStringList>

#include "helper/rundata.h"
#include "datamodel/datamodeltypes.h"
#include "datamodel/htree.h"
#include "datamodel/connector.h"

class DataManager
{
public:
    static DataManager *getDataManager();
    HTree *getHTree(ATTRTYPE name);
    QString getORFFaa(QString sampleName);
    QString getORFFna(QString sampleName);
    QString getORFFasta(QString sampleName);
    QStringList getConnectorSamples();
    QList<ATTRTYPE> getConnectorSampleAttributes(QString sampleName);

    QList<ORF *> *getORFList(QString sampleName);

    void setIndexFileFaa(QString sampleName, QString fileName);
    void setIndexFileFna(QString sampleName, QString fileName);
    void setIndexFileFasta(QString sampleName, QString fileName);
    Connector *getConnector(QString sampleName, ATTRTYPE type);
    Connector *createConnector(QString sampleName, HTree *htree, ATTRTYPE attrType, QList<ORF *> *orfList);

    Connector *createSubConnector(HTree *tree, HNODE *hnode, Connector* connector, ATTRTYPE attrType);

    void deleteConnector(QString sampleName) ;
    void deleteAllConnectors();

    void createORFs(QString sampleName, QString fileName);

    void addNewAnnotationToORFs(QString sampleName, QString fileName);
    void addRPKMToORFs(QString sampleName, QString fileName);

    ORF* _createAnORF(QStringList &attributes, QString &sampleName);
    void destroyORFs(QString sampleName);
    void destroyAllORFs();
    void _destroyAnORF(ORF *);
    void destroyAllContigs();
    void destroyContigs(QString &sampleName);
   // void destroyAllAttributes();

    void setDataModelCreated(bool);
    bool getDataModelCreated();
    bool AreORFsUptoDate(QString sample);
    void setORFsUptoDate(QString, bool);
    void setORFsUptoDateAll( bool);

    void createDataModel();
    CATEGORYNODE createCategoryNode(QString line);
    HTree *createHTree(QString refDB);
    void destroyAllHTrees();
    void destroyHTree(ATTRTYPE refDB );
    void _destroyHTree(HNODE *hnode );

    void destroyAllAttributes();


private:
    DataManager();
    QHash<QString, QList<ORF *> *> *ORFList;
    QHash<QString, bool> ORFsUptoDateList;
    QHash<QString, QHash<ATTRTYPE, Connector *> > connectors;
    QHash<ATTRTYPE, HTree *>  htrees;

    static DataManager *datamanager;
    QHash<QString, QHash<QString, CONTIG *> > contigHash;

    QHash<ATTRTYPE, QHash<QString, ATTRIBUTE *> > attributes;

    bool dataModelCreated;


};

#endif // DATAMANAGER_H
