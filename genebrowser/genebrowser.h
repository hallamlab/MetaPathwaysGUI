#ifndef GENEBROWSER_H
#define GENEBROWSER_H
#include <QGraphicsView>
#include "types.h"

#include <QGraphicsItemGroup>
#include <QRectF>
#include <QPointF>
#include <QSizeF>
#include <QEvent>
#include <QWheelEvent>
#include <QObject>
#include "genebrowser/graphicsitemscollection.h"

class GraphicsNotchedLine;
class GraphicsGeneItems;

class GeneBrowser
{
public:
    GeneBrowser(QGraphicsView *view);
    GeneBrowser();

    void setQGraphicsViewer(QGraphicsView *view);
    void setData(GeneBrowserData &data);
    void drawGenomeBrowser();
    void addORFDiagrams(QList<ORFData> &orfs, STRAND strand, GENEPROPERTY &geneProp, PENPOSITION &pen);

    void eventFilter(QObject *object, QEvent *event);
    void wheelEvent(QWheelEvent* event);

    static bool sortRankBeginPair(const RANK_BEGIN_PAIR &a, const RANK_BEGIN_PAIR &b);
private:
    unsigned int computeORFLevels(QList<ORFData> &orfs, STRAND strand);
    QGraphicsView *view;
    QGraphicsScene *gscene;
    unsigned int XSIZE, YSIZE;
    GeneBrowserData data;
    double scale, basePairToPixelRatio;
    double xstart, ystart;
    GraphicsNotchedLine *line;
    GraphicsGeneItems *revorfs, *fwdorfs;
};

#endif // GENEBROWSER_H
