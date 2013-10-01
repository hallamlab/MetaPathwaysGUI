#ifndef CONNECTOR_H
#define CONNECTOR_H
#include "datamodel/datamodeltypes.h"
#include <QHash>
#include <QList>

class Connector
{
public:
    Connector();
    void addToList(ATTRIBUTE *attribute, ORF *orf);
        QHash<ATTRIBUTE *, QList<ORF *> > connected;
private:
    QString source, sink;
  //  QHash<ATTRIBUTE *, QList<ORF *> > connected;
    QList<ATTRIBUTE *> getSinks(ATTRIBUTE *);

};

#endif // CONNECTOR_H
