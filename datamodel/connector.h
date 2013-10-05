#ifndef CONNECTOR_H
#define CONNECTOR_H
#include "datamodel/datamodeltypes.h"
#include <QHash>
#include <QList>
#include <QDebug>

class Connector
{
public:
    Connector();
    void addToList(ATTRIBUTE *attribute, ORF *orf);
    QHash<ATTRIBUTE *, QList<ORF *> > connected;
    void setAttrType(ATTRTYPE attrType);
    ATTRTYPE getAttrType();
    QList<ORF *> getORFList(ATTRIBUTE *attribute);
    QList<ORF *> getORFList();
    QList<ORF *> getORFList(QList<ATTRIBUTE *> attribute);
private:
    ATTRTYPE attrType;

    QString source, sink;
  //  QHash<ATTRIBUTE *, QList<ORF *> > connected;
    QList<ATTRIBUTE *> getSinks(ATTRIBUTE *);

};

#endif // CONNECTOR_H
