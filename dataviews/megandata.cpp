#include "megandata.h"
#include <QDebug>
MeganData::MeganData()
{
    this->deltaX = 70;
    this->deltaY = 30;
    this->STARTX =0;
    this->STARTY = 0;
    this->INIT_LINE_LENGTH = 20;
    this->scale = 1;
}
void MeganData::setOffset(double startX, double startY) {
    this->STARTX = startX;
    this->STARTY = startY;
}

void MeganData::setInitLineLength(double length) {
    this->INIT_LINE_LENGTH = length;
}

void MeganData::setScale(double scale) {

    this->scale = scale;
}

void MeganData::setDeltaY(double deltaY) {
    this->deltaY = deltaY;
}

void MeganData::setData(const QString &data) {
   this->data  = data;
}

void MeganData::printTree(TREENODE *root, int i) {

    QString space;
    for(unsigned int j =0; j < i; j++) space.append(' ');
   // qDebug() << i <<" " << space  << root->name << " " << root->count ;

    foreach(TREENODE *child, root->children) {
        printTree(child, i + 1);
    }

}

void MeganData::createTreeView() {
    QStringList tokens;
    this->tokenize(tokens);

    this->createTreeStructure(tokens);

    this->computeCoordinates(this->deltaX,this->deltaY);

}


void MeganData::_createTaxonItems(TREENODE *node, QHash<unsigned int, QList< GraphicsTaxonItem *> > &taxons,  GraphicsTaxonItem *parentItem, GraphicsItemsCollection *itemCreator, double deltaX, double deltaY, QBrush taxonBrush) {

    GraphicsTaxonItem *item = itemCreator->getTaxonNode(node);

    item->setBrush(taxonBrush);
    if( !taxons.contains(node->depth) ) {
        taxons[node->depth] = QList<GraphicsTaxonItem *>();
    }
    taxons[node->depth].append(item);
    if(parentItem!=0) parentItem->Children.append(item);

    foreach(TREENODE *child, node->children) {
        _createTaxonItems(child, taxons, item, itemCreator, deltaX, deltaY, taxonBrush);
    }
}

void MeganData::setLines(GraphicsConnectorLines *lines, LineStyle style, GraphicsTaxonItem *a, GraphicsTaxonItem *b) {
    if( style == STRAIGHT) {
        lines->segments[0]->setLine(a->rect().left() + a->radius/2, a->rect().top() + a->radius/2, b->rect().left()+ b->radius/2, b->rect().top() + b->radius/2);
    }

    if( style == MANHATTAN ) {
       lines->segments[0]->setLine(a->rect().left() + a->radius/2, a->rect().top() + a->radius/2, a->rect().left()+ a->radius/2, b->rect().top() + b->radius/2);
       lines->segments[1]->setLine(a->rect().left() + a->radius/2, b->rect().top() + b->radius/2, b->rect().left()+ b->radius/2, b->rect().top() + b->radius/2);
    }

}

void MeganData::setStyleVisible(GraphicsTaxonItem *taxon, LineStyle style,  bool visible, unsigned int currDepth, unsigned int maxDepth) {
    if( currDepth <= maxDepth && visible)
        taxon->inscene = true;
    else {
        taxon->inscene = false;
       // qDebug() << " synchro " << node->depth << " " << node->deltaX << " (" << node->depth*node->deltaX <<"," << node->deltaY*(node->Ydown + node->Yup)/2 << ")  " << taxon->pos();
    }

    for(unsigned int i=0; i < taxon->Children.size(); i++) {
        if( currDepth < maxDepth && visible) {
            taxon->Lines[style][i]->inscene = true;
            this->setLines(taxon->Lines[style][i], style, taxon, taxon->Children[i]);
        }
        else {
             taxon->Lines[style][i]->inscene = false;
        }
        this->setStyleVisible( taxon->Children[i], style, visible, currDepth + 1, maxDepth);
    }
}

void MeganData::synchronizeConnectingLines(GraphicsTaxonItem *taxon, unsigned int currDepth, unsigned int maxDepth, LineStyle style) {
    if(currDepth > maxDepth ) {
        return;
    }

    for(unsigned int i=0; i < taxon->Children.size(); i++) {
        this->setLines(taxon->Lines[style][i], style, taxon, taxon->Children[i]);
        synchronizeConnectingLines( taxon->Children[i], currDepth + 1, maxDepth, style);
    }
}

void MeganData::synchronizeTaxonNames(GraphicsTaxonItem *taxon, unsigned int currDepth, unsigned int maxDepth) {
    if(currDepth > maxDepth ) {
        return;
    }
    double textHeight =  taxon->Name->boundingRect().height();
    taxon->Name->setPos(taxon->rect().right() + 50, (taxon->rect().top() + taxon->rect().bottom() - textHeight)/2);
    for(unsigned int i=0; i < taxon->Children.size(); i++) {
        synchronizeTaxonNames( taxon->Children[i], currDepth + 1, maxDepth);
    }
}


