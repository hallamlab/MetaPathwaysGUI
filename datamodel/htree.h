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
    unsigned int count;
    unsigned int depth;
} ROWDATA;


class HTree
{
public:
    HTree();
    void loadFromFile(QString fileName);
    void hashNodes(HNODE *hnode);
    QList<HNODE *> getChildrenOf(QString name);
    void printTree(HNODE *hnode);
    unsigned int countTree(HNODE *hnode, unsigned int maxDepth, int showHierarchy, short int currDepth, QList<ROWDATA *> &data);

    QList<ROWDATA *> getRows(unsigned int maxDepth, int showHierarchy, Connector *connector);
    HNODE *getHNODE(QString name);
    HNODE *root;
private:
    QHash<QString, HNODE *> nodes;
    Connector *connector;
};

#endif // HTREE_H
