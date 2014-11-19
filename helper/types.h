#ifndef TYPES_H
#define TYPES_H
#include <QString>
#include <QHash>
#include <QList>
#include <QPair>
#include <QGraphicsItem>

typedef enum _STATUS_SYMBOL {REDCROSS, GREENCHECK, LOADING, UNSURE, PARTIAL}  STATUS_SYMBOL;

const QString  SELECT_SAMPLE_TAG = "Select sample";
enum TYPE {INT, DOUBLE, STRING, UNKNOWN};
enum OPTYPE{ OR, AND};
enum DECISION { YES, NO, MAYBE};
typedef enum { MANHATTAN, STRAIGHT } LineStyle;
typedef enum _SETTING {_CONFIG, _PARAMS, _TEMP_CONFIG, _PARAMS_DEFAULT } SETTING_TYPE;


enum SOURCETYPE {FASTA, TABTABLE};

typedef enum _INPUTFILETYPE{ NUCFASTA, AMINOFAA, NUCFNA, CONTIGLENGTH, ORFLENGTH, \
                             NUCSTATS, AMINOSTATS, MEGANTREE, FUNCTIONALTABLE,\
                             FUNCTIONAL_SRC1, ORFTABLE, ORFMETACYC, ORFRPKM, MEGANLASTFILE,\
                             MEGANBLASTFILE, RUNSTATS, ERRORS, GLOBAL_ERRORS , rRNATABLES,\
                             tRNATABLES, rRNA, tRNA, BLAST_RESULTS_FOLDER} RESOURCE;

typedef enum _RESOURCETYPE{ NCBITREEFILE, NCBINAMEMAPFILE} RESOURCETYPE;

#ifdef LCASTAR
typedef enum _VALUETYPE { ORFCOUNT, LCASTAR, RPKMCOUNT} VALUETYPE;
#else
typedef enum _VALUETYPE { ORFCOUNT, RPKMCOUNT, BASEPWY_ORF,  BASEPWY_RXNTOT,  BASEPWY_RXNCOV} VALUETYPE;
#endif

/**
 * the types for the export boxes
 */
typedef enum _TABLEEXPORTTYPE {
    rRNATABLEEXP, tRNATABLEEXP, OTHERSTABLEEXP
} TABLEEXPORTTYPE;

typedef enum _TABLETYPE {
    rRNATABLE, tRNATABLE, OTHERSTABLE
} TABLETYPE;

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
    QString name;
    QString func_annot, tax_annot, note;
    short int _level;

} ORFData;

typedef struct _SEQUENCEDATA {
    unsigned int start, end;
    unsigned int length;
    QString name;

} SequenceData;


typedef QPair<QString, unsigned int > TaxonFreqQPair;


template <class T, class U, class V>
struct _Triplet {
    int first;
    QString second;
    QString third;
};


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

// hold caption information for a geneviewer
typedef struct _CAPTION_PARAMS {
    QString contigName;
    QList<QPair< QString, unsigned int> > taxonFreq;
    int maxpairs;
    double x, y;
    double spaceAbove, spaceBelow;
} CAPTIONPARAMS;

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


typedef struct _SEARCH {
    QString searchItem;
    enum OPTYPE type;
    QList<unsigned int> searchCols;
    bool caseSensitive;


} SEARCH;


const QString PIPELINE_STEP_BLAST = "metapaths_steps:BLAST_REFDB";





#endif // TYPES_H