void MeganData::synchronizeNodeAndTaxonItems(TREENODE *node, GraphicsTaxonItem *taxon, unsigned int currDepth, unsigned int maxDepth) {
    if(currDepth > maxDepth ) {
        return;
    }
    taxon->setRect(this->STARTX + (node->depth*this->deltaX -  taxon->radius/2)*this->scale, (this->STARTY + this->deltaY*(node->Ydown + node->Yup)/2 - taxon->radius/2)*this->scale, taxon->radius, taxon->radius);

    for(unsigned int i=0; i < node->children.size(); i++) {
        synchronizeNodeAndTaxonItems(node->children[i], taxon->Children[i], currDepth + 1, maxDepth);
    }
}

void MeganData::createTaxonItems(QHash<unsigned int, QList< GraphicsTaxonItem *> > &taxons, GraphicsItemsCollection *itemCreator, double deltaX, double deltaY, QBrush taxonBrush) {
    this->_createTaxonItems(this->root, taxons, 0, itemCreator, deltaX, deltaY, taxonBrush);
}



void MeganData::createConnectingLines(GraphicsTaxonItem *pitem, GraphicsItemsCollection* itemCreator, QPen connectorPen){
    QList<GraphicsTaxonItem *>::Iterator it;
    GraphicsConnectorLines *clines;
    for(it = pitem->Children.begin(); it!=pitem->Children.end(); it++) {
       clines = itemCreator->getLineBetweenTaxonItems(pitem, *it, MANHATTAN, connectorPen);
       pitem->Lines[MANHATTAN].append(clines);
       clines = itemCreator->getLineBetweenTaxonItems(pitem, *it, STRAIGHT, connectorPen);
       pitem->Lines[STRAIGHT].append(clines);
       this->createConnectingLines(*it, itemCreator, connectorPen);
    }
}


void MeganData::createTaxonNames(GraphicsTaxonItem *pitem, GraphicsItemsCollection* itemCreator, QPen namePen){
    GraphicsNameItem *name;
    name = itemCreator->getNameNode(pitem);
    pitem->Name = name;
    QList<GraphicsTaxonItem *>::Iterator it;
    for(it = pitem->Children.begin(); it!=pitem->Children.end(); it++) {
        this->createTaxonNames(*it, itemCreator, namePen);
    }
}


unsigned int MeganData::getDepth() {
    return root->depth;
}



unsigned int MeganData::getHeight() {
    return root->Ydown;
}


void MeganData::computeCoordinates(double deltaX, double deltaY) {
    this->deltaX = deltaX;
    this->deltaY = deltaY;
    int leafcounter =0;
    this->treeTraversal(root, leafcounter, 0 );
    this->adjustDepth(root, root->depth);
}

double MeganData::_spanAtDepth(TREENODE *node, unsigned int depth, double *Yup, double *Ydown) {
    if( node->depth== depth) {
        if( *Yup  <   (node->Ydown+node->Yup)/2 ) *Yup = (node->Ydown+node->Yup)/2;
        if( *Ydown > (node->Ydown+node->Yup)/2 ) *Ydown =  (node->Ydown+node->Yup)/2;
       // qDebug() << " update ";
        return 0;
    }

    foreach(TREENODE *child, node->children) {
     //   qDebug() << " Going in " << node->depth;
        _spanAtDepth(child, depth, Yup, Ydown);
    }
    return 0;

}



void MeganData::unscaleVertically(GraphicsTaxonItem *taxon, double unscale, unsigned int currDepth, unsigned int maxDepth) {
    if(currDepth > maxDepth ) {
        return;
    }

  //  qDebug() << " unscaling " << unscale;
    taxon->Name->setScale(unscale);

    for(unsigned int i=0; i < taxon->Children.size(); i++) {
        synchronizeTaxonNames( taxon->Children[i], currDepth + 1, maxDepth);
    }


}

double MeganData::spanAtDepth(unsigned int depth) {
    double Yup, Ydown;
    Yup = root->Ydown;
    Ydown = root->Yup;

    qDebug() << " root span = " << root->Yup << "   " << root->Ydown;
    _spanAtDepth(root, depth, &Yup, &Ydown);
    qDebug() << "  span = " << Yup << "   " << Ydown;

    return (Ydown-Yup)/(root->Ydown - root->Yup);
}


void MeganData::adjustDepth(TREENODE *node, unsigned int maxdepth) {
    _adjustDepth(root, 0);
}


void MeganData::_adjustDepth(TREENODE *node,unsigned int depth) {
    node->depth = depth;
    foreach(TREENODE *child, node->children) {
        _adjustDepth(child, depth +1  );
    }
}

