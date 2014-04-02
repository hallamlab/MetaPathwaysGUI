#ifndef DATAMODELTYPES_H
#define DATAMODELTYPES_H

#include <QString>
#include <QList>
#include <QHash>
#include <QDebug>



typedef struct _ATTRIBUTE {
    QString name, alias;
    ~_ATTRIBUTE() {}
} ATTRIBUTE;


typedef struct _CONTIG {
    QString name;
    unsigned int length;
} CONTIG;


typedef struct _HNODE HNODE;

typedef struct _HNODE {
    ATTRIBUTE *attribute;
    HNODE *parent;
    QList<HNODE *> children;
    short int depth;
} HNODE;

typedef enum _ATTRTYPE{ KEGG, COG, METACYC, SEED, TAXON}  ATTRTYPE;

typedef struct _ORF {
    QHash<ATTRTYPE, ATTRIBUTE*> attributes;
    CONTIG *contig;
    unsigned int start, end, length;
    QString name;
    bool strand;


    ~_ORF() {
        if(this->contig) {
           delete this->contig;
           this->contig = NULL;
         }
    }

} ORF;

typedef struct _CATEGORYNODE {
    QString name;
    QString alias;
    short int depth;
} CATEGORYNODE;

#endif // DATAMODELTYPES_H
