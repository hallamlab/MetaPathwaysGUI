#include "tabledata.h"
#include "ui_tabledata.h"
#include "utilities.h"
#include <QScrollBar>
#include <QtCore>
#include <QFile>
#include <QRegExp>
#include "QDebug"

TableData::TableData(  QWidget *parent) :
    QWidget(parent), MetaWidget(),
    ui(new Ui::TableData)
{
    ui->setupUi(this);

    dw = 500;
    statsText = new QString();


    titleLabel = this->findChild<QLabel *>("titleLabel");
    tableWidget = this->findChild<QTableWidget *>("tableWidget");
    searchButton = this->findChild<QPushButton *>("searchButton");
    exportButton = this->findChild<QPushButton *>("exportButton");


    connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButtonPressed()));
    connect(exportButton, SIGNAL(clicked()), this, SLOT(exportButtonPressed()));

    connect(tableWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(outputRows(int)));
    connect(tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerClicked(int)));


    this->searchWidget = new SearchWidget(this);
    this->searchWidget->hide();

    this->exportBox = new ExportBox(this);
    this->exportBox->hide();
    this->p =0;
}


bool TableData::setParameters(bool HAS_COMMENT, const QString &file, QList<TYPE> _types, bool CACHE) {

    top = 0;
    bottom = 0;

    this->CACHE = CACHE;

    this->setNumCols(_types.size());
    foreach(enum TYPE type, _types) {
        types.append(type);
    }

    this->file = file;
    this->HAS_COMMENT = HAS_COMMENT;

    this->setupFromFile(file);

}

void TableData::setPopupListener(DisplayInfo *p) {
    if( this->p==0 ) {
      connect(tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), p, SLOT(itemDoubleClicked(QTableWidgetItem *)));
      this->p= p;
    }
}

QStringList TableData::getHeaders(){
    return this->headers;
}


QString TableData::getHeader(unsigned int i){
    if(i >= this->headers.size()) return QString();

    return this->headers.at(i);
}

enum TYPE TableData::getFieldType(unsigned int i) {
    if( i >= this->types.size() )
        return UNKNOWN;
    return this->types.at(i);
}

void TableData::searchQuery(QString query, int column, bool caseSensitive){
    this->initializeSearchFilter(query, column, caseSensitive);
    this->largeTable->markRowsSearch(this->searchFilter);
    tableWidget->setRowCount(largeTable->tableData.length());
    int currPos = tableWidget->verticalScrollBar()->value();
    this->updateData(currPos, true);
}


void TableData::initializeSearchFilter(QString query, int column, bool caseSensitive) {

    QString _and = "&&", _or="||";

    QStringList searchFields;
    this->searchFilter.searchItems.clear();
    this->searchFilter.searchCols.clear();


    searchFields = query.split(_or, QString::SkipEmptyParts);
    this->searchFilter.type = OR;

    if( searchFields.size() ==0 ) {
        searchFields = query.split(_and, QString::SkipEmptyParts);
        this->searchFilter.type = AND;
    }

    this->searchFilter.searchItems = searchFields;


    if( column==0) {
        for(unsigned int i = 0; i < this->numCols; i++) {
            if( this->types[i]==STRING)
               this->searchFilter.searchCols.append(i);
        }
    }
    else {
        this->searchFilter.searchCols.append(column-1);
    }

    this->searchFilter.caseSensitive = caseSensitive;

}

void TableData::searchButtonPressed(){
    this->searchWidget->show();
}

void TableData::exportButtonPressed(){
    this->exportBox->show();
}

void TableData::setNumCols(unsigned int numCols) {
    this->numCols = numCols;
}

void TableData::headerClicked(int index){
    this->largeTable->sortByField(index);
    tableWidget->clearContents();

    int currPos = tableWidget->verticalScrollBar()->value();
    this->updateData(currPos, true);
}

void TableData::outputRows(int top){
    this->top = top;
    this->updateData(top);
}