unsigned int MeganData::treeTraversal(TREENODE *node, int & leafcounter, unsigned int currDepth, unsigned int maxDepth) {

    if(node->children.size()==0) {
        node->depth =0;
        node->Yup = leafcounter;
        node->Ydown = node->Yup;
        leafcounter++;
        return node->depth;
    }


    node->depth = 0;
    node->Ydown = -1;
    node->Yup = -1;
    unsigned int _depth;
    for(QList<TREENODE *>::iterator c= node->children.begin(); c!= node->children.end(); c++) {
        _depth = this->treeTraversal(*c, leafcounter, currDepth +1, maxDepth );
        if( node->depth < _depth) node->depth = _depth;
        if( node->Ydown < 0 ) {
            node->Ydown = (*c)->Ydown;
            node->Yup = (*c)->Yup;
        }
        else {
            if(node->Yup > (*c)->Yup)
                node->Yup = (*c)->Yup;
            if(node->Ydown < (*c)->Ydown)
                node->Ydown = (*c)->Ydown;
        }
      //  qDebug() << "name " << (*c)->name;
    }
    node->depth++;
    node->leafcount = -1;
    return node->depth;
}

QRect MeganData::getGeometry(unsigned int depth) {
    QRect rect;

    rect.setTop(0);
    rect.setLeft(0);
    rect.setBottom(2*STARTY + root->Ydown*this->deltaY);
    rect.setRight(2*STARTX + depth*this->deltaX);
    return rect;
}

void MeganData::computeBounds(unsigned int maxDepth) {
    int leafcounter =0;
    this->_computeBounds(this->root, leafcounter, 0, maxDepth);
}

void MeganData::_computeBounds(TREENODE *node, int & leafcounter, unsigned int currDepth, unsigned int maxDepth) {

    if(node->children.size()==0 || currDepth == maxDepth) {
        node->Yup = leafcounter;
        node->Ydown = node->Yup;
        leafcounter++;
        return;
    }

    node->Ydown = -1;
    node->Yup = -1;

    for(QList<TREENODE *>::iterator c= node->children.begin(); c!= node->children.end(); c++) {
        this->_computeBounds(*c, leafcounter, currDepth +1, maxDepth );

        if( node->Ydown < 0 ) {
            node->Ydown = (*c)->Ydown;
            node->Yup = (*c)->Yup;
        }
        else {
            if(node->Yup > (*c)->Yup)
                node->Yup = (*c)->Yup;
            if(node->Ydown < (*c)->Ydown)
                node->Ydown = (*c)->Ydown;
        }
    }
    node->leafcount = -1;

}


void MeganData::tokenize(QStringList &tokens) {
    QStringList tempTokens;
    QStringList tempTempTokens;
    tempTokens << this->splitByDelimiter(this->data, QChar('(')) ;
    foreach(QString tempToken, tempTokens) {
       tempTempTokens << this->splitByDelimiter(tempToken, QChar(')'));
    }
    foreach(QString tempToken, tempTempTokens) {
        tokens << this->splitByDelimiter(tempToken, QChar('{'));
    }

    for(unsigned int i =0; i < tokens.size(); i++) {
        if( tokens[i] == QString("{"))
            tokens[i] = QString(",");
    }
}

void MeganData::addAttributes(TREENODE *p, QString str) {
    QStringList tempTokens;
    tempTokens << str.split(QChar('}')) ;
    if( tempTokens.size() < 2 )
        p->count = 0;
    else {
        bool k;
        p->count = tempTokens[1].toInt(&k);
    }
    if( tempTokens.size() < 1)
        p->name = "Error";
    else
        p->name = tempTokens[0];

}

TREENODE *MeganData::createTree(QStringList &tokens) {
    QStack<TREENODE *> S;
    QString prev;
    TREENODE *node, *singleNode, *pnode;
    for(QStringList::const_iterator t = tokens.begin(); t != tokens.end(); t++ ) {
        if( *t == QString(",")) continue;

        if( *t == QString("(")) {
            node = new TREENODE;
            S.push(node);
        }

        if( (*t !=QString(")") && *t!=QString("(")) && prev!=QString(")")) {
            singleNode = new TREENODE;
            this->addAttributes(singleNode, *t);
            node->children.append(singleNode);
        }

        if( (*t !=QString(")") && *t!=QString("(")) && prev==QString(")")) {
            this->addAttributes(node, *t);

            node = S.pop();
            if(!S.empty()) {
                pnode = S.top();
                pnode->children.append(node);
                node = pnode;
            }
        }
        prev = *t;

    }
    return node;
}

void MeganData::createTreeStructure(QStringList &tokens) {
    root = createTree(tokens);
}

QStringList MeganData::splitByDelimiter(const QString &str, const QChar &sep, bool removeEmptyStrings) {
    QStringList results;
    QStringList tempStrings = str.split(sep);

    for(unsigned int i=0; i < tempStrings.size(); i++) {
        if( i > 0 ) results << QString(sep);
        if(!(tempStrings[i].isEmpty() && removeEmptyStrings) )
           results << tempStrings[i] ;
    }
    return results;
}

QStringList MeganData::splitByDelimiter(const QStringList &strlist, const QChar &sep) {
     QStringList results;
     foreach( QString s, strlist) {
         results << this->splitByDelimiter(s, sep);
     }
     return results;
}


void MeganData::_deleteNode(TREENODE *t) {
    QList<TREENODE *>::iterator it;
    for(it = t->children.begin(); it!= t->children.end(); it++) {
        this->_deleteNode(*it);
    }
    t->children.clear();
    delete t;
}

MeganData::~MeganData() {
    this->_deleteNode(root);    
}
