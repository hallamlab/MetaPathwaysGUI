#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>

namespace Ui {
class TableData;
}

class TableData : public QWidget
{
    Q_OBJECT
    
public:
    explicit TableData(const QString &file, QWidget *parent = 0);
    ~TableData();

    void setupFromFile();
    QLabel* titleLabel;
    QLabel* statsLabel;
    QTableWidget* tableData;
    QString file;

private:
    Ui::TableData *ui;
};

#endif // TABLEDATA_H
