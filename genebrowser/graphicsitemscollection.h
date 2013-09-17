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


#include "genebrowser/genebrowser.h"



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




class GraphicsGeneItems: public QGraphicsItemGroup{

public:
    GraphicsGeneItems(QList<ORFData> &orfs, STRAND strand, GENEPROPERTY &geneProp, PENPOSITION &pen) ;

    void drawORFDiagrams();
    void clearORFs();
    void insertORFs(double scale = 1);

private:
    QVector<QGraphicsPolygonItem *> orfs;
    QList<ORFData>  orfData;
    STRAND strand;
    GENEPROPERTY geneProp;
    PENPOSITION pen;
    double scale;

};





class GraphicsItemsCollection
{
public:
    static GraphicsItemsCollection *getGraphicsItemsCollection();
    GraphicsNotchedLine *getNotchedLine(NotchedLineParams &params);
    static QGraphicsRectItem *getLineShape(LINEPROPERTY &linesize);
    QGraphicsPolygonItem *getGeneShape(GENEPROPERTY &gene, STRAND strand = FORWARD);
    GraphicsGeneItems *getORFDiagrams(QList<ORFData> &orfs, STRAND strand, GENEPROPERTY &geneProp, PENPOSITION &pen);

private:

    GraphicsItemsCollection();
    static GraphicsItemsCollection *graphicsItemsCreator;
};

#endif // GRAPHICSITEMSCOLLECTION_H
