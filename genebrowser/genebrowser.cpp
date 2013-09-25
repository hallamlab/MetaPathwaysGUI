#include "genebrowser.h"
#include <QToolTip>

GeneBrowser::GeneBrowser()
{
    scale =1;
    basePairToPixelRatio = 1;
    xstart= 20;
    ystart = 20;
    gscene=0;
    line=0;
}


GeneBrowser::GeneBrowser(QGraphicsView *view)
{
    scale =1;
    basePairToPixelRatio = 15;
    xstart= 20;
    ystart = 60;
    gscene =0;
    line=0;
    setQGraphicsViewer(view);
}


void GeneBrowser::setQGraphicsViewer(QGraphicsView *view) {
    this->view = view;
    XSIZE = this->view->size().width()*10000;
    YSIZE = this->view->size().height();
}

void GeneBrowser::setData(GeneBrowserData &data) {
    this->data = data;
}


void GeneBrowser::drawGenomeBrowser() {

    //if( data.seq.length > 50000) basePairToPixelRatio = 5;
   // this->XSIZE = this->data.seq.length*basePairToPixelRatio*1.05;
    unsigned int nomInterval = pow(10, (unsigned int) log10(this->data.seq.length/100));

    if( gscene==0) gscene = new QGraphicsScene(0,0,this->data.seq.length*basePairToPixelRatio*1.05 , this->YSIZE, this->view);

    view->setScene(gscene);


    PENPOSITION pen;
    pen.xstart = this->xstart;
    pen.ystart = this->xstart;

    GraphicsItemsCollection *itemCreator  = GraphicsItemsCollection::getGraphicsItemsCollection();

    unsigned int fwdMaxLevel = this->computeORFLevels(data.orfData, FORWARD);
    this->computeORFLevels(data.orfData, REVERSE);

    GENEPROPERTY geneProp;
    geneProp.tipLen = 10;
    geneProp.height = 10;
    geneProp.spaceAbove = 5;
    geneProp.spaceBelow = 5;
    geneProp.color = Qt::green;
    geneProp.basePairToPixelRatio = basePairToPixelRatio;


    fwdorfs = itemCreator->getORFDiagrams(data.orfData, FORWARD,geneProp, pen);
    gscene->addItem(this->fwdorfs);

    pen.ystart = pen.ystart + (fwdMaxLevel +1)*(geneProp.spaceAbove + geneProp.height + geneProp.spaceBelow);

    pen.ystart = pen.ystart +  2 + 1*basePairToPixelRatio;
    NotchedLineParams nparams;
    nparams.x = pen.xstart;
    nparams.y = pen.ystart;
    nparams.height = 2;
    nparams.width = basePairToPixelRatio*data.seq.length;
    nparams.pixInterval = nomInterval*basePairToPixelRatio;
    nparams.nomWidth = data.seq.length;
    nparams.notchHeight = 2 + 1*basePairToPixelRatio;
    nparams.basePairToPixelRatio = basePairToPixelRatio;

/*

     qDebug() << " Before drawing 3";
     qDebug() << "x " << nparams.x;
     qDebug() << "y " << nparams.y;
     qDebug() << "height " << nparams.height;
     qDebug() << "width " << nparams.width ;
     qDebug() << "pixinterval  " << nparams.pixInterval;
     qDebug() << "nomwidth " << nparams.nomWidth;
     qDebug() << "notch height " << nparams.notchHeight;
     qDebug() << "baseparitopixel " << nparams.basePairToPixelRatio;
*/

    this->line = itemCreator->getNotchedLine(nparams);
    gscene->addItem(this->line);

    pen.ystart = pen.ystart + 2*(2 + 1*basePairToPixelRatio);
    geneProp.color = Qt::red;

    this->revorfs = itemCreator->getORFDiagrams(data.orfData, REVERSE, geneProp, pen);
    gscene->addItem(this->revorfs);

    this->view->setAttribute( Qt::WA_AlwaysShowToolTips);
    gscene->setSceneRect(gscene->itemsBoundingRect());


}
/*
void GeneBrowser::addORFDiagrams(QList<ORFData> &orfs, STRAND strand, GENEPROPERTY &geneProp, PENPOSITION &pen)  {
    GraphicsItemsCollection *itemCreator  = GraphicsItemsCollection::getGraphicsItemsCollection();

    QGraphicsPolygonItem *p;


    foreach( ORFData orf, orfs) {
        if( orf.strand != strand ) continue;
            geneProp.x = pen.xstart + orf.start*geneProp.basePairToPixelRatio ;
            geneProp.y = pen.ystart +  orf._level*(geneProp.spaceAbove + geneProp.height + geneProp.spaceBelow) + geneProp.spaceAbove;
            geneProp.width = (orf.end - orf.start)*geneProp.basePairToPixelRatio;
            geneProp.width = geneProp.width*this->scale;
            p = itemCreator->getGeneShape(geneProp, strand);

            p->setToolTip(QString("hello"));
            qDebug() << "this is my tooltip " << p->toolTip();
            //p->setToolTip( "Function : " + orf.func_annot + "<br>" \
              //             + "Taxonomy : " + orf.tax_annot);
            gscene->addItem(p);
    }
}
*/

