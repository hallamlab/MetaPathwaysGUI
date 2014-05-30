#ifndef HTREE_H
#define HTREE_H

#include "datamodel/datamodeltypes.h"
#include <QHash>
#include <QDebug>
#include <QList>
#include <QString>
#include "datamodel/connector.h"

typedef struct _ROWDATA {
    QString name;
    QString alias;
    QVector<unsigned int> counts;
    unsigned int depth;
} ROWDATA;


class HTree
{
public:
    HTree();
    void loadFromFile(QString fileName);
    void hashNodes(HNODE *hnode);
    short getTreeDepth();
    QList<HNODE *> getChildrenOf(QString name);
    QList<ATTRIBUTE *> getLeafAttributesOf(HNODE *hnode);

    void printTree(HNODE *hnode, unsigned int d =0);
    QVector<unsigned int> countTree(HNODE *hnode, unsigned int maxDepth, bool showHierarchy, short int currDepth, QList<ROWDATA *> &data, bool showRPKM = false);

    QList<ROWDATA *> getRows(unsigned int maxDepth, bool showHierarchy, QList< Connector *> &connectors, bool showRPKM);
    QList<ROWDATA *> getRows(QString category, unsigned int maxDepth, bool showHierarchy, QList< Connector *> &connectors, bool showRPKM);
    HNODE *getHNODE(QString name);
    void copyDataToSubConnector(HNODE *hnode, Connector *srcConnector, Connector *targetConnector);
    void copyDataToSubConnector(HNODE *hnode, Connector *targetConnector, HTree* targetTree, QHash<ORF *, bool> & orfHash);
    HNODE *getRootHNODE();
    HNODE *root;
private:
    void _getLeafAttributesOf(HNODE *hnode, QList<ATTRIBUTE *> &attrList);
    short _getTreeDepth(HNODE *hnode, short currDepth) ;
    QHash<QString, HNODE *> nodes;
    QList<Connector *> connectors;
};

#endif // HTREE_H
