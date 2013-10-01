#include "genomeview.h"
#include "genebrowser/genebrowser.h"
GenomeView::GenomeView()
{

  //  this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);

//    qDebug() << " Geometry X " << this->size();

    gBrowser = new GeneBrowser(this);

}

void GenomeView::showData(GeneBrowserData &gbdata) {
    this->gBrowser->setData(gbdata);
    this->gBrowser->drawGenomeBrowser();
}


void GenomeView::wheelEvent(QWheelEvent* event) {
    gBrowser->wheelEvent(event);
}



void GenomeView::itemGenomeDisplay(QTableWidgetItem * item) {
    static  int column= -1;
    static  int row = -1;


    if( !(column==item->column() && row == item->row()) )  {
       column = item->column();
       row = item->row();
       return;
    }

    this->show();
}


