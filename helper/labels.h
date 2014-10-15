#ifndef LABELS_H
#define LABELS_H

#include <QStringList>
#include <QHash>

#include "types.h"
#include "datamodel/datamodeltypes.h"


class Labels
{
public:
  static Labels* getLabels();
  static QStringList getHtableHeader(ATTRTYPE attrType, VALUETYPE  valutType);
  static QString getTabName(ATTRTYPE attrType);


private:
  Labels();
  static Labels *labels;
  static HTABLE_PARAMS  *tableParams;
  static QHash<ATTRTYPE, QString> *tabNames;
};

#endif // LABELS_H
