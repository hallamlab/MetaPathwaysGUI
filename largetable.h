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
} ROW;

enum TYPE {INT, DOUBLE, STRING};

class LargeTable
{
public:
    LargeTable();
    LargeTable(unsigned int i);
    LargeTable(const QString filename, const QChar delim, bool ignoreComments, bool firstRowAsHeaders, QList<TYPE> types = QList<TYPE>());

    int readDataFile(const QString fileName, const QChar delim,  const bool ignoreComments = true, const bool firstRowAsHeaders = true);
    void getData(QList<ROW *> &data,  int pivotPoint = 0, unsigned int deltaW = 100);
    unsigned int getBegData( int pivotPoint = 0, unsigned int deltaW = 100);
    void sortByField(unsigned int fieldNum);

    int getPivot();
    void setPivot(int pivot);


    QList< ROW *> tableData;
    QStringList *colNames;

    unsigned int numCols;
    enum TYPE *types;
    unsigned int *varType;
    unsigned int *index;

private:
    int pivotPointer;

};

#endif // LARGETABLE_H
