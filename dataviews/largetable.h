#ifndef LARGETABLE_H
#define LARGETABLE_H

#include <QList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#include <QtCore>
#include <QChar>


typedef struct row{
    QList<int> intVar;
    QList<double> doubleVar;
    QList<QString>  strVar;
    int intTemp;
    unsigned int rank;
    double doubleTemp;
    QString strTemp;
} ROW;


enum TYPE {INT, DOUBLE, STRING, UNKNOWN};
enum OPTYPE{ OR, AND};
enum DECISION { YES, NO, MAYBE};


typedef struct _SEARCH {
    QList<QString> searchItems;
    enum OPTYPE type;
    QList<unsigned int> searchCols;
    bool caseSensitive;

} SEARCH;

class LargeTable
{
public:
    LargeTable();
    LargeTable(unsigned int i);
    LargeTable(const QString filename, const QChar delim, bool ignoreComments, bool firstRowAsHeaders, QList<TYPE> types = QList<TYPE>());
    ~LargeTable();

    int readDataFile(const QString fileName, const QChar delim,  const bool ignoreComments = true, const bool firstRowAsHeaders = true);
    void getData(QList<ROW *> &data,  int pivotPoint = 0, unsigned int deltaW = 100);
    unsigned int getBegData( int pivotPoint = 0, unsigned int deltaW = 100);
    void sortByField(unsigned int fieldNum);

    void copyDataToSort(unsigned int k);

    int getPivot();
    void setPivot(int pivot);
    void markRowsSearch(SEARCH &searchFilter);
    void reRank();
//

    QList< ROW *> tableData;
    QList<ROW *>  wholeTableData;
    QStringList colNames;

    unsigned int lastUsedField;
    unsigned int numCols;
    enum TYPE *types;
    unsigned int *varType;
    unsigned int *index;
    bool sortingDirectionForward;

private:
    int pivotPointer;

};

#endif // LARGETABLE_H
