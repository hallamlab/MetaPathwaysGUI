#ifndef TABLEDATA_H
#define TABLEDATA_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include "metawidget.h"
#include "dataviews/largetable.h"
#include "popupviews/searchwidget.h"
#include "popupviews/exportwidget.h"
#include "popupviews/exportbox.h"
#include "popupviews/displayinfo.h"
#include "genebrowser/genomeview.h"
#include "caching/tablemanager.h"

#include <QStringList>
namespace Ui {
class TableData;
}



class SearchWidget;
class ExportWidget;
class ExportBox;

class TableData : public QWidget, public MetaWidget
{
    Q_OBJECT

public:
    explicit TableData(QWidget *parent = 0 );
    //explicit TableData(bool LARGE, bool HAS_COMMENT, const QString &file, QList<enum TYPE> types = QList<enum TYPE>(),   QWidget *parent = 0 );
    virtual void setupFromFile(const QString &file);
    void parseFile(const QString &file, QList<QStringList> &data, const QChar &DELIM, bool HAS_COMMENT);
    void populateTable(QTableWidget &table,  QList<ROW *>  &bigdata, const QStringList &headers, int from = 0);
    void setNumCols(unsigned int numCols);
    QStringList getHeaders();
    QString getHeader(unsigned int i);

    bool setParameters(bool HAS_COMMENT, const QString &file, QList<enum TYPE> _types,bool CACHE=false);

    void setPopupListener(DisplayInfo *p);
    void setPopupListener(GenomeView *g);
    void initializeSearchFilter(QString query, int column=0, bool caseSensitive = true) ;
    enum TYPE getFieldType(unsigned int i);

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

    SearchWidget* searchWidget;
    QPushButton* searchButton;

    ExportWidget* exportWidget;
    QPushButton* exportButton;
    DisplayInfo *p;
    GenomeView *g;


    ExportBox* exportBox;

    SEARCH searchFilter;

    Ui::TableData *ui;

    bool CACHE;
};

#endif // TABLEDATA_H
