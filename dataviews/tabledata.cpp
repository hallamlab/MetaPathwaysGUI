
#include <QScrollBar>
#include <QtCore>
#include <QFile>
#include <QRegExp>
#include "QDebug"


#include "dataviews/tabledata.h"
#include "ui_tabledata.h"
#include "helper/utilities.h"



TableData::TableData(  QWidget *parent) :
    QWidget(parent),
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

  //  this->searchWidget = new SearchWidget(this);
   // this->searchWidget->hide();

#ifdef SECTION
    tableWidget->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
#else
    tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    //     tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif


    //this->exportBox = new ExportBox(this);
    //this->exportBox->hide();
    this->p =0;
    this->g =0;

    this->exportType = OTHERSTABLEEXP;
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

}

/**
 * @brief TableData::loadData loads the data into the table, but if it finds a cached table
 * then it always uses it
 */

void TableData::loadData() {
    this->setupFromFile(this->file);
}

// with select columns
/**
 * @brief TableData::setParameters
 * @param HAS_COMMENT
 * @param file
 * @param _types
 * @param _columns
 * @param CACHE
 * @param filter
 * @return
 */

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
        orf.name = row->strVar[largeTable->index[0]];
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
    QStringList sampleNames;

    sampleNames << this->sampleName;
    return sampleNames;

    //return this->sampleNames;
}

QString TableData::getSampleName(unsigned int i) {
   if( this->sampleNames.size() > i )  {
       return this->sampleNames[i];
   }
   else
       return QString("");
}


void TableData::setType(TABLETYPE type) {
    this->type = type;
}


void TableData::setExportType(TABLEEXPORTTYPE type) {
    this->exportType = type;
}


void TableData::setAuxName(QString auxname) {
    this->auxName = auxname;
}


void TableData::setSampleNames(QStringList sampleNames) {
    this->sampleNames = sampleNames;
}

enum TYPE TableData::getFieldType(unsigned int i) {
    if( i >= this->types.size() )
        return UNKNOWN;
    return this->types.at(i);
}


/**
 * @brief TableData::searchQuery, signal slots and column choices for the 4 querys in the query box
 * @param query1
 * @param column1
 * @param query2
 * @param column2
 * @param query3
 * @param column3
 * @param query4
 * @param column4
 * @param type, AND or OR type
 * @param caseSensitive, case sensitive or case insensitive
 */
void TableData::searchQuery(QString query1, int column1, QString query2, int column2, QString query3, int column3, QString query4, int column4, OPTYPE type,  bool caseSensitive){

    this->clearSearchFilters();
    if(query1.size() > 0) this->addSearchFilter(query1, column1);
    if(query2.size() > 0) this->addSearchFilter(query2, column2);
    if(query3.size() > 0) this->addSearchFilter(query3, column3);
    if(query4.size() > 0) this->addSearchFilter(query4, column4);

    this->largeTable->markRowsSearch(this->searchFilters, type, caseSensitive);
    tableWidget->setRowCount(largeTable->tableData.length());
    int currPos = tableWidget->verticalScrollBar()->value();
    this->updateData(currPos, true);
}

/**
 * @brief TableData::clearSearchFilters, removes the search filters added so far
 */
void TableData::clearSearchFilters() {
    this->searchFilters.clear();
}


/**
 * @brief TableData::initializeSearchFilter adds a search filter
 * @param query, the query
 * @param column, column chosen at the export box
 * @param caseSensitive
 */
void TableData::addSearchFilter(QString query, int column) {

    SEARCH searchFilter;
    searchFilter.searchItem = query.trimmed();


    if( column==0) {
        for(unsigned int i = 0; i < this->numCols; i++) {
            if( this->types[i]==STRING)
               searchFilter.searchCols.append(i);
        }
    }
    else {
        searchFilter.searchCols.append(column-1);
    }

    this->searchFilters.append(searchFilter);
}

void TableData::searchButtonPressed(){
    this->searchWidget = new SearchWidget(this);
    this->searchWidget->show();
}

