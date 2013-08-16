#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include "metawidget.h"

namespace Ui {
class TableData;
}


class TableData : public QWidget, public MetaWidget
{
    Q_OBJECT

public:
    explicit TableData(bool HAS_COMMENT, const QString &file, QWidget *parent = 0);
    virtual void setupFromFile(const QString &file);
    QList<QStringList>* parseFile(const QString &file, const QChar &DELIM, bool HAS_COMMENT);
    QTableWidget* populateTable(QTableWidget &table, const QList<QStringList> &data, const QStringList &headers);

    QStringList* headers; //table headers
    QList<QStringList>* data; //table data
    QString* statsText;
    bool HAS_COMMENT;

    ~TableData();

    QTableWidget* tableData;

private:
    Ui::TableData *ui;
};

#endif // TABLEDATA_H
