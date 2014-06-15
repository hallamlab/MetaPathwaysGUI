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
#include <QChar>

#include "helper/types.h"


typedef struct row{
    QList<int> intVar;
    QList<double> doubleVar;
    QList<QString>  strVar;
    int intTemp;
    unsigned int rank;
    double doubleTemp;
    QString strTemp;
} ROW;


class LargeTable
{
public:
    LargeTable();
    LargeTable(unsigned int i);
    LargeTable(const QString filename, const QChar delim, bool ignoreComments, bool firstRowAsHeaders, QList<TYPE> types = QList<TYPE>());
    LargeTable(const QString filename, const QChar delim, bool ignoreComments, bool firstRowAsHeaders, QList<TYPE> &types, QList<unsigned int> &columns, QRegExp filter);
    ~LargeTable();

    int readDataFile(const QString fileName, const QChar delim,  const bool ignoreComments = true, const bool firstRowAsHeaders = true);
    int readDataFile(const QString fileName, const QChar delim,  QList<unsigned int> &columns, QRegExp filter, const bool ignoreComments = true, const bool firstRowAsHeaders = true);
    void getData(QList<ROW *> &data,  int pivotPoint = 0, unsigned int deltaW = 100);
    unsigned int getBegData( int pivotPoint = 0, unsigned int deltaW = 100);
    void sortByField(unsigned int fieldNum);

    void copyDataToSort(unsigned int k);

    int getPivot();
    void setPivot(int pivot);
    void markRowsSearch(SEARCH &searchFilter);
    void reRank();
    void getSelectRows(QList<ROW *> & sourcerows,  QList<ROW *> & selectrows, QString search, unsigned int col, enum TYPE type);

    QList< ROW *> tableData;   // stores the filtered data
    QList<ROW *>  wholeTableData;  //stores all the data, and copied to the tableData when filtered
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
