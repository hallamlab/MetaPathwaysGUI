#ifndef DATAMODELTYPES_H
#define DATAMODELTYPES_H

#include <QString>
#include <QList>
#include <QHash>
#include <QDebug>



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
} HNODE;

typedef enum _ATTRTYPE{ KEGG, COG, METACYC, SEED, TAXON, RPKM}  ATTRTYPE;



typedef struct _HTABLE_PARAMS {
    QHash<ATTRTYPE, QStringList>  headers;
} HTABLE_PARAMS;



struct _ORF;
typedef struct _ORF ORF;

typedef struct _CONTIG {
    QList<ORF *> orfList;
    QString name;
    unsigned int length;
} CONTIG;


typedef struct _ORF {
    QHash<ATTRTYPE, ATTRIBUTE*> attributes;
    CONTIG *contig;
    unsigned int start, end, length;
    QString name;
    bool strand;
    float rpkm;


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
