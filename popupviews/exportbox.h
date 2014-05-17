#ifndef EXPORTBOX_H
#define EXPORTBOX_H

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QCheckBox>
#include <QString>
#include "dataviews/tabledata.h"
#include <QList>
#include "datamodel/htabledata.h"
#include "popupviews/exportsource.h"
#include <QRadioButton>
#include <QFileDialog>
#include "utilities.h"
#include "types.h"

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
     ExportBox(TableData *td = 0, QWidget *parent = 0);
     ExportBox(HTableData *td = 0, QWidget *parent =0);

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

};

#endif // EXPORTBOX_H
