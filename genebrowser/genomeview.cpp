#include "genomeview.h"
#include "genebrowser/genebrowser.h"
#include <QScrollArea>
#include <QScrollBar>
GenomeView::GenomeView()
{

   // this->setDragMode(QGraphicsView::ScrollHandDrag);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   // this->setHorizontalScrollBar();
    //this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);
    this->setInteractive(true);
    gBrowser = new GeneBrowser(this);
  //  this->viewport()->installEventFilter(gBrowser);
   // this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

}

void GenomeView::showData(GeneBrowserData &gbdata) {
    this->gBrowser->setData(gbdata);
    this->setWindowTitle(gbdata.seq.name);
    this->gBrowser->drawGenomeBrowser();
}


void GenomeView::wheelEvent(QWheelEvent* event) {
    gBrowser->wheelEvent(event);
    /*
    qDebug() << "Wheeling";
    qDebug() << this->horizontalScrollBar()->minimum() << "  " << this->horizontalScrollBar()->maximum();
    qDebug() << this->horizontalScrollBar()->value();
    */
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


