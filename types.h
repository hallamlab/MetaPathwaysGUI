#ifndef TYPES_H
#define TYPES_H
#include <QString>
#include <QHash>
#include <QList>
#include <QGraphicsItem>



enum SOURCETYPE {FASTA, TABTABLE};

typedef struct _RANK_BEGIN_PAIR {
    unsigned int rank, begin;
} RANK_BEGIN_PAIR;

typedef enum { FORWARD, REVERSE } STRAND;

typedef struct _PENPOSITION {
    double xstart, ystart;
} PENPOSITION;

typedef struct _ORFData {
    unsigned int start, end;
    STRAND strand;
    QString func_annot, tax_annot, note;
    short int _level;

} ORFData;

typedef struct _SEQUENCEDATA {
    unsigned int start, end;
    unsigned int length;

} SequenceData;

typedef struct _GeneBrowserData {
    QList<ORFData>  orfData;
    SequenceData seq;
} GeneBrowserData;


typedef struct _NotchedLineParams {
    double x,y, width, height;
    double nomWidth, pixInterval;
    double notchWidth, notchHeight;
    double basePairToPixelRatio;
} NotchedLineParams;


typedef struct _Marker {
    QGraphicsLineItem *notch;
    QGraphicsTextItem *text;
} MARKER;




typedef struct LineProp {
    double x, y;
    double width, thickness;
} LINEPROPERTY;


typedef struct GeneProperty {
    double x, y;
    double height,  width;
    double geneLength;
    double tipLen;
    double spaceAbove, spaceBelow;
    double basePairToPixelRatio;
    enum Qt::GlobalColor color;
    QString annotation;
}  GENEPROPERTY;


#endif // TYPES_H
