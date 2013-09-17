#include "graphicsitemscollection.h"


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
        p->setToolTip(orf.note);
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


