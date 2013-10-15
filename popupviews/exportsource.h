#ifndef EXPORTSOURCE_H
#define EXPORTSOURCE_H
#include "datamodel/htabledata.h"
#include "dataviews/tabledata.h"

class HTableData;
class TableData;

class ExportSource {
public:
    ExportSource(HTableData *td);
    ExportSource(TableData *td);
    QStringList getHeaders();
    QString getHeader(unsigned int i) ;
    enum TYPE getFieldType(unsigned int i);
    bool saveTableToFile(QString fileName, QChar delim);

private:
    unsigned int index;
    HTableData *td1;
    TableData *td0;
};

#endif // EXPORTSOURCE_H
