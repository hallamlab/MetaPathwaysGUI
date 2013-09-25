#include "graphicsitemscollection.h"
#include <math.h>

GraphicsItemsCollection *GraphicsItemsCollection::graphicsItemsCreator =0;
GraphicsItemsCollection::GraphicsItemsCollection() {


}


GraphicsItemsCollection *GraphicsItemsCollection::getGraphicsItemsCollection() {
    if( graphicsItemsCreator==0 )
        graphicsItemsCreator = new GraphicsItemsCollection();

    return graphicsItemsCreator;

}


GraphicsNotchedLine *GraphicsItemsCollection::getNotchedLine(NotchedLineParams &p) {

    GraphicsNotchedLine *g =new GraphicsNotchedLine();
    g->params = p;
    g->basePairsToPixel =  g->params.width/g->params.nomWidth;

    g->insertMarkers();
    return g;
}


void  GraphicsNotchedLine::insertMarkers(double scale) {
    LINEPROPERTY lineSize;

    this->scale = scale;
    lineSize.x = this->params.x;
    lineSize.y = this->params.y;

    double adjNomInterval = this->params.pixInterval/(this->params.basePairToPixelRatio*this->scale*5) < 1 ? 5 :5*(unsigned int)(this->params.pixInterval/(this->params.basePairToPixelRatio*this->scale*5.0));

    lineSize.width  = this->params.width*this->scale;

    lineSize.thickness = this->params.height;
    if(hline != 0 ) {
        this->removeFromGroup(hline);
        delete hline;
    }
    hline = GraphicsItemsCollection::getLineShape(lineSize);
    this->addToGroup(hline);

    MARKER marker;
    double deltax;
    for(unsigned int i=0; i*adjNomInterval < this->params.nomWidth; i++) {
        lineSize.x = this->params.x + i*adjNomInterval*this->scale*this->params.basePairToPixelRatio;
        lineSize.thickness = this->params.height;

        marker.notch = new QGraphicsLineItem(lineSize.x, lineSize.y, lineSize.x, lineSize.y -this->params.notchHeight);
        marker.text  = new QGraphicsTextItem();
        marker.text->setPlainText( QString::number( (unsigned int)(i*adjNomInterval)));

        deltax =  marker.text->boundingRect().width()/2;
        marker.text->setPos( QPointF(lineSize.x -deltax, lineSize.y) );

        this->addToGroup(marker.notch);
        this->addToGroup(marker.text);
        this->addMarker(marker);
    }

}

void GraphicsNotchedLine::addMarker(MARKER &item) {
    this->markers.push_back(item);
}


void GraphicsNotchedLine::clearMarkers() {
    foreach( MARKER marker, this->markers) {
        this->removeFromGroup(marker.notch);
        this->removeFromGroup(marker.text);
        delete marker.notch;
        delete marker.text;
    }
    this->markers.clear();
}
/*********************** GRAPHICS NAME ITEM  *********************/
 GraphicsNameItem::GraphicsNameItem() {

     this->inscene = false;
 }

/***********************   GRAPHICSTAXONITEM *********************/
GraphicsTaxonItem::GraphicsTaxonItem() {

   this->inscene = false;

}


/***********************   GRAPHICS CONNECTOR LINE ****************/
GraphicsConnectorLines::GraphicsConnectorLines() {
   this->inscene = false;

}

void GraphicsConnectorLines::addLineSegment(QGraphicsLineItem *line) {
    segments.append(line);
}

void GraphicsConnectorLines::setStyle(LineStyle style) {
    this->style  = style;
}

GraphicsConnectorLines::~GraphicsConnectorLines() {
   QList<QGraphicsLineItem *>::iterator it;
   for(it  = this->segments.begin(); it!=this->segments.end(); it++) {
       delete  *it;
       qDebug() << "deleting ";
   }
}

/************************  GRAPHICS GENE ITEM *********************/

void GraphicsGeneItems::insertORFs(double scale) {
    this->scale = scale;
    this->drawORFDiagrams();
}

GraphicsGeneItems::GraphicsGeneItems(QList<ORFData> &orfs, STRAND strand, GENEPROPERTY &geneProp, PENPOSITION &pen) {
    this->orfData = orfs;
    this->strand = strand;
    this->geneProp = geneProp;
    this->pen = pen;
    this->scale = 1;
    this->drawORFDiagrams();
}


void GraphicsGeneItems::clearORFs() {
    foreach(QGraphicsPolygonItem *orf, orfs) {
        this->removeFromGroup(orf);
        delete orf;
    }
    orfs.clear();
}



void GraphicsGeneItems::drawORFDiagrams()  {
    GraphicsItemsCollection *itemCreator  = GraphicsItemsCollection::getGraphicsItemsCollection();
    QGraphicsPolygonItem *p;

    foreach( ORFData orf, orfData) {
        if( orf.strand != strand ) continue;
        this->geneProp.x = this->pen.xstart + this->scale*orf.start*this->geneProp.basePairToPixelRatio ;
        this->geneProp.y = this->pen.ystart +  orf._level*(this->geneProp.spaceAbove + geneProp.height + geneProp.spaceBelow) + geneProp.spaceAbove;
        this->geneProp.width = (orf.end - orf.start)*this->geneProp.basePairToPixelRatio;
        this->geneProp.width = this->geneProp.width*this->scale;
        p = itemCreator->getGeneShape(this->geneProp, strand);
        p->setToolTip("Function : " + orf.func_annot + "<br>" \
                + "Taxonomy : " + orf.tax_annot);
        orfs.push_back(p);
        this->addToGroup(p);
    }
}


