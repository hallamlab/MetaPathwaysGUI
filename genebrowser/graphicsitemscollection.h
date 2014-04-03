#ifndef GRAPHICSITEMSCOLLECTION_H
#define GRAPHICSITEMSCOLLECTION_H
#include <QGraphicsItemGroup>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QVector>
#include <QGraphicsLineItem>
#include <QList>
#include <QGraphicsEllipseItem>
#include "types.h"
#include <QHash>
#include "genebrowser/genebrowser.h"
#include "dataviews/megandata.h"
#include "utilities.h"



class GraphicsNotchedLine: public QGraphicsItemGroup{

public:
    GraphicsNotchedLine() { hline=0; scale=1; }

    void addMarker(MARKER &item);
    void clearMarkers();
    void insertMarkers(double scale =1 );
    NotchedLineParams params;
    double nomInterval;
    double basePairsToPixel;

    QGraphicsRectItem *hline;
private:
    double nucleoToPixel;
    double lineWidth;
    double lineThickness;

    double interval;
    double notchHeight, notchThickness;

    double scale;

    QVector<MARKER> markers;
};




class GraphicsConnectorLines:public QGraphicsItemGroup  {
public:
    GraphicsConnectorLines();
    ~GraphicsConnectorLines();
    void addLineSegment(QGraphicsLineItem *line);
    void setStyle(LineStyle style);
    QList<QGraphicsLineItem *> segments;
    bool inscene;
private:
    LineStyle  style;

};



class GraphicsGeneItems: public QGraphicsItemGroup{

public:
    GraphicsGeneItems(QList<ORFData> &orfs, STRAND strand, GENEPROPERTY &geneProp, PENPOSITION &pen) ;

    void drawORFDiagrams();
    void clearORFs();
    void insertORFs(double scale = 1);
  //  bool inscene;

private:
    QVector<QGraphicsPolygonItem *> orfs;
    QList<ORFData>  orfData;
    STRAND strand;
    GENEPROPERTY geneProp;
    PENPOSITION pen;
    double scale;

};

class GraphicsNameItem: public QGraphicsTextItem {

public:
    GraphicsNameItem();
    QString name;
    bool inscene;

};

class GraphicsTaxonItem: public QGraphicsEllipseItem {

public:
    GraphicsTaxonItem();

   // int x, y,
    //width, height;
    QList<GraphicsTaxonItem *> Children;
    GraphicsNameItem *Name;
    unsigned int depth;
    QHash< LineStyle, QList<GraphicsConnectorLines *> > Lines;
 //   QList<GraphicsConnectorLines *> Lines;
    QString name;
    double radius;
    bool inscene;

};




typedef struct _TREENODE  TREENODE;
class GraphicsItemsCollection
{
public:
    static GraphicsItemsCollection *getGraphicsItemsCollection();
    GraphicsNotchedLine *getNotchedLine(NotchedLineParams &params);
    static QGraphicsRectItem *getLineShape(LINEPROPERTY &linesize);
    QGraphicsPolygonItem *getGeneShape(GENEPROPERTY &gene, STRAND strand = FORWARD);
    GraphicsGeneItems *getORFDiagrams(QList<ORFData> &orfs, STRAND strand, GENEPROPERTY &geneProp, PENPOSITION &pen);

    GraphicsConnectorLines *getLineBetweenTaxonItems(GraphicsTaxonItem *a, GraphicsTaxonItem *b, LineStyle style, QPen pen);

    GraphicsTaxonItem *getTaxonNode(TREENODE *node, double STARTX=0, double STARTY=0);
    GraphicsNameItem *getNameNode(GraphicsTaxonItem *taxon, double STARTX=0, double STARTY=0);

private:

    GraphicsItemsCollection();
    static GraphicsItemsCollection *graphicsItemsCreator;
    GraphicsTaxonItem *item;
};

#endif // GRAPHICSITEMSCOLLECTION_H