void TableData::updateData(double top, bool reload){
    int pivot =  top;
    if (largeTable->getPivot() -0.9*dw >=  pivot || pivot >= largeTable->getPivot()  + 0.9 *dw || reload){
         largeTable->setPivot(pivot);
         int from = pivot > dw ? pivot -dw : 0;
         largeTable->getData(bigdata, from, dw);
     //    qDebug() << " Update from " << from << " size " << bigdata.size();
         this->populateTable(*tableWidget, bigdata, headers, from );
    }
}


void TableData::setupFromFile(const QString &file){
       QChar tableDELIM = '#';

        TableManager *tableManager = TableManager::getTableManager();

        largeTable = tableManager->getTable(file);

        if( largeTable ==0 ) {
           largeTable = new LargeTable(file, '\t', true, true, types);
           if (CACHE) tableManager->setTable(file, largeTable);
        }

        largeTable->getData(bigdata, 0,dw);
        tableWidget->setRowCount(largeTable->tableData.length());
        tableWidget->setColumnCount(largeTable->colNames.length());
        headers = largeTable->colNames;

        titleLabel->setText(file);



        if(!bigdata.empty() ) {
           this->populateTable(*tableWidget, bigdata, headers, top);
        }
    //    qDebug() << "Done";

}

/*
 * Creates the actual table widget the data given to it, and populates the table with data and headers given with
 * the pointer to the table given.
 */

//BIG DATA
void TableData::populateTable(QTableWidget &table,  QList<ROW *> &data, const QStringList &headers, int from){

    table.setColumnCount(headers.size());
    table.setHorizontalHeaderLabels(headers);

    int k = from;
    table.clearContents();
    foreach( ROW *datum,  data) {
      for( unsigned int i = 0; i < largeTable->numCols; i++) {
          try{
              //table.setItem(k,i,new QTableWidgetItem("aldkfjglkjdsfgjdfsjglkjdsfgjldfkgjjdfglkdfgjldfjgljdsflkjlsdkfjglkjsdflgjlkdsfjgljdfslgjldfgljdsflkgjkdfsjlgkja;sdkfj;alwkj;l435u3209"));
              if( types[i] == INT) {
                 table.setItem(k,i, new QTableWidgetItem(QString::number(datum->intVar.at(largeTable->index[i]))));
              }
              if( types[i] == DOUBLE) table.setItem(k,i,new QTableWidgetItem(QString::number(datum->doubleVar.at(largeTable->index[i]))));

              if(types[i] == STRING) {
                  table.setItem(k,i,new QTableWidgetItem(QString(datum->strVar.at(largeTable->index[i]))));
              }

          }
          catch(...) {
              qDebug() << " Failed at " << i << datum << " double " << datum->doubleVar << " int " << datum->intVar << " str " << datum->strVar;

          }
      }
      k++;
    }
    table.resizeColumnsToContents();
    //table.resizeRowsToContents();
    table.horizontalHeader()->setStretchLastSection(true);
}




/*
 * Parses a result file. If HAS_COMMENT is true, then it expects that there are lines with DELIM in certain lines
 * of the text file.
 */
void  TableData::parseFile(const QString &file, QList<QStringList> &data, const QChar &DELIM, bool HAS_COMMENT){
    QFile inputFile(file);
    bool firstLine = true;

    if (inputFile.open(QIODevice::ReadOnly)){
        QTextStream in(&inputFile);
        while ( !in.atEnd() ){
            QString line = in.readLine().trimmed();
            QStringList lineSplit = line.split(QRegExp("\\t"));
            lineSplit.removeAll("");

            if (!lineSplit.isEmpty()){
                if (HAS_COMMENT){
                    //if there are comment lines in the file
                    if (line.at(0)==DELIM){
                        line.remove(0,1);
                        statsText->append(line.trimmed() + '\n');
                        continue; //skip to the next line in the file
                    }
                }
                //take lines as they come
                if (firstLine){
                    QStringList headerList = lineSplit;
                    headers = headerList;
                    firstLine = false;
                }else {
                    data.append(lineSplit);
                }
            }
        }
        inputFile.close();
    }

}

TableData::~TableData()
{
    delete this->largeTable;
    delete this->tableWidget;
    delete ui;
}