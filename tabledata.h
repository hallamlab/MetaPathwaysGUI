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
    explicit TableData(const QString &file, QWidget *parent = 0);
    void setupFromFile(const QString &file);
    ~TableData();

    QTableWidget* tableData;

private:
    Ui::TableData *ui;
};

#endif // TABLEDATA_H
