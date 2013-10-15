#ifndef EXPORTBOX_H
#define EXPORTBOX_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QCheckBox>
#include "dataviews/tabledata.h"
#include <QList>
#include "datamodel/htabledata.h"
#include "popupviews/exportsource.h"
#include <QRadioButton>
#include <QFileDialog>

typedef struct _export {
    QString name;
    int rank;
    unsigned int col;
    QCheckBox *checkbox;
} EXPORT_SELECT;


class TableData;
class ExportSource;

class ExportBox : public QWidget
{
    Q_OBJECT
public:
     void setTableData(TableData *td);
     ExportBox(TableData *td = 0, QWidget *parent = 0);
     ExportBox(HTableData *td = 0, QWidget *parent =0);
    ~ExportBox() ;
signals:
    
public slots:
    void clickedChoice();
    bool saveAs();

private:
    void createWidget();
private:
    QGroupBox *createNonExclusiveGroup(QGridLayout *grid);
    ExportSource *td;

    QGroupBox *exportFormat;
    QRadioButton *tsvRadio, *csvRadio;
    QHBoxLayout *vbox;
    QPushButton *cancelButton, *exportButton;

    QList<EXPORT_SELECT> Columns;

};

#endif // EXPORTBOX_H
