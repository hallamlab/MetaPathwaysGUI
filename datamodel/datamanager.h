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
#include "datamodel/taxons.h"
#include "datamodel/samplercmanager.h"

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
    QList<ORF *> getORFList(QString sampleName, QString contig);
    QList<ORF *> getORFByNames(const QString &sampleName, const QHash<QString, bool> &orfNames);

    void setIndexFileFaa(QString sampleName, QString fileName);
    void setIndexFileFna(QString sampleName, QString fileName);
    void setIndexFileFasta(QString sampleName, QString fileName);
    Connector *getConnector(QString sampleName, ATTRTYPE type);
    Connector *createConnector(QString sampleName, HTree *htree, ATTRTYPE attrType, QList<ORF *> *orfList);
    Connector *_createConnector(QString sampleName, HTree *htree, ATTRTYPE attrType, QList<ORF *> *orfList);

    Connector *createSubConnector(HTree *tree, HNODE *hnode, Connector* connector, ATTRTYPE attrType);

    void deleteConnector(QString sampleName) ;
    void deleteAllConnectors();

    void createORFs(QString sampleName);

    void addNewAnnotationToORFs(QString sampleName, QString fileName);
    void addRPKMToORFs(QString sampleName, QString fileName);
    void addTaxons(const QString &sampleName);
    void addTaxonToORFs(const QString &sampleName, const QString &fileName);

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
    QStringList getResourceFiles(const RESOURCETYPE &resType) ;

    HTree *createHTree(QString refDB, ATTRTYPE type);
    HTree *createBaseTree(HTree *shtree);
    bool  _createBaseTree(HNODE *snode, HNODE *hnode, QHash<QString, bool> &created);
    void destroyAllHTrees();
    void destroyHTree(ATTRTYPE treeType );
    void _destroyHTree(HNODE *hnode );

    void destroyAllAttributes();
    void destroyAllTaxons();


    bool loadNameMapping(QHash<QString,QString> &nameMaps, const QString fileName) ;
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
