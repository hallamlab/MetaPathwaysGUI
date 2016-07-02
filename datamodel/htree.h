#ifndef HTREE_H
#define HTREE_H


#include <QHash>
#include <QDebug>
#include <QList>
#include <QString>

#include "datamodel/connector.h"
#include "helper/types.h"
#include "datamodel/datamodeltypes.h"


typedef struct _ROWDATA {
    QString name;
    QString alias;
   // QString lcaStar;
    QVector<unsigned int> counts;
    QVector<QString> taxons;
    QVector<LCARESULT> lcaresults;
    unsigned int depth;
    bool active ;
} ROWDATA;


class HTree
{
public:
    HTree();
    ~HTree();
    void loadFromFile(QString fileName);
    void hashNodes(HNODE *hnode);
    short getTreeDepth();
    QList<HNODE *> getChildrenOf(QString name);
    QList<ATTRIBUTE *> getLeafAttributesOf(HNODE *hnode);

    void printTree(HNODE *hnode, unsigned int &d);
    QVector<unsigned int> countTree(HNODE *hnode, unsigned int maxDepth, bool showHierarchy, short int currDepth, QList<ROWDATA *> &data, bool showRPKM = false);
    QVector<unsigned int> countTreeRxnInfo(HNODE *hnode, unsigned int maxDepth, bool showHierarchy, short int currDepth, QList<ROWDATA *> &data, VALUETYPE valueType) ;

    void enumerateTaxons(HNODE *hnode, unsigned int maxDepth, bool showHierarchy, short int currDepth, QList<ROWDATA *> &data);

    QList<ROWDATA *> getRows(unsigned int maxDepth, QList< Connector *> &connectors, bool showHierarchy, VALUETYPE valueType);
    QList<ROWDATA *> getRows(QString category, unsigned int maxDepth, QList< Connector *> &connectors, bool showHierarchy, VALUETYPE valueType);
    HNODE *getHNODE(QString name);
    void copyDataToSubConnector(HNODE *hnode, Connector *srcConnector, Connector *targetConnector);
    void copyDataToSubConnector(HNODE *hnode, Connector *targetConnector, HTree* targetTree, QHash<ORF *, bool> & orfHash);
    HNODE *getRootHNODE();
    void setAttrType(ATTRTYPE type);
    ATTRTYPE getAttrType();
    HNODE *root;
private:
    void _getLeafAttributesOf(HNODE *hnode, QList<ATTRIBUTE *> &attrList);
    short _getTreeDepth(HNODE *hnode, short currDepth) ;
    QHash<QString, HNODE *> nodes;
    QList<Connector *> connectors;
    ATTRTYPE type;
};

#endif // HTREE_H