bool GeneBrowser::sortRankBeginPair(const RANK_BEGIN_PAIR &a, const RANK_BEGIN_PAIR &b) {
    if( a.begin < b.begin )
        return true;

    return false;
}

unsigned int GeneBrowser::computeORFLevels(QList<ORFData> &orfs, STRAND strand) {
    QVector<unsigned int> levelEnds;
    QVector<RANK_BEGIN_PAIR> ranks;

    for(unsigned int i=0; i <  orfs.size(); i++ ) {
        RANK_BEGIN_PAIR pair;
        pair.rank = i;
        pair.begin = orfs[i].start;
        ranks.push_back(pair);
    }

    qSort(ranks.begin(), ranks.end(), GeneBrowser::sortRankBeginPair);
    QVector<unsigned int> rankIndex;
    foreach(RANK_BEGIN_PAIR p, ranks) {
        rankIndex.push_back(p.rank );
    }

    unsigned int j=0;
    for(unsigned int i =0; i < rankIndex.size(); i++) {
        if( orfs[rankIndex[i]].strand != strand) continue;

        bool inserted = false;
        for(j=0; j < levelEnds.size(); j++ ) {
            if( orfs[rankIndex[i]].start > levelEnds[j]) {
                orfs[rankIndex[i]]._level = j;
                levelEnds[j]=orfs[rankIndex[i]].end ;
                inserted = true;
                break;
            }
        }
        if( !inserted ) {
            orfs[rankIndex[i]]._level = j;
            levelEnds.push_back( orfs[rankIndex[i]].end);
        }
    }

    unsigned int maxLevel = levelEnds.size()-1;
    if(strand==FORWARD) {
        for(unsigned int i =0; i < orfs.size(); i++) {
            orfs[i]._level = maxLevel - orfs[i]._level;
        }
    }
    return maxLevel;
}

void  GeneBrowser::eventFilter(QObject *object, QEvent *event){
    if (object==view->viewport() && event->type() == QEvent::Wheel){
        view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

        // Scale the view / do the zoom
        double scaleFactor = 1.05;
        QWheelEvent *qwe = dynamic_cast<QWheelEvent *>(event);
        if(qwe->delta() > 0) {
            view->scale(scaleFactor, 1);
        } else {
            view->scale(1.0 / scaleFactor, 1.0);
        }
    }
    //return true;
}

void GeneBrowser::wheelEvent(QWheelEvent* event) {

    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.05;
   // qDebug() << " scale value before " << this->scale;
    if(event->delta() > 0) {
        this->scale = this->scale*scaleFactor;
    } else {
        this->scale = this->scale/scaleFactor;
    }

   // qDebug() << " scale value " << this->scale;
    line->clearMarkers();
    line->insertMarkers(this->scale);;

    revorfs->clearORFs();
    revorfs->insertORFs(this->scale);
    fwdorfs->clearORFs();
    fwdorfs->insertORFs(this->scale);

    gscene->setSceneRect(gscene->itemsBoundingRect());
    view->update();


    // Don't call superclass handler here
    // as wheel is normally used for moving scrollbars
}
