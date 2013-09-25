#include "genomeview.h"
#include "genebrowser/genebrowser.h"
GenomeView::GenomeView()
{

    this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);


//    qDebug() << " Geometry X " << this->size();

    gBrowser = new GeneBrowser(this);

/*

    SequenceData s;
    s.start =0;
    s.end = 900;
    s.length = s.end - s.start + 1;

    GeneBrowserData gbd;
    gbd.seq = s;

    ORFData orf;
    orf.start = 20; orf.end= 60; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="20-60+";
    gbd.orfData.append(orf);

    orf.start = 45; orf.end= 55; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="45-55+";
    gbd.orfData.append(orf);

    orf.start = 65; orf.end= 95; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="65-95+";
    gbd.orfData.append(orf);

    orf.start = 85; orf.end= 108; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="85-108+";
    gbd.orfData.append(orf);

    orf.start = 90; orf.end= 109; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="90-109+";
    gbd.orfData.append(orf);

    orf.start = 120; orf.end= 180; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="120-180+";
    gbd.orfData.append(orf);

    orf.start = 160; orf.end= 210; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="160-210+";
    gbd.orfData.append(orf);

    orf.start = 180; orf.end= 250; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="180-250+";
    gbd.orfData.append(orf);

    orf.start = 190; orf.end= 360; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="190-360+";
    gbd.orfData.append(orf);

    orf.start = 340; orf.end= 410; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="340-410+";
    gbd.orfData.append(orf);

    orf.start = 430; orf.end= 490; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="430-490+";
    gbd.orfData.append(orf);

    orf.start = 450; orf.end= 480; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="450-480+";
    gbd.orfData.append(orf);

    orf.start = 455; orf.end= 478; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="455-478+";
    gbd.orfData.append(orf);

    orf.start = 610; orf.end= 800; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="610-800+";
    gbd.orfData.append(orf);

    orf.start = 750; orf.end= 810; orf.strand =FORWARD;
    orf.func_annot=""; orf.tax_annot=""; orf.note="750-810+";
    gbd.orfData.append(orf);

    orf.start = 20; orf.end= 60; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="20-60-";
    gbd.orfData.append(orf);

    orf.start = 45; orf.end= 55; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="45-55-";
    gbd.orfData.append(orf);

    orf.start = 65; orf.end= 95; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="65-95-";
    gbd.orfData.append(orf);

    orf.start = 85; orf.end= 108; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="85-108-";
    gbd.orfData.append(orf);

    orf.start = 90; orf.end= 109; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="90-109-";
    gbd.orfData.append(orf);

    orf.start = 120; orf.end= 180; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="120-180-";
    gbd.orfData.append(orf);

    orf.start = 160; orf.end= 210; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="160-210-";
    gbd.orfData.append(orf);

    orf.start = 180; orf.end= 250; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="180-250-";
    gbd.orfData.append(orf);

    orf.start = 190; orf.end= 360; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="190-360-";
    gbd.orfData.append(orf);

    orf.start = 340; orf.end= 410; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="340-410-";
    gbd.orfData.append(orf);

    orf.start = 430; orf.end= 490; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="430-490-";
    gbd.orfData.append(orf);

    orf.start = 450; orf.end= 480; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="450-480-";
    gbd.orfData.append(orf);

    orf.start = 455; orf.end= 478; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="455-478-";
    gbd.orfData.append(orf);

    orf.start = 610; orf.end= 800; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="610-800-";
    gbd.orfData.append(orf);

    orf.start = 750; orf.end= 810; orf.strand =REVERSE;
    orf.func_annot=""; orf.tax_annot=""; orf.note="750-810-";
    gbd.orfData.append(orf);


  //  gBrowser->setData(gbd);
   // gBrowser->drawGenomeBrowser();
   */
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

    qDebug() << " you just clicked";
    if( !(column==item->column() && row == item->row()) )  {
       column = item->column();
       row = item->row();
       return;
    }
    qDebug() << "ready to go";
    this->show();
}