void TableData::exportButtonPressed(){
    this->exportBox = new ExportBox(this, 0 , this->exportType);
    this->exportBox->setAuxName(this->auxName);
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
         this->populateTable(from );
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
           this->populateTable(top);
        }

        //this->exportBox = new ExportBox(this);
}


void TableData::setupFromFile(){

        largeTable->getData(bigdata, 0,dw);

        tableWidget->setRowCount(largeTable->tableData.length());
        tableWidget->setColumnCount(largeTable->colNames.length());
        headers = largeTable->colNames;

        if(!bigdata.empty() ) {
           this->populateTable(0);
        }

}


//select columns variant
void TableData::setupFromFile(const QString &file, QList<unsigned int> & columns, QRegExp filter){

        TableManager *tableManager = TableManager::getTableManager();

        largeTable = tableManager->getTable(file);

        if( largeTable ==0 ) {
           largeTable = new LargeTable(file, '\t', true, true, types, columns, filter);
           if (CACHE) tableManager->setTable(file, largeTable);
        }

        largeTable->getData(bigdata, 0, dw);
        tableWidget->setRowCount(largeTable->tableData.length());
        tableWidget->setColumnCount(largeTable->colNames.length());
        headers = largeTable->colNames;

        if(!bigdata.empty() ) {
            this->populateTable(this->top);
        }

      //  this->exportBox = new ExportBox(this);
}

/*
 * Creates the actual table widget the data given to it, and populates the table with data and headers given with
 * the pointer to the table given.
 */

/**
 * @brief TableData::populateTable Creates the actual table widget the data given to it, and populates
 * the table with data and headers given with the pointer to the table given.
 * @param table
 * @param data
 * @param headers
 * @param from
 */
void TableData::populateTable(int top){

    tableWidget->setColumnCount(this->headers.size());
    tableWidget->setHorizontalHeaderLabels(this->headers);

    int k = top;
    tableWidget->clearContents();
    foreach( ROW *datum,  bigdata) {
      for( unsigned int i = 0; i < largeTable->numCols; i++) {
          try{

              //table.setItem(k,i,new QTableWidgetItem("aldkfjglkjdsfgjdfsjglkjdsfgjldfkgjjdfglkdfgjldfjgljdsflkjlsdkfjglkjsdflgjlkdsfjgljdfslgjldfgljdsflkgjkdfsjlgkja;sdkfj;alwkj;l435u3209"));
              if( types[i] == INT) {
                 tableWidget->setItem(k,i, new QTableWidgetItem(QString::number(datum->intVar.at(largeTable->index[i]))));
              }
              if( types[i] == DOUBLE) {
                  tableWidget->setItem(k,i,new QTableWidgetItem(QString::number(datum->doubleVar.at(largeTable->index[i]))));
              }
              if(types[i] == STRING) {

                  tableWidget->setItem(k,i,new QTableWidgetItem(QString(datum->strVar.at(largeTable->index[i]))));
              }

          }
          catch(...) {
              qDebug() << " Failed at " << i << datum << " double " << datum->doubleVar << " int " << datum->intVar << " str " << datum->strVar;

          }
      }
      k++;
    }
    tableWidget->resizeColumnsToContents();
    //table.resizeRowsToContents();
    tableWidget->horizontalHeader()->setStretchLastSection(true);
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


bool TableData::saveTableToFile(QString fileName, QChar delim, const QStringList &selectedHeaders) {

    QFile outFile(fileName);

    if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outFile);
        //QLabel *waitScreen = Utilities::ShowWaitScreen("Please wait while the file is being written!");
        QProgressBar progressBar;

        QHash<QString, bool> includeColumn;
        foreach(QString header, this->largeTable->colNames) {
            includeColumn[header] = false;
        }

        foreach(QString header, selectedHeaders) {
            includeColumn[header]= true;
        }


        bool delimOn = false;
        foreach(QString header, this->largeTable->colNames) {
            if( !includeColumn[header])
                continue;
            else {
                if(delimOn)  out << delim;
                delimOn = true;
                out << header;
            }

        }
        out << "\n";

        unsigned int interval =  this->largeTable->tableData.size() > 100 ? this->largeTable->tableData.size()/100 : 10;

        progressBar.setRange(0, this->largeTable->tableData.size());
        progressBar.show();

        for(int i =0; i < this->largeTable->tableData.size();  i++) {   
            bool delimOn = false;
            for(int j =0; j < this->tableWidget->columnCount(); j++)  {
                 if( i%interval ==0) { progressBar.setValue(i); qApp->processEvents();  progressBar.update(); }
                 if( !includeColumn[this->largeTable->colNames[j]])
                     continue;
                 else {
                     if(delimOn)  out << delim;
                     delimOn = true;
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

             }
             out << "\n";
        }

       // waitScreen->hide();
        progressBar.hide();

        outFile.close();
    }
    return true;

}



