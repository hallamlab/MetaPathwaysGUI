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

/* computes the depth of the tree recursively starting from the root
 *\return depth : the depth of the tree
 */
short int HTree::getTreeDepth() {
    short int depth;
    depth = this->_getTreeDepth(this->root, 0);
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

HNODE *HTree::getRootHNODE() {
    return root;
}

void HTree::printTree(HNODE *hnode, unsigned int d) {

    for(QList<HNODE *>::const_iterator it= hnode->children.begin(); it!=hnode->children.end(); ++it ) {
       // qDebug("%s\n", (*it)->attribute->name);
        qDebug() <<  (*it)->attribute->name;
        printTree(*it);
    }

}


HNODE *HTree::getHNODE(QString name) {
    if( nodes.contains(name))
        return nodes[name];
    else
        return 0;
}

/** This function travels through the functional  hierarchies of several
 * samples simultaneously
 \param hnode this is the root node of the HTree
 \param maxDepth maximum depth of the the current HTree
 \param showHierarchy sets to show hierarcy or not
 \param currDepth a variable to keep track of the recursive depth
 \param data the ROWDATA list to show on the table
 \param showRPKM shows/hides RPKM values instead of count

 \return vcount it is a vector of counts, one per sample
*/
QVector<unsigned int> HTree::countTree(HNODE *hnode, unsigned int maxDepth, bool showHierarchy, short int currDepth, QList<ROWDATA *> &data, bool showRPKM) {
    QVector<unsigned int> vcount;
    QVector<unsigned int> tempcount;

    currDepth++;

    ROWDATA *r = new ROWDATA;
    r->name = hnode->attribute->name;
    r->alias = hnode->attribute->alias;
    r->depth = currDepth;


    // initialize the counts
    for(unsigned int j=0; j < this->connectors.size(); j++) {
        vcount.push_back(0);
    }

    // we reached the bottom leaves
    if(hnode->children.size()==0) {
    //    qDebug() << "Bottom " << hnode->attribute->name;
        for(unsigned int j=0; j < this->connectors.size(); j++) {
           if( this->connectors[j]->connected.contains(hnode->attribute)) {
               if(showRPKM)
                 vcount[j] = this->connectors[j]->getRPKMForAttribute(hnode->attribute);
               else
                 vcount[j] = this->connectors[j]->connected[hnode->attribute].size();
           }
           else
               vcount[j] = 0;
        }
    }

   // qDebug() << r->name << " " << currDepth << " " << maxDepth;

    if( currDepth != 0 &&( (!showHierarchy && currDepth == maxDepth)  || (showHierarchy && currDepth <= maxDepth)) ) {
       data.append(r);
    }

    for(QList<HNODE *>::const_iterator it= hnode->children.begin(); it!=hnode->children.end(); ++it ) {
        tempcount = this->countTree(*it, maxDepth, showHierarchy, currDepth, data, showRPKM);
        for(unsigned int j=0; j < this->connectors.size(); j++) vcount[j] += tempcount[j];
    }
    r->counts= vcount;

    return vcount;
}

/** function to get the list of rowdata at a certain depth of the HTree
 *\param maxDepth, the maximum depth to collect the categories
 *\param showHierarchy, a bool to show/hide the hierarchy
 *\param connectors, the list of conneectors for various samples
 *\param showRPKM, a bool to show/hide the RPKM or counts to show
 *
 *\return the list of ROWDATA to show in the QTableWidget
 **/
QList<ROWDATA *> HTree::getRows(unsigned int maxDepth, bool showHierarchy, QList<Connector *> &connectors, bool showRPKM) {
   QList<ROWDATA *> data;
   this->connectors = connectors;
   this->countTree(this->root, maxDepth, showHierarchy, -1, data, showRPKM);
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

/** This function to get the data vector for the count/RPKM values for orfs in the category
 *\param category, functional category
 *\param maxDepth, the maximum depth of the functional category
 *\param showHierarchy, a bool that hides/shows the hierachy
 *\param connectors, the connectors that connects the ORFs to the different functional category
 *\param showRPKM, shows/hides the RPKM values instead of the counts

 *\return a list of ROWDATA that can be used to fill the QWidgetTable with counts/RPKM
 **/
QList<ROWDATA *> HTree::getRows(QString category, unsigned int maxDepth, bool showHierarchy, QList<Connector *> &connectors, bool showRPKM) {
   QList<ROWDATA *> data;
   this->connectors = connectors;
   HNODE *node = this->getHNODE(category);
  // qDebug() << "Hnode " << node << " category " << category << " depth " << node->depth;
   if(node!=0)
      this->countTree(node, maxDepth, showHierarchy, -1 + node->depth, data, showRPKM);
   return data;
}
