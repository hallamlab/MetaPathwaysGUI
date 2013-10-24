#ifndef HTABLEDATA_H
#define HTABLEDATA_H
#include <QList>
#include <QDialog>
#include "types.h"
#include <QTableWidget>
#include "datamodel/connector.h"
#include "datamodel/htree.h"
#include <QDebug>
#include <QCheckBox>
#include <QSpinBox>
#include <QFont>
#include <QComboBox>
#include <QLabel>
#include <QPalette>
#include <QTableWidgetItem>
#include "datamodel/datamanager.h"
#include "displaywidgets/toolbarmanager.h"
#include "displaywidgets/toolbarwidgets.h"
#include "displaywidgets/mdiareawidget.h"
#include "displaywidgets/widgetstacker.h"
#include "popupviews/exportbox.h"
#include "popupviews/searchwidget.h"


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

class HTableData : public QDialog
{
    Q_OBJECT
    
public:
    explicit HTableData(QWidget *parent = 0);
    ~HTableData();
    bool setParameters(HTree *htree, QList<enum TYPE> _types);
    void addConnector(Connector *connector);
    void clearConnectors();
    void setNumCols(unsigned int numCols);
    void fillData(unsigned int maxDepth, int state, bool hideZeroRows);

    void populateTable( QList<ROWDATA *> &data, const QStringList &headers, int state);

    void setMaxSpinBoxDepth(unsigned int maxDepth);
    void setShowHierarchy(bool flag);
    void setHeaders(QStringList &headers);
    QStringList getHeaders();
    QString getHeader(unsigned int i);
    enum TYPE getFieldType(unsigned int i);

    void showTableData(bool hideZeroRows = false);
    unsigned int  showSelectedTableData(QString categoryName, bool hideZeroRows = false);
    void setTableIdentity(QString sampleName, ATTRTYPE attrType);
    unsigned int fillSelectedData(QString category, unsigned int maxDepth, int state, bool hideZeroRows);

    bool saveTableToFile(QString fileName, QChar delim);
    bool saveSequencesToFile(QString sampleName, QString fileName,  RESOURCE type);

    void setMultiSampleMode(bool multisample);
    bool isMultiSampleMode();
    QStringList getSampleNames();
    void addSampleName(QString sampleName) ;
    void setSampleNames(QStringList sampleNames);
    QString getSampleName(unsigned int i);

    QTableWidget* tableWidget;
    unsigned int numCols;
    QList<enum TYPE> types;
    Connector *connector;
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
private:
     bool isNonZero(ROWDATA *r);

private slots:

    void spinBoxChanged(int depth);
    void showHierarchyChanged(int state);
    void hideZeroRowsChanged(int state);
    void showInformativeTable(QTableWidgetItem *item);
    void switchCategory(int);
    void searchButtonPressed();
    void exportButtonPressed();


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
    QCheckBox *viewToggleBox;
    QComboBox *categorySelector;
    QCheckBox *hideZeroRows;

    bool multiSampleMode;
    QStringList headers;
    QStringList sampleNames;
    unsigned int maxSpinBoxDepth;

    Ui::HTableData *ui;
};

#endif // HTABLEDATA_H
