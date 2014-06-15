#ifndef GENEBROWSER_H
#define GENEBROWSER_H
#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <QRectF>
#include <QPointF>
#include <QSizeF>
#include <QEvent>
#include <QWheelEvent>
#include <QObject>
#include "genebrowser/graphicsitemscollection.h"
#include <math.h>

#include "helper/types.h"

class GraphicsNotchedLine;
class GraphicsGeneItems;
class GraphicsNameItem;
class GraphicsTextItems;

class GeneBrowser
{
   // Q_OBJECT
public:
    GeneBrowser(QGraphicsView *view);
    GeneBrowser();

    void setQGraphicsViewer(QGraphicsView *view);
    void setData(GeneBrowserData &data);
    void drawGenomeBrowser();
    void sanitizeORFData() ;
   // void addORFDiagrams(QList<ORFData> &orfs, STRAND strand, GENEPROPERTY &geneProp, PENPOSITION &pen);

    bool eventFilter(QObject *object, QEvent *event);
    void wheelEvent(QWheelEvent* event);

    static bool sortRankBeginPair(const RANK_BEGIN_PAIR &a, const RANK_BEGIN_PAIR &b);
private:
    unsigned int computeORFLevels(QList<ORFData> &orfs, STRAND strand);
    void computeCaptionParams(QList<ORFData> &orfs, CAPTIONPARAMS &captionParams);
    QGraphicsView *view;
    QGraphicsScene *gscene;
    GraphicsTextItems *captionItems;
    double  XSIZE, YSIZE;
    GeneBrowserData data;
    double scale, basePairToPixelRatio;
    double xstart, ystart;
    GraphicsNotchedLine *line;

    GraphicsGeneItems *revorfs, *fwdorfs;
};

#endif // GENEBROWSER_H
