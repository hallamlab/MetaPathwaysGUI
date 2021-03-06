#ifndef DATAMODELTYPES_H
#define DATAMODELTYPES_H

#include <QString>
#include <QList>
#include <QHash>
#include <QDebug>

#include "helper/types.h"



typedef struct _LCARESULT {
    QString taxon;
    unsigned int count;
    unsigned int total;
    unsigned int valid;
} LCARESULT;

typedef struct _ATTRIBUTE {
    QString name, alias;
    ~_ATTRIBUTE() {}
} ATTRIBUTE;


typedef struct _HNODE HNODE;

typedef struct _HNODE {
    ATTRIBUTE *attribute;
    HNODE *parent;
    QList<HNODE *> children;
    short int depth;
    ~_HNODE() {

    }

} HNODE;

typedef enum _ATTRTYPE{ KEGG, COG, METACYC, SEED, CAZY, TAXON, RPKM, METACYCBASE, CUSTOM}  ATTRTYPE;



typedef struct _HTABLE_PARAMS {
    QHash<ATTRTYPE, QHash<VALUETYPE, QStringList> >  headers;
} HTABLE_PARAMS;



struct _ORF;
typedef struct _ORF ORF;

typedef struct _CONTIG {
    QList<ORF *> orfList;
    QString name;
    unsigned int length;
    ~_CONTIG(){

    }
} CONTIG;


typedef struct _ORF {
    QHash<ATTRTYPE, ATTRIBUTE*> attributes;
    CONTIG *contig;
    unsigned short int start, end, length;
    QString name;
    bool strand;
    float rpkm;

    _ORF() {
       rpkm=0;
    }

    ~_ORF() {
        /*
        if(this->contig!=0) {
           delete this->contig;
           this->contig = 0;
         }*/
     }

} ORF;


typedef struct _CATEGORYNODE {
    QString name;
    QString alias;
    short int depth;
} CATEGORYNODE;

#endif // DATAMODELTYPES_H
