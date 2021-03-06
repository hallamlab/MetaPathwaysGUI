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
    XSIZE = 800; //this->view->size().width()*10000;
    YSIZE = 500; //this->view->size().height();
}

void GeneBrowser::sanitizeORFData() {
    for(int i =0; i < this->data.orfData.size(); i++) {
        if( this->data.orfData[i].start > this->data.orfData[i].end ) {
            unsigned int temp;
            temp = this->data.orfData[i].start;
            this->data.orfData[i].start = this->data.orfData[i].end;
            this->data.orfData[i].end = temp;
        }
    }
}

void GeneBrowser::setData(GeneBrowserData &data) {
    this->data = data;

    this->sanitizeORFData();
}


void GeneBrowser::drawGenomeBrowser() {

    //if( data.seq.length > 50000) basePairToPixelRatio = 5;
   // this->XSIZE = this->data.seq.length*basePairToPixelRatio*1.05;

    this->basePairToPixelRatio = (double)this->XSIZE/(double)this->data.seq.length;

    unsigned int nomInterval = 50; //pow(10, (unsigned int) log10(this->data.seq.length/100));

   // if( gscene==0) gscene = new QGraphicsScene(0,0,this->data.seq.length*basePairToPixelRatio*1.05 , this->YSIZE, this->view);
    if( gscene==0) gscene = new QGraphicsScene(0,0,this->XSIZE*1.2, this->YSIZE*1.2, this->view);
    view->setStyleSheet("QWidget {padding:10px; margin-left:0px;}");

    view->setScene(gscene);


    PENPOSITION pen;
    pen.xstart = this->XSIZE*0.1;
    pen.ystart = this->ystart;

    GraphicsItemsCollection *itemCreator  = GraphicsItemsCollection::getGraphicsItemsCollection();

    // Create the CAPTION
    CAPTIONPARAMS captionParams;
    captionParams.x = pen.xstart;
    captionParams.y = pen.ystart + 5;
    captionParams.spaceAbove = 1;
    captionParams.spaceBelow = 1;

    this->computeCaptionParams(data.orfData, captionParams);
    this->captionItems = itemCreator->getCaptionItems(captionParams);
    gscene->addItem(this->captionItems);


    pen.ystart = captionParams.y;

    // Compute the Levels of the ORFs for both forward and reverse
    unsigned int fwdMaxLevel = this->computeORFLevels(data.orfData, FORWARD);
    this->computeORFLevels(data.orfData, REVERSE);


    // Prepare the ORF properties
    GENEPROPERTY geneProp;
    geneProp.tipLen = 10;
    geneProp.height = 10;
    geneProp.spaceAbove = 5;
    geneProp.spaceBelow = 5;
    geneProp.basePairToPixelRatio = basePairToPixelRatio;


    // Draw the forward ORFs
    geneProp.color = Qt::blue;
    fwdorfs = itemCreator->getORFDiagrams(data.orfData, FORWARD,geneProp, pen);
    gscene->addItem(this->fwdorfs);


   // Draw the notched line with contig and  notches
    pen.ystart = pen.ystart + (fwdMaxLevel +1)*(geneProp.spaceAbove + geneProp.height + geneProp.spaceBelow);
    pen.ystart = pen.ystart +  2 + 1*basePairToPixelRatio;

    NotchedLineParams nparams;
    nparams.x = pen.xstart;
    nparams.y = pen.ystart;
    nparams.height = 2;
    nparams.width = basePairToPixelRatio*data.seq.length;
    nparams.pixInterval = nomInterval*basePairToPixelRatio;
    nparams.nomWidth = data.seq.length;
    nparams.notchHeight = 10 + 1*basePairToPixelRatio;
    nparams.basePairToPixelRatio = basePairToPixelRatio;

    this->line = itemCreator->getNotchedLine(nparams);
    gscene->addItem(this->line);



    // Draw the reverse ORF diagrams
    pen.ystart = pen.ystart + 2*(2 + 1*basePairToPixelRatio);
    geneProp.color = Qt::magenta;
    this->revorfs = itemCreator->getORFDiagrams(data.orfData, REVERSE, geneProp, pen);
    gscene->addItem(this->revorfs);

    this->view->setAttribute( Qt::WA_AlwaysShowToolTips);
    gscene->setSceneRect(gscene->itemsBoundingRect());
    this->view->setMinimumSize(gscene->sceneRect().width(), gscene->sceneRect().height());


}


bool GeneBrowser::sortRankBeginPair(const RANK_BEGIN_PAIR &a, const RANK_BEGIN_PAIR &b) {
    if( a.begin < b.begin )
        return true;

    return false;
}

/**
 * @brief compareFreq compares to taxon based on the taxon frequency
 * @param a, first taxon
 * @param b, second taxon
 * @return
 */
bool compareFreq(const  TaxonFreqQPair &a, const TaxonFreqQPair &b) {
    //qDebug() << a->strTemp << "  " << b->strTemp << "  " << QString::compare(a->strTemp,   b->strTemp);
    return a.second  > b.second;
}


/**
 * @brief GeneBrowser::computeCaptionParams, computes the caption for the sequence name and
 * the taxon freqeuncy and picks the top few
 * @param orfs the orf data
 * @param captionParams, the parameters
 */

void GeneBrowser::computeCaptionParams(QList<ORFData> &orfs, CAPTIONPARAMS &captionParams) {

    unsigned int _maxorfs = 5;
    QRegExp all("^all$");
    QRegExp root("^root$");

    QHash<QString, unsigned int> freq;
    foreach( ORFData orf, orfs) {
        if( all.indexIn(orf.tax_annot) != -1) continue;
        if( root.indexIn(orf.tax_annot) != -1) continue;

        if( !freq.contains(orf.tax_annot)) freq.insert(orf.tax_annot, 0);
        freq[orf.tax_annot]++;
    }


    QList<TaxonFreqQPair > taxonFreqList;
    foreach(QString key, freq.keys()) {
        taxonFreqList.append(TaxonFreqQPair(key, freq[key]) );
    }
    qSort(taxonFreqList.begin(), taxonFreqList.end(), compareFreq);

    unsigned int i =0;
    foreach(TaxonFreqQPair taxon, taxonFreqList  ) {
        if(i > _maxorfs ) break;
        qDebug() << taxon.first << "  " << taxon.second;
        captionParams.taxonFreq.append(taxon);
        i++;
    }
    captionParams.maxpairs = 5;
    captionParams.contigName = this->data.seq.name;

}

unsigned int GeneBrowser::computeORFLevels(QList<ORFData> &orfs, STRAND strand) {
    QVector<unsigned int> levelEnds;
    QVector<RANK_BEGIN_PAIR> ranks;

    for(int i=0; i <  orfs.size(); i++ ) {
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

    int j=0;
    for(int i =0; i < rankIndex.size(); i++) {
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
        for(int i =0; i < orfs.size(); i++) {
            orfs[i]._level = maxLevel - orfs[i]._level;
        }
    }
    return maxLevel;
}

bool GeneBrowser::eventFilter(QObject *object, QEvent *event){
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

        return true;
    }else {

         return false;
    }

    //return true;
}

void GeneBrowser::wheelEvent(QWheelEvent* event) {

    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.05;

    if(event->delta() > 0) {
        this->scale = this->scale*scaleFactor;
    } else {
        this->scale = this->scale/scaleFactor;
    }

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
