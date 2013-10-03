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


typedef struct _HTABLE_IDENTITY {
    QString sampleName;
    ATTRTYPE attrType;
  //  _HTABLE_IDENTITY(ATTRTYPE type, QString name): attrType(type), sampleName(name) {}
}  HTABLEIDENTITY;

namespace Ui {
class HTableData;
}

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
    void showTableData(bool hideZeroRows = false);
    unsigned int  showSelectedTableData(QString categoryName, bool hideZeroRows = false);
    void setTableIdentity(QString sampleName, ATTRTYPE attrType);
    unsigned int fillSelectedData(QString category, unsigned int maxDepth, int state, bool hideZeroRows);

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
  //  void searchButtonPressed();
  //  void exportButtonPressed();
    void spinBoxChanged(int depth);
    void showHierarchyChanged(int state);
    void hideZeroRowsChanged(int state);
    void showInformativeTable(QTableWidgetItem *item);
    void switchCategory(int);

private:
    QPushButton* searchButton;
    QList<QBrush> colors;
    QList<QString> indents;
    QList<QFont> fonts;
    QCheckBox *showHierarchy;
    QSpinBox *showDepth;
    QCheckBox *viewToggleBox;
    QComboBox *categorySelector;
    QCheckBox *hideZeroRows;


    QStringList headers;
    unsigned int maxSpinBoxDepth;

    Ui::HTableData *ui;
};

#endif // HTABLEDATA_H
