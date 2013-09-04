#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include "metawidget.h"
#include "largetable.h"

namespace Ui {
class TableData;
}

class TableData : public QWidget, public MetaWidget
{
    Q_OBJECT

public:
    explicit TableData(bool LARGE, bool HAS_COMMENT, const QString &file, QList<enum TYPE> types = QList<enum TYPE>(), QWidget *parent = 0 );
    virtual void setupFromFile(const QString &file);
    QList<QStringList *>* parseFile(const QString &file, const QChar &DELIM, bool HAS_COMMENT);
    void populateTable(QTableWidget &table, const QList<QStringList *> &data, const QStringList &headers, int from = 0);
    void populateTable(QTableWidget &table,  QList<ROW *>  &bigdata, const QStringList &headers, int from = 0);
    void setNumCols(unsigned int numCols);
    QList<enum TYPE> types;

    QStringList* headers; //table headers
    QList<QStringList *>* data; //table data
    QList<ROW *> bigdata; //table data
    unsigned int numCols;

    QString* statsText;
    LargeTable* largeTable;

    bool HAS_COMMENT;
    bool LARGE;

    int top;
    int bottom;

    void updateData(double top, double bottom);

    ~TableData();

    QTableWidget* tableWidget;


public slots:
    void outputRows(int top);
    void headerClicked(int index);

private:
    int dw;

    Ui::TableData *ui;
};

#endif // TABLEDATA_H
