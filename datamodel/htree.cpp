#include "htree.h"

HTree::HTree()
{
    this->root = 0;
}


void HTree::loadFromFile(QString fileName) {



}

short int HTree::_getTreeDepth(HNODE *hnode, short int currDepth) {
    if( hnode==0 ) return currDepth;

    short int depth = currDepth;

    short int tempDepth;
    for(QList<HNODE *>::const_iterator it= hnode->children.begin(); it!=hnode->children.end(); ++it ) {
       tempDepth = this->_getTreeDepth(*it, currDepth+1);
       if( tempDepth > depth)
           depth = tempDepth;
    }

    return depth;

}

short int HTree::getTreeDepth() {
    short int depth;

    depth = this->_getTreeDepth(this->root, -1);
    return depth;
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


QList<ATTRIBUTE *> HTree::getLeafAttributesOf(HNODE *hnode) {
    QList<ATTRIBUTE *> attrList;
    this->_getLeafAttributesOf(hnode, attrList);
    return attrList;
}

void HTree::_getLeafAttributesOf(HNODE *hnode, QList<ATTRIBUTE *> &attrList) {
    if(hnode->children.size()==0) {
        attrList.append(hnode->attribute);
    }
    for(QList<HNODE *>::const_iterator it= hnode->children.begin(); it!=hnode->children.end(); ++it ) {
        this->_getLeafAttributesOf(*it, attrList);
    }
}


void HTree::printTree(HNODE *hnode) {

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



QVector<unsigned int> HTree::countTree(HNODE *hnode, unsigned int maxDepth, int showHierarchy, short int currDepth, QList<ROWDATA *> &data) {
    QVector<unsigned int> vcount;
    QVector<unsigned int> tempcount;

    for(unsigned int j=0; j < this->connectors.size(); j++) {
        vcount.push_back(0);
    }

    if(hnode->children.size()==0) {
        for(unsigned int j=0; j < this->connectors.size(); j++) {

           if( this->connectors[j]->connected.contains(hnode->attribute)) {
               vcount[j] = this->connectors[j]->connected[hnode->attribute].size();
           }

        }
    }

    ROWDATA *r = new ROWDATA;
    r->name = hnode->attribute->name;
    r->alias = hnode->attribute->alias;
    r->depth = currDepth;

    if( currDepth != -1 &&( (showHierarchy ==0 && currDepth == maxDepth)  || (showHierarchy !=0 && currDepth <= maxDepth)) ) {
       data.append(r);
    }

    for(QList<HNODE *>::const_iterator it= hnode->children.begin(); it!=hnode->children.end(); ++it ) {
        tempcount = this->countTree(*it, maxDepth, showHierarchy, currDepth + 1, data);
        for(unsigned int j=0; j < this->connectors.size(); j++) vcount[j] += tempcount[j];
    }
    r->counts= vcount;

    return vcount;
}

QList<ROWDATA *> HTree::getRows(unsigned int maxDepth, int showHierarchy, QList<Connector *> &connectors) {
   QList<ROWDATA *> data;
   this->connectors = connectors;
   this->countTree(this->root, maxDepth, showHierarchy, -1, data);
   return data;
}


void HTree::copyDataToSubConnector(HNODE *hnode, Connector *srcConnector, Connector *targetConnector) {

    if(hnode->children.size()==0) {
        targetConnector->connected[hnode->attribute]=srcConnector->connected[hnode->attribute];
    }

    for(QList<HNODE *>::const_iterator it= hnode->children.begin(); it!=hnode->children.end(); ++it ) {
         this->copyDataToSubConnector(*it, srcConnector, targetConnector);
    }

}



void HTree::copyDataToSubConnector(HNODE *hnode,  Connector *targetConnector, HTree* targetTree, QHash<ORF *,bool> &orfHash) {

    if(hnode->children.size()==0) {
       foreach(ORF *orf, targetConnector->connected[hnode->attribute] ) {
           if( orfHash.contains(orf)) {
                targetConnector->connected[hnode->attribute].append(orf);
           }
       }
    }

    for(QList<HNODE *>::const_iterator it= hnode->children.begin(); it!=hnode->children.end(); ++it ) {
         this->copyDataToSubConnector(*it, targetConnector, targetTree,  orfHash);
    }

}

QList<ROWDATA *> HTree::getRows(QString category, unsigned int maxDepth, int showHierarchy, QList<Connector *> &connectors) {
   QList<ROWDATA *> data;
   this->connectors = connectors;
   HNODE *node = this->getHNODE(category);
   if(node!=0)
      this->countTree(node, maxDepth, showHierarchy, -1, data);
   return data;
}