QGraphicsRectItem *GraphicsItemsCollection::getLineShape(LINEPROPERTY &lineshape) {
    QGraphicsRectItem *p = new QGraphicsRectItem(lineshape.x,lineshape.y, lineshape.width, lineshape.thickness);
    p->setPen( QPen(Qt::black));
    p->setBrush( QBrush(Qt::black));
    return p;
}


GraphicsGeneItems *GraphicsItemsCollection::getORFDiagrams(QList<ORFData> &orfs, STRAND strand, GENEPROPERTY &geneProp, PENPOSITION &pen) {
    GraphicsGeneItems *orfDiagram = new GraphicsGeneItems(orfs, strand, geneProp, pen);
    return orfDiagram;
}

QGraphicsPolygonItem *GraphicsItemsCollection::getGeneShape(GENEPROPERTY &gene, STRAND strand) {
    QPolygonF dim;
    if( strand == FORWARD)
    dim << QPointF(gene.x, gene.y)\
        << QPointF(gene.x + gene.width-gene.tipLen, gene.y)\
        << QPointF(gene.x + gene.width, gene.y + gene.height/2)\
        << QPointF(gene.x+ gene.width-gene.tipLen, gene.y + gene.height)\
        << QPointF(gene.x, gene.y + gene.height)<< QPointF(gene.x, gene.y) ;
    else
        dim << QPointF(gene.x, gene.y + gene.height/2)\
            << QPointF(gene.x+gene.tipLen, gene.y)\
            << QPointF(gene.x + gene.width, gene.y)\
            << QPointF(gene.x + gene.width, gene.y + gene.height)\
            << QPointF(gene.x  + gene.tipLen, gene.y + gene.height);

    QGraphicsPolygonItem *p = new QGraphicsPolygonItem(dim);
    p->setPen( QPen(gene.color));
    p->setBrush( QBrush(gene.color));
    p->setToolTip(gene.annotation);

    return p;
}


GraphicsTaxonItem *GraphicsItemsCollection::getTaxonNode(TREENODE *node, double STARTX, double STARTY) {
    GraphicsTaxonItem *taxon;
    taxon = new GraphicsTaxonItem();
    taxon->radius = node->count > 1 ? log(node->count)*5 + 2 : 2 ;
    taxon->setRect(STARTX + node->depth + taxon->radius/2, STARTY + (node->Ydown + node->Yup)/2 + taxon->radius/2, taxon->radius, taxon->radius );
    taxon->name = node->name;
    taxon->depth = node->depth;
   // qDebug() << " Setting depth item " << taxon->depth << " from " << node->depth;
    taxon->setToolTip(taxon->name + " " + QString::number(node->depth + taxon->radius/2) + " " + QString::number((node->Ydown + node->Yup)/2 + taxon->radius/2) + " depth = " + QString::number(node->depth));
    return taxon;
}


GraphicsNameItem *GraphicsItemsCollection::getNameNode(GraphicsTaxonItem *taxon, double STARTX, double STARTY) {
    GraphicsNameItem *name= new GraphicsNameItem();
    name->setPlainText(taxon->name);
    return name;
}

GraphicsConnectorLines *GraphicsItemsCollection::getLineBetweenTaxonItems(GraphicsTaxonItem *a, GraphicsTaxonItem *b, LineStyle style, QPen pen) {
    GraphicsConnectorLines *connector = new GraphicsConnectorLines;
  //  qDebug() << " hello; ";
    connector->setStyle(style);
    if( style == STRAIGHT ) {
         QGraphicsLineItem *line = new QGraphicsLineItem(); //a->rect().left() + a->radius/2, a->rect().top() + a->radius/2, b->rect().left()+ a->radius/2, b->rect().top() + b->radius/2);
         line->setPen(pen);
         connector->addToGroup(line);
         connector->addLineSegment(line);
    }
    if( style == MANHATTAN) {
       //  qDebug() << " hello;1 ";
        QGraphicsLineItem *line = new QGraphicsLineItem() ; //a->rect().left() + a->radius/2, a->rect().top() + a->radius/2, b->rect().left()+ a->radius/2, a->rect().top() + a->radius/2);
        line->setPen(pen);
        connector->addToGroup(line);
        connector->addLineSegment(line);
 //qDebug() << " hello2 ";
        line = new QGraphicsLineItem(); //b->rect().left()+ a->radius/2, a->rect().top() + a->radius/2, b->rect().left()+ a->radius/2, b->rect().top() + b->radius/2);
        line->setPen( pen);
        connector->addToGroup(line);
        connector->addLineSegment(line);
       //  qDebug() << " hello 4 ";
    }
    connector->setZValue(-1);
   //  qDebug() << " hello 5 ";
    return connector;
}




