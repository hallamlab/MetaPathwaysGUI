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
    this->multiSampleMode =false;
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

    tableWidget->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);


    this->exportBox = new ExportBox(this);
    this->exportBox->hide();
    this->p =0;
    this->g =0;
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

// with select columns
bool TableData::setParameters(bool HAS_COMMENT, const QString &file, QList<TYPE> &_types, QList<unsigned int> &_columns, bool CACHE, QRegExp filter) {

    top = 0;
    bottom = 0;

    this->CACHE = CACHE;

    this->setNumCols(_types.size());
    foreach(enum TYPE type, _types) {
        types.append(type);
    }

    this->file = file;
    this->HAS_COMMENT = HAS_COMMENT;

    this->setupFromFile(file, _columns, filter);

}


void TableData::createGeneViewData(QTableWidgetItem *item) {

    unsigned int col = item->column();

    QString key  = item->text();

    QList< ROW *> rows;
    this->largeTable->getSelectRows(largeTable->wholeTableData, rows, key, col, STRING);


    GeneBrowserData gbd;

    SequenceData s;
    ORFData orf;
    s.length = 0;
    s.name = key;

    foreach(ROW *row, rows) {

        orf.start = row->intVar[largeTable->index[2]];
        orf.end = row->intVar[largeTable->index[3]];
        if(s.length < row->intVar[largeTable->index[5]] ) {
            s.length = row->intVar[largeTable->index[5]];
        }


        if( row->strVar[largeTable->index[6]] == QString("+"))
           orf.strand = FORWARD;
        else
           orf.strand = REVERSE;

        orf.tax_annot = row->strVar[largeTable->index[8]];
        orf.func_annot = row->strVar[largeTable->index[9]];
        orf.note = "";

        gbd.orfData.append(orf);
    }

    s.start =0;
    s.end = s.length-1;

    gbd.seq =s;
    emit transmitSequenceData(item, gbd);

}

void TableData::setPopupListener(DisplayInfo *p) {
    if( this->p==0 ) {
     // connect(tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), p, SLOT(itemDoubleClicked(QTableWidgetItem *)));

      connect(tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT( createGeneViewData(QTableWidgetItem *) ));

      connect(this, SIGNAL( transmitSequenceData( QTableWidgetItem *, GeneBrowserData )), p, SLOT( receiveSequenceData(QTableWidgetItem *, GeneBrowserData ) ) );

      this->p= p;
    }
}

void TableData::setPopupListener(GenomeView *g) {
    if( this->g==0 ) {
      connect(tableWidget, SIGNAL(itemClicked(QTableWidgetItem *)), g, SLOT(itemGenomeDisplay(QTableWidgetItem *)));
      this->g= g;
    }
}

QStringList TableData::getHeaders(){
    return this->headers;
}


QString TableData::getHeader(unsigned int i){
    if(i >= this->headers.size()) return QString();

    return this->headers.at(i);
}
void TableData::setMultiSampleMode(bool multisample) {
    this->multiSampleMode = multisample;
}

bool TableData::isMultiSampleMode() {
    return this->multiSampleMode;
}

QStringList TableData::getSampleNames() {
    return this->sampleNames;
}

QString TableData::getSampleName(unsigned int i) {
   if( this->sampleNames.size() > i )  {
       return this->sampleNames[i];
   }
   else
       return QString("");
}


void TableData::setSampleNames(QStringList sampleNames) {
    this->sampleNames = sampleNames;
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
    qDebug() << " search button";
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
         this->populateTable(*tableWidget, bigdata, headers, from );
    }
}


void TableData::setupFromFile(const QString &file){

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

        if(!bigdata.empty() ) {
           this->populateTable(*tableWidget, bigdata, headers, top);
        }

        this->exportBox = new ExportBox(this);
}


//select columns variant
void TableData::setupFromFile(const QString &file, QList<unsigned int> & columns, QRegExp filter){

        TableManager *tableManager = TableManager::getTableManager();

        largeTable = tableManager->getTable(file);

        if( largeTable ==0 ) {
           largeTable = new LargeTable(file, '\t', true, true, types, columns, filter);
           if (CACHE) tableManager->setTable(file, largeTable);
        }

        largeTable->getData(bigdata, 0,dw);
        tableWidget->setRowCount(largeTable->tableData.length());
        tableWidget->setColumnCount(largeTable->colNames.length());
        headers = largeTable->colNames;

        if(!bigdata.empty() ) {
           this->populateTable(*tableWidget, bigdata, headers, top);
        }

        this->exportBox = new ExportBox(this);
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


bool TableData::saveTableToFile(QString fileName, QChar delim) {

    QFile outFile(fileName);

    if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outFile);
        QLabel *waitScreen = Utilities::ShowWaitScreen("Please wait while the file is being written!");
        QProgressBar progressBar;
        progressBar.setRange(0, this->largeTable->tableData.size());
        progressBar.show();

        unsigned int interval = this->largeTable->tableData.size()/100;


        for(int i =0; i < this->largeTable->tableData.size();  i++) {
            for(int j =0; j < this->tableWidget->columnCount(); j++)  {
                 if( i%interval ==0) { progressBar.setValue(i); qApp->processEvents();  progressBar.update(); }
                 if(j!=0)  out << delim;
                 switch( this->types[j] ) {
                     case STRING:
                          out << this->largeTable->tableData[i]->strVar[ this->largeTable->index[j]];
                          break;
                     case INT:
                          out << QString::number(this->largeTable->tableData[i]->intVar[ this->largeTable->index[j]]);
                          break;
                     case DOUBLE:
                          break;
                     default:
                          break;
                 }

             }
             out << "\n";
        }
        waitScreen->hide();
        progressBar.hide();

        outFile.close();
    }
    return true;

}


bool TableData::saveSequencesToFile( QString fileName,  RESOURCE type) {
    QFile outFile(fileName);

    if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outFile);

        QProgressBar progressBar;

        progressBar.setRange(0, this->largeTable->tableData.size());
        progressBar.show();

        unsigned int interval = this->largeTable->tableData.size()/100;

        SampleResourceManager *sampleResourceManager = SampleResourceManager::getSampleResourceManager();
        FileIndex *fileIndex = sampleResourceManager->getFileIndex(this->getSampleName(), type);

        if( fileIndex !=0 ) {
            unsigned int col;
            if( type == NUCFNA || type == AMINOFAA ) col = 0;
            if( type == NUCFASTA) col = 4;

            QString name, resultStr;

            for(int i =0; i < this->largeTable->tableData.size();  i++) {
               if( i%interval ==0) { progressBar.setValue(i); qApp->processEvents();  progressBar.update(); }
               name = this->largeTable->tableData[i]->strVar[ this->largeTable->index[col]];
               resultStr = fileIndex->getDataToDisplay(name);
               if( !resultStr.isEmpty()) { out << resultStr; }
            }
        }

        progressBar.hide();
        outFile.close();
    }
    return true;
}

void TableData::setSampleName(QString sampleName){
    this->sampleName = sampleName;
}

QString TableData::getSampleName() {
    return this->sampleName;
}

TableData::~TableData()
{
    delete this->largeTable;
    delete this->tableWidget;
    delete ui;
}
