#ifndef HTABLEDATA_H
#define HTABLEDATA_H
#include <QList>
#include <QDialog>
#include <QTableWidget>
#include <QDebug>
#include <QCheckBox>
#include <QSpinBox>
#include <QFont>
#include <QComboBox>
#include <QLabel>
#include <QPalette>
#include <QTableWidgetItem>
#include <QThread>

#include "datamodel/connector.h"
#include "datamodel/htree.h"
#include "datamodel/datamanager.h"
#include "datamodel/lcastar.h"
#include "displaywidgets/toolbarmanager.h"
#include "displaywidgets/toolbarwidgets.h"
#include "displaywidgets/mdiareawidget.h"
#include "displaywidgets/widgetstacker.h"
#include "popupviews/exportbox.h"
#include "popupviews/searchwidget.h"
#include "displaywidgets/progressview.h"
#include "helper/types.h"
#include "helper/globaldatatransit.h"



typedef struct _HTABLE_IDENTITY {
    QString sampleName;
    ATTRTYPE attrType;
  //  _HTABLE_IDENTITY(ATTRTYPE type, QString name): attrType(type), sampleName(name) {}
}  HTABLEIDENTITY;



namespace Ui {
class HTableData;
}
class ExportBox;
class SearchWidget;
class ResultWindow;


class HTableData : public QDialog
{
    Q_OBJECT
    
public:
    explicit HTableData(QWidget *parent = 0, int spinBoxValuem = 1, bool _showHierachy = false, bool _hideZeroRows = false, VALUETYPE _valueType = ORFCOUNT);
    ~HTableData();
    bool setParameters(HTree *htree, QList<enum TYPE> _types);
    void addConnector(Connector *connector, ATTRTYPE attr);
    void clearConnectors();
    void setNumCols(unsigned int numCols);
    void fillData();

    void populateTable(QList<ROWDATA *> &data, const QStringList &headers);
    void computeLCAStar(QList<ROWDATA *> &data);
    QString LCAStarValue(const QString &category, unsigned int sampleNum);

    void getTaxonList(ROWDATA *datum, unsigned int sampleNum, QStringList &taxonList);

    unsigned int getTaxDistribution(const QString &category, unsigned int sampleNum,  QList<FREQUENCY> &freq, unsigned int &totalorfs);

    void setMaxSpinBoxDepth(unsigned int maxDepth);
    void setShowHierarchy(bool flag);
   // void setHeaders(QStringList &headers);
    QStringList getHeaders();
    QString getHeader(unsigned int i);
    enum TYPE getFieldType(unsigned int i);

    void showTableData();
    unsigned int  showSelectedTableData();
    void setTableIdentity(QString sampleName, ATTRTYPE attrType);
    unsigned int fillSelectedData();
    QStringList multiSampleHeaders() ;

    bool saveTableToFile(QString fileName, QChar delim, const QStringList &selectedHeaders);
    bool saveSequencesToFile(QString sampleName, QString fileName,  RESOURCE type);

    void setMultiSampleMode(bool multisample);
    bool isMultiSampleMode();
    QStringList getSampleNames();
    void addSampleName(QString sampleName, bool clearPrev = false) ;
    void setSampleNames(QStringList sampleNames);
    unsigned int getSampleNumber(QString sampleName);
    QString getSampleName(unsigned int i);

    void initializeSearchFilter(QString query, int column);

    static void spawnInformativeTable(const QString &sampleName, const QList<ORF *> &orfList);

    QTableWidget* tableWidget;
    unsigned int numCols;
    QList<enum TYPE> types;
    QStringList windowtitles;
    Connector *_connector;
    QList<Connector *> connectors;
    QHash<ATTRTYPE, QList<Connector *> > allConnectors;


    HTree *htree;
    bool subWindow;
    QString category;
    unsigned int level;
    QLabel *subCategoryName;
    QLabel *depthLabelValue;
    QList<HTABLEIDENTITY> htableIdentities;


    HTABLEIDENTITY id;

    HTABLE_PARAMS tableParams;
private:
     bool isNonZero(ROWDATA *r);

private slots:

    void spinBoxChanged(int depth);
    void showHierarchyChanged(int state);
    void hideZeroRowsChanged(int state);
    void showHierarchyOrZeroRowToggleChanged();
    void showInformativeTable(QTableWidgetItem *item);
    void switchCategory(int);
    void searchButtonPressed();
    void exportButtonPressed();
    void ProvideContexMenu(QPoint pos);
    void toggleAlpha(int vType);
    void matchAlphaVisible();


signals:
    void showTable(QString sampleName, ATTRTYPE type);


public slots:
    void searchQuery(QString query1, int column1, QString query2, int column2, QString query3, int column3, QString query4, int column4, OPTYPE type,  bool caseSensitive);

private:
    void addSearchFilter(QString query, int column);
    void clearSearchFilters();
    void makeSearch(OPTYPE optype,  bool caseSensitive);

    VALUETYPE getValueType();

private:

    SearchWidget * searchWidget;
    QPushButton* searchButton;

    ExportBox * exportBox;
    QPushButton* exportButton;

    QList<QBrush> colors;
    QList<QString> indents;
    QList<QFont> fonts;
    QCheckBox *showHierarchy;
    QSpinBox *showDepth;
    QLabel *numOrfsLabel;
    QComboBox *categorySelector;

    QCheckBox *hideZeroRows;
    QCheckBox *showRPKM;

    QComboBox *valueType;
    QComboBox *alpha;
    QSpinBox *lcaDepth;


    bool multiSampleMode;
 //   QStringList headers;
    QStringList sampleNames;
    unsigned int maxSpinBoxDepth;

    QList<SEARCH> searchFilters;

    Ui::HTableData *ui;
};

#endif // HTABLEDATA_H
