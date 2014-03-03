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
    QStringList getSampleNames();
    QString getHeader(unsigned int i) ;
    QString getSampleName(unsigned int i) ;
    enum TYPE getFieldType(unsigned int i);
    bool saveTableToFile(QString fileName, QChar delim, const QStringList &selectedHeaders);
    bool saveSequencesToFile(QString sampleName, QString fileName,  RESOURCE type);
    bool isMultiSampleMode();
    unsigned int getIndex();
    HTableData *getHTableDataPointer();
    TableData *getTableDataPointer();

private:
    unsigned int index;
    HTableData *td1;
    TableData *td0;
};

#endif // EXPORTSOURCE_H