bool TableData::saveSequencesToFile( QString fileName,  RESOURCE type) {
    QFile outFile(fileName);



    if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outFile);

        //type can be only of NUCFASTA, NUCFNA or AMINOFAA
        SampleResourceManager *sampleResourceManager = SampleResourceManager::getSampleResourceManager();
        FileIndex *fileIndex = sampleResourceManager->getFileIndex(this->getSampleName(), type);

        if( this->type == rRNATABLE) type = rRNA;
        if( this->type == tRNATABLE) type = tRNA;

        QString seqType;

        //change to the proper type if the contigs are actually specific type of sequences
        // such rRNA or tRNA

        switch(type) {
            case NUCFNA:
                 seqType = "ORFs";
                 break;
            case AMINOFAA:
                 seqType= "Amino Acid";
                 break;
            case NUCFASTA:
                 seqType = "Contigs";
                 break;
            case rRNA:
                 seqType = "rRNA";
                 break;
            case tRNA:
                 seqType = "tRNA";
                 break;
            default:
                 seqType = "Unknown";
                 break;
        }

        ProgressView progressbar("Saving " + seqType + " sequences for sample : " + sampleName, 0, 0, this);

        if( fileIndex !=0 ) {
            unsigned int col;
            if( type == NUCFNA || type == AMINOFAA ) col = 0;
            if( type == rRNA || type==tRNA) col = 0;
            if( type == NUCFASTA) col = 4;

            QString name, resultStr, shortname;
            QHash<QString, bool> alreadyWritten;
          //  qDebug() << "Size to export " << this->largeTable->tableData.size();

            QHash<QString, unsigned int> orfNum;

            for(int i =0; i < this->largeTable->tableData.size();  i++) {
               name = this->largeTable->tableData[i]->strVar[this->largeTable->index[col]];

               QString seqName;

               if(type==NUCFASTA  || type ==rRNA || type == tRNA) {
                  shortname = Utilities::getShortContigId(name);
                  seqName = shortname;
               }

               if(type==NUCFNA || type==AMINOFAA) {
                  shortname = Utilities::getShortORFId(name);
                  seqName = shortname;
               }

               unsigned int start=0, end=0, col1, col2;
               if(type ==rRNA || type == tRNA) {
                   QString typeTag;
                   if(type==rRNA ) {
                       col1=1; col2=2;
                       typeTag = "_rRNA_";
                   }
                   if( type==tRNA ){
                       col1= 3; col2 = 4;
                       typeTag = "_tRNA_";
                   }

                   if( !orfNum.contains(shortname)) orfNum[shortname] = -1;
                   orfNum[shortname]++;
                   seqName = this->sampleName + QString("_") +  shortname + typeTag + QString::number(orfNum[shortname]);
               }


               if(alreadyWritten.contains(seqName) ) continue;
               alreadyWritten[seqName] = true;

               if(type ==rRNA || type == tRNA) {
                   resultStr = fileIndex->getDataToDisplay(shortname);

                   start = this->largeTable->tableData[i]->intVar[this->largeTable->index[col1]];

                   end = this->largeTable->tableData[i]->intVar[this->largeTable->index[col2]];

                   resultStr = Utilities::getSubSequence(resultStr,  seqName, start, end);
               }
               else {
                   resultStr = fileIndex->getDataToDisplay(shortname);
               }

               if( !resultStr.isEmpty()) { out << resultStr; }
            }
        }

        progressbar.hide();
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
