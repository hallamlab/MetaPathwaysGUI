#ifndef EXPORTBOX_H
#define EXPORTBOX_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QCheckBox>
#include <QString>
#include <QList>
#include <QRadioButton>
#include <QFileDialog>

#include "dataviews/tabledata.h"
#include "datamodel/htabledata.h"
#include "popupviews/exportsource.h"
#include "helper/utilities.h"
#include "helper/types.h"

extern const QString SELECT_SAMPLE_TAG;


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
     ExportBox(TableData *td = 0, QWidget *parent = 0, TABLEEXPORTTYPE type = OTHERSTABLEEXP);
     ExportBox(HTableData *td = 0, QWidget *parent =0);

     void setExportType(TABLEEXPORTTYPE type);
     void setAuxName(QString auxname);

    ~ExportBox() ;

signals:
    
public slots:
    void clickedChoice();
    bool saveAs();
    bool cancelWindow();

private:
    void createWidget();
    QStringList getCheckedHeaders();
    QStringList getAllHeaders();
    QStringList getCheckedSamples();
private:
    QGroupBox *createNonExclusiveGroup(QVBoxLayout *grid);
    ExportSource *td;

    QGroupBox *exportFormat;
    QCheckBox *tsvRadio, *csvRadio;
    QHBoxLayout *vbox;
    QPushButton *cancelButton, *exportButton;

    QCheckBox *fasta ;
    QCheckBox *fna ;
    QCheckBox *faa ;

    QList<EXPORT_SELECT> Columns;
    QList<QCheckBox *> samplesCheckboxes;
    TABLEEXPORTTYPE exportType;
    QString auxName;

};

#endif // EXPORTBOX_H
