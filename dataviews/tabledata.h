#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QProgressBar>
#include "dataviews/largetable.h"
#include "popupviews/searchwidget.h"
#include "popupviews/exportbox.h"
#include "popupviews/displayinfo.h"
#include "genebrowser/genomeview.h"
#include "caching/tablemanager.h"
#include "helper/utilities.h"
#include "datamodel/samplercmanager.h"

#include <QStringList>
namespace Ui {
   class TableData;
}


class SearchWidget;
class ExportBox;

class TableData : public QWidget
{
    Q_OBJECT

public:
    explicit TableData(QWidget *parent = 0 );
    //explicit TableData(bool LARGE, bool HAS_COMMENT, const QString &file, QList<enum TYPE> types = QList<enum TYPE>(),   QWidget *parent = 0 );
    virtual void setupFromFile(const QString &file);
    void setupFromFile(const QString &file, QList<unsigned int> & columns, QRegExp filter);

    void parseFile(const QString &file, QList<QStringList> &data, const QChar &DELIM, bool HAS_COMMENT);
    void populateTable(QTableWidget &table,  QList<ROW *>  &bigdata, const QStringList &headers, int from = 0);
    void setNumCols(unsigned int numCols);
    QStringList getHeaders();
    QString getHeader(unsigned int i);

    bool setParameters(bool HAS_COMMENT, const QString &file, QList<enum TYPE> _types,bool CACHE=false);

    bool setParameters(bool HAS_COMMENT, const QString &file, QList<TYPE> &_types, QList<unsigned int> &_columns, bool CACHE, QRegExp filter);

    void setPopupListener(DisplayInfo *p);
    void setPopupListener(GenomeView *g);
    void initializeSearchFilter(QString query, int column=0, bool caseSensitive = true) ;
    enum TYPE getFieldType(unsigned int i);

    bool saveTableToFile(QString fileName, QChar delim, const QStringList &selectedHeaders);
    bool saveSequencesToFile(QString fileName,  RESOURCE type);
    void setSampleName(QString sampleName);
    QString getSampleName();
    void setMultiSampleMode(bool multisample);
    bool isMultiSampleMode();

    QStringList getSampleNames();
    QString getSampleName(unsigned int i);
    void setSampleNames(QStringList sampleNames);


    QList<enum TYPE> types;

    QList<ROW *> bigdata; //table data
    unsigned int numCols;

    QString* statsText;
    LargeTable* largeTable;

    bool HAS_COMMENT;

    int top;
    int bottom;

    void updateData(double top, bool reload = false);

    ~TableData();

    QTableWidget* tableWidget;


public slots:
    void outputRows(int top);
    void headerClicked(int index);
    void searchQuery(QString query, int column, bool caseSensitive);
    void createGeneViewData(QTableWidgetItem *item);

private slots:
    void searchButtonPressed();
    void exportButtonPressed();

signals:
    void transmitSequenceData(QTableWidgetItem *item, GeneBrowserData gbdata);

private:
    int dw;
    QStringList headers; //table headers
    QLabel *titleLabel;
    SearchWidget * searchWidget;
    QPushButton* searchButton;

   // ExportWidget* exportWidget;
    QPushButton* exportButton;
    DisplayInfo *p;
    GenomeView *g;
    QString sampleName;
    QString file;
    ExportBox * exportBox;

    SEARCH searchFilter;


    bool multiSampleMode;
    QStringList sampleNames;


    Ui::TableData *ui;

    bool CACHE;
};

#endif // TABLEDATA_H
