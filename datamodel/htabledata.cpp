#include "htabledata.h"
#include "ui_htabledata.h"
#include <QTableWidget>

HTableData::HTableData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HTableData)
{
    ui->setupUi(this);

    tableWidget = this->findChild<QTableWidget *>("tableWidget");
    showHierarchy  = this->findChild<QCheckBox *>("showHierarchy");
    showDepth = this->findChild<QSpinBox *>("showDepth");


    connect(showDepth, SIGNAL(valueChanged(int) ), this, SLOT(spinBoxChanged(int) ) );
    connect(showHierarchy, SIGNAL(stateChanged(int)), this, SLOT(showHierarchyChanged(int) ) );



    colors  << QBrush(Qt::black) <<  QBrush(Qt::red) << QBrush(Qt::blue) << QBrush(QColor(0,153,0)) << QBrush(QColor(255,128,0)) <<  QBrush(QColor(102,0,102)) << QBrush(QColor(76,0,183)) << QBrush(QColor(204,0,102))   << QBrush(Qt::gray);
    indents << QString("")       << QString("\t")    << QString("\t\t")  << QString("\t\t\t")       << QString("\t\t\t\t")       <<  QString("\t\t\t\t\t")     << QString("\t\t\t\t\t\t")  << QString("\t\t\t\t\t\t\t")   << QString("\t\t\t\t\t\t\t\t")   ;


}

HTableData::~HTableData()
{
    delete ui;
}

void HTableData::setMaxSpinBoxDepth(unsigned int maxDepth) {
    this->maxSpinBoxDepth  = maxDepth;
    showDepth->setMaximum(maxDepth);
}

void HTableData::setShowHierarchy(bool flag) {
    this->showHierarchy->setChecked(flag);
}

bool HTableData::setParameters(HTree *htree,  QList<TYPE> _types) {

    this->setNumCols(_types.size());
    foreach(enum TYPE type, _types) {
        types.append(type);
    }
    this->htree = htree;
}

void HTableData::clearConnectors() {
    this->connectors.clear();
}

void HTableData::addConnector(Connector *connector) {
     this->connectors.append(connector);
}

void HTableData::setNumCols(unsigned int numCols) {
    this->numCols = numCols;
}


void HTableData::setHeaders(QStringList &headers) {
  this->headers = headers;

 }

void HTableData::spinBoxChanged(int depth) {

    this->showTableData();
    // this->fillData(depth, (this->showHierarchy->checkState()==Qt::Checked) );
}


void HTableData::showTableData() {
    this->fillData(this->showDepth->value(), (this->showHierarchy->checkState()==Qt::Checked) );

}

void HTableData::showHierarchyChanged(int state) {
    this->showTableData();
    //this->fillData(this->showDepth->value(), (this->showHierarchy->checkState()==Qt::Checked) );

}

void HTableData::fillData(unsigned int maxDepth, int state) {
     QList<ROWDATA *> data;
     data =  this->htree->getRows(maxDepth, state, this->connectors);
     this->populateTable(data, this->headers, state );

}



void HTableData::populateTable( QList<ROWDATA *> &data, const QStringList &headers, int hierarchyEnabled){

    tableWidget->clearContents();
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setRowCount(data.size());
    tableWidget->setColumnCount(types.size());

    if( hierarchyEnabled)
       tableWidget->setSortingEnabled(false);
    else
       tableWidget->setSortingEnabled(true);


    QTableWidgetItem *item;
    int k = 0;


    foreach( ROWDATA * datum,  data) {
        if(hierarchyEnabled)
            item = new QTableWidgetItem(this->indents[datum->depth] + QString(datum->name));
        else
            item = new QTableWidgetItem( QString(datum->name));

        item->setForeground(this->colors[datum->depth]);
        tableWidget->setItem(k,0,item);

        tableWidget->setItem(k,1,new QTableWidgetItem(QString(datum->alias)));
        for(unsigned int j=0; j <  datum->counts.size(); j++) {
            tableWidget->setItem(k,2 +j, new QTableWidgetItem(QString::number(datum->counts[j])));
        }
      k++;
    }
    tableWidget->resizeColumnsToContents();
    //table.resizeRowsToContents();
    tableWidget->horizontalHeader()->setStretchLastSection(true);
}


