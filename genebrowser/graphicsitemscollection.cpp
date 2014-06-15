#include "graphicsitemscollection.h"
#include <math.h>

GraphicsItemsCollection *GraphicsItemsCollection::graphicsItemsCreator =0;
GraphicsItemsCollection::GraphicsItemsCollection() {


}

/**
 * @brief GraphicsItemsCollection::getGraphicsItemsCollection, the constructor to
 * GraphicsItemsCollection that creates different graphics items
 * @return the pointer to the graphicsitemscollection (the creator of objects to draw)
 */
GraphicsItemsCollection *GraphicsItemsCollection::getGraphicsItemsCollection() {
    if( graphicsItemsCreator==0 )
        graphicsItemsCreator = new GraphicsItemsCollection();

    return graphicsItemsCreator;

}
/**
  * @brief GraphicsItemsCollection::getCaptionItems, gets a colection of texts for contig name
  * and tthe taxonomic distribution
  * @param captionParams
  * @return the group of textitems.
  */

 GraphicsTextItems *GraphicsItemsCollection::getCaptionItems(CAPTIONPARAMS &captionParams) {
     GraphicsTextItems *nameItem = new GraphicsTextItems();


     QGraphicsTextItem *p;
     p = new QGraphicsTextItem();
     p->setPlainText(QString("Contig : " ) + captionParams.contigName);
     p->setPos( QPointF(captionParams.x, captionParams.y) );
     nameItem->addToGroup(p);
     captionParams.y += p->boundingRect().width()/8 + captionParams.spaceBelow;

     if( captionParams.taxonFreq.size() > 0 ) {
         p = new QGraphicsTextItem();
         p->setPlainText( QString("Dominant Taxons") );
         p->setPos( QPointF(captionParams.x, captionParams.y) );
         nameItem->addToGroup(p);
         captionParams.y += p->boundingRect().width()/8 + captionParams.spaceBelow/2;
     }

     foreach( TaxonFreqQPair pair, captionParams.taxonFreq ) {
         p = new QGraphicsTextItem();
         p->setPlainText( QString("   %1 : %2").arg(  QString(pair.first + QString("                    ") ).left(20), QString::number(pair.second))  );
         p->setPos( QPointF(captionParams.x, captionParams.y) );
         nameItem->addToGroup(p);
         captionParams.y += p->boundingRect().width()/8 + captionParams.spaceBelow/2;
     }


     return nameItem;
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
    int j =0;
    for(unsigned int i=0; i*adjNomInterval < this->params.nomWidth; i = i + static_cast<int>(this->params.nomWidth/adjNomInterval) ) {
        lineSize.x = this->params.x + i*adjNomInterval*this->scale*this->params.basePairToPixelRatio;
        lineSize.thickness = this->params.height;

        marker.notch = new QGraphicsLineItem(lineSize.x, lineSize.y, lineSize.x, lineSize.y -this->params.notchHeight);
        marker.text  = new QGraphicsTextItem();
        marker.text->setPlainText( QString::number( (unsigned int)(i*adjNomInterval)));

        deltax =  marker.text->boundingRect().width()/2;

        marker.text->setPos( QPointF(lineSize.x + (j-1)*deltax, lineSize.y) );
        j++;

        this->addToGroup(marker.notch);
        this->addToGroup(marker.text);
        this->addMarker(marker);
    }
}


/**
 * @brief GraphicsNotchedLine::addMarker, adds a marker
 * @param item
 */

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

/***********************   GRAPHICSTEXTITEMS ***********************/
 void GraphicsTextItems::createTextItems(CAPTIONPARAMS captionParams) {



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
        QList<QString> list2;
        QList< QList< QString > > list1;

        list2.append("ORF Name");
        list2.append(orf.name);
        list1.append(list2);
        list2.clear();

        list2.append("Function");
        list2.append(orf.func_annot);
        list1.append(list2);
        list2.clear();
        list2.append("Taxonomy");
        list2.append(orf.tax_annot);
        list1.append(list2);

        list2.clear();
        list2.append("(Start, End)");
        list2.append(QString("(") + QString::number(orf.start) + ", " + QString::number(orf.end) + QString(")"));
        list1.append(list2);

        p->setToolTip( Utilities::createToolTipTable(list1));

       // p->setToolTip("Function : " + orf.func_annot + "<br>" \
         //       + "Taxonomy : " + orf.tax_annot);
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
    if( strand == FORWARD) {
        if( gene.width > gene.tipLen)
            dim << QPointF(gene.x, gene.y)\
                << QPointF(gene.x + gene.width-gene.tipLen, gene.y)\
                << QPointF(gene.x + gene.width, gene.y + gene.height/2)\
                << QPointF(gene.x+ gene.width-gene.tipLen, gene.y + gene.height)\
                << QPointF(gene.x, gene.y + gene.height)<< QPointF(gene.x, gene.y) ;
        else
            dim << QPointF(gene.x, gene.y)\
                << QPointF(gene.x + gene.width, gene.y + gene.height/2)\
                << QPointF(gene.x, gene.y + gene.height)<< QPointF(gene.x, gene.y) ;
    }
    else
        if( gene.width > gene.tipLen)
            dim << QPointF(gene.x, gene.y + gene.height/2)\
                << QPointF(gene.x+gene.tipLen, gene.y)\
                << QPointF(gene.x + gene.width, gene.y)\
                << QPointF(gene.x + gene.width, gene.y + gene.height)\
                << QPointF(gene.x  + gene.tipLen, gene.y + gene.height);
        else
            dim << QPointF(gene.x, gene.y + gene.height/2)\
                << QPointF(gene.x + gene.width, gene.y)\
                << QPointF(gene.x  + gene.width, gene.y + gene.height);

    QGraphicsPolygonItem *p = new QGraphicsPolygonItem(dim);
    p->setPen( QPen(gene.color));
    p->setBrush( QBrush(gene.color));
    p->setToolTip(gene.annotation);

    return p;
}


GraphicsTaxonItem *GraphicsItemsCollection::getTaxonNode(TREENODE *node, double STARTX, double STARTY) {
    GraphicsTaxonItem *taxon;
    taxon = new GraphicsTaxonItem();
    taxon->radius = node->count > 1 ? log10(node->count)*5 + 5 : 5 ;
    taxon->setRect(STARTX + node->depth + taxon->radius/2, STARTY + (node->Ydown + node->Yup)/2 + taxon->radius/2, taxon->radius, taxon->radius );
    taxon->name = node->name;
    taxon->depth = node->depth;
   // taxon->setPen();
   // qDebug() << " Setting depth item " << taxon->depth << " from " << node->depth;
   // taxon->setToolTip(taxon->name + " " + QString::number(node->depth + taxon->radius/2) + " " + QString::number((node->Ydown + node->Yup)/2 + taxon->radius/2) + " depth = " + QString::number(node->depth));

    QString string;
    string = "<table border=\"\" frame=void> \
            <tr> \
            <td>row 1, cell 1</td> \
            <td>row 1, cell 2</td> \
            </tr> \
            <tr> \
            <td>row 2, cell 1</td> \
            <td>row 2, cell 2</td> \
            </tr> \
            </table>";


    taxon->setToolTip(string);
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




