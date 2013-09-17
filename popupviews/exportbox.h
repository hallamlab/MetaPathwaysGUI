#ifndef EXPORTBOX_H
#define EXPORTBOX_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QCheckBox>
#include "dataviews/tabledata.h"
#include <QList>

typedef struct _export {
    QString name;
    int rank;
    unsigned int col;
    QCheckBox *checkbox;
} EXPORT_SELECT;


class ExportBox : public QWidget
{
    Q_OBJECT
public:
    explicit ExportBox(TableData *td = 0, QWidget *parent = 0);
    ~ExportBox() {}
signals:
    
public slots:
    void clickedChoice();

private:
    QGroupBox *createNonExclusiveGroup(QGridLayout *grid);
    TableData *td;

    QList<EXPORT_SELECT> Columns;


};

#endif // EXPORTBOX_H
