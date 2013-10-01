#include "htree.h"

HTree::HTree()
{
}


void HTree::loadFromFile(QString fileName) {



}


void HTree::hashNodes(HNODE *hnode) {
    this->nodes[hnode->attribute->name] = hnode;
    for(QList<HNODE *>::const_iterator it= hnode->children.begin(); it!=hnode->children.end(); ++it ) {
        this->hashNodes(*it);
    }
}

QList<HNODE *> HTree::getChildrenOf(QString name) {
   QList<HNODE *> results;



   return results;

}


void HTree::printTree(HNODE *hnode) {
    qDebug() << hnode->attribute->name << " " << hnode->depth;

    for(QList<HNODE *>::const_iterator it= hnode->children.begin(); it!=hnode->children.end(); ++it ) {
        printTree(*it);
    }

}


HNODE *HTree::getHNODE(QString name) {
    if( nodes.contains(name))
        return nodes[name];
    else
        return 0;
}


unsigned int HTree::countTree(HNODE *hnode, unsigned int maxDepth, int showHierarchy, short int currDepth, QList<ROWDATA *> &data) {
    unsigned int count =0;

    if(hnode->children.size()==0) {
        if( this->connector->connected.contains(hnode->attribute))
           count = this->connector->connected[hnode->attribute].size();
        else
           count =0;
    }

    ROWDATA *r = new ROWDATA;
    r->name = hnode->attribute->name;
    r->alias = hnode->attribute->alias;
    r->depth = currDepth;

    if( currDepth != -1 &&( (showHierarchy ==0 && currDepth == maxDepth)  || (showHierarchy !=0 && currDepth <= maxDepth)) ) {
       data.append(r);
    }

    for(QList<HNODE *>::const_iterator it= hnode->children.begin(); it!=hnode->children.end(); ++it ) {
        count += this->countTree(*it, maxDepth, showHierarchy, currDepth + 1, data);
    }

    r->count = count;
    return count;
}

QList<ROWDATA *> HTree::getRows(unsigned int maxDepth, int showHierarchy, Connector *connector) {
   QList<ROWDATA *> data;
   this->connector = connector;
   this->countTree(this->root, maxDepth, showHierarchy, -1, data);
   return data;
}
