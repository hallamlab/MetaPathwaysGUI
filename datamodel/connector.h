#ifndef CONNECTOR_H
#define CONNECTOR_H
#include "datamodel/datamodeltypes.h"
#include <QHash>
#include <QList>
#include <QDebug>

// This class connects the attributes to a list of ORFs and maintains the knowledge
// connections
class Connector
{
public:
    Connector();
    ~Connector();
    void addToList(ATTRIBUTE *attribute, ORF *orf);
    QHash<ATTRIBUTE *, QList<ORF *> > connected;
    void setAttrType(ATTRTYPE attrType);
    ATTRTYPE getAttrType();
    QList<ORF *> getORFList(ATTRIBUTE *attribute);
    QList<ORF *> getORFList();
    QList<ORF *> getORFList(QList<ATTRIBUTE *> attributes);
    unsigned int getNumOfORFs(ATTRIBUTE *attribute);
    unsigned int getNumOfORFs();
    unsigned int getNumOfORFs(QList<ATTRIBUTE *> attributes);
    unsigned int getNumAttributes();
private:
    ATTRTYPE attrType;


    QString source, sink;
  //  QHash<ATTRIBUTE *, QList<ORF *> > connected;
    QList<ATTRIBUTE *> getSinks(ATTRIBUTE *);

};

#endif // CONNECTOR_H
