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


namespace Ui {
class HTableData;
}

class HTableData : public QDialog
{
    Q_OBJECT
    
public:
    explicit HTableData(QWidget *parent = 0);
    ~HTableData();
    bool setParameters(HTree *htree, Connector  *connector, QList<enum TYPE> _types);
    void setNumCols(unsigned int numCols);
    void fillData(unsigned int maxDepth, int state);

    void populateTable( QList<ROWDATA *> &data, const QStringList &headers, int state);

    void setMaxSpinBoxDepth(unsigned int maxDepth);
    void setShowHierarchy(bool flag);
    void showTableData();
    QTableWidget* tableWidget;
    unsigned int numCols;
    QList<enum TYPE> types;
    Connector *connector;
    HTree *htree;


private slots:
  //  void searchButtonPressed();
  //  void exportButtonPressed();
    void spinBoxChanged(int depth);
    void showHierarchyChanged(int state);

private:
    QPushButton* searchButton;
    QList<QBrush> colors;
    QList<QString> indents;
    QList<QFont> fonts;
    QCheckBox *showHierarchy;
    QSpinBox *showDepth;
    unsigned int maxSpinBoxDepth;

    Ui::HTableData *ui;
};

#endif // HTABLEDATA_H
