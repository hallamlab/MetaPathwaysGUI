
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
    alpha  = this->findChild<QComboBox *>("alphaValue");
    lcaDepth  = this->findChild<QSpinBox *>("lcaDepth");
    functionTable = this->findChild<QPushButton *>("functionTable");



    connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButtonPressed()));
    connect(exportButton, SIGNAL(clicked()), this, SLOT(exportButtonPressed()));

    connect(alpha, SIGNAL(currentIndexChanged(int)) , this, SLOT(updateLCAStar(int)) );
    connect(lcaDepth, SIGNAL(valueChanged(int)) , this, SLOT(updateLCAStar(int)) );

    connect(tableWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(outputRows(int)));
    connect(tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerClicked(int)));

  //  this->searchWidget = new SearchWidget(this);
   // this->searchWidget->hide();

    connect(functionTable, SIGNAL(clicked()), this, SLOT(spawnFunctionTable()) );

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
    this->addLCAStar = false;
}

/**
 * @brief TableData::setParameters, sets the parameters for the table to read and
 * present the data
 * @param HAS_COMMENT
 * @param file
 * @param _types
 * @param CACHE
 * @return
 */
bool TableData::setParameters(bool HAS_COMMENT, const QString &file, QList<TYPE> _types, bool CACHE) {

    top = 0;
    bottom = 0;

    this->CACHE = CACHE;

    this->setNumCols(_types.size());
    foreach(enum TYPE type, _types) {
        types.append(type);
    }

    if(this->addLCAStar) {
        types.append(STRING);
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



void TableData::addLCAStarColumn() {
    foreach( ROW *row,this->largeTable->wholeTableData ) {
        row->strVar.append(QString("-"));
    }
    this->largeTable->colNames.append("LCAStar");
    this->largeTable->numCols++;
    this->largeTable->addLCAStar =true;
}

// with select columns
/**
 * @brief TableData::setParameters
 * @param HAS_COMMENT
 * @param file
 * @param _types
 * @param _column-
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

/**
 * @brief TableData::createGeneViewData, prepare the data to be displayed by the
 * geneviewer
 * @param item
 */
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

/**
 * @brief TableData::isMultiSampleMode checks if the table is from the multisample mode
 * @return  boolean
 */
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

/**
 * @brief TableData::useLCAStar, decides if the table computes the LCA value
 * or not
 * @param value
 */
void TableData::useLCAStar(bool value) {
    this->addLCAStar = value;
}

/**
 * @brief TableData::hideLCAControl, hides the LCA controls from the rRNA and tRNA tables
 */
void TableData::hideLCAControl() {

    this->lcaDepth->hide();
    this->alpha->hide();

}

/**
 * @brief TableData::hideFunctionalPopupControl, hides the functional popup table from rRNA
 * and tRNA tables
 */
void TableData::hideFunctionalPopupControl() {
    this->functionTable->hide();
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

/**
 * @brief TableData::searchButtonPressed, clicked on the search button to enable the user
 * to be able to search on the tables
 */
void TableData::searchButtonPressed(){
    this->searchWidget = new SearchWidget(this);
    this->searchWidget->show();
}


/**
 * @brief TableData::exportButtonPressed, the export weidget is shows to enable
 * the user to export data
 */
void TableData::exportButtonPressed(){
    this->exportBox = new ExportBox(this, 0 , this->exportType);
    this->exportBox->setAuxName(this->auxName);
    this->exportBox->show();
}

void TableData::setNumCols(unsigned int numCols) {
    this->numCols = numCols;
}

/**
 * @brief TableData::spawnFunctionTable, spawns a few function table
 */
void TableData::spawnFunctionTable() {


    QList<QString> orfNames = this->largeTable->getORFNames();

    QHash<QString, bool> orfNamesHash;
    foreach(QString orfName, orfNames) {
        orfNamesHash[orfName] = true;
    }
    DataManager *datamanager = DataManager::getDataManager();

    QList<ORF *> orflist = datamanager->getORFByNames(this->sampleName, orfNamesHash);

    HTableData::spawnInformativeTable(this->sampleName, orflist);


}

/**
 * @brief TableData::headerClicked, clicked the header
 * @param index, the column that was clicked
 */
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


/**
 * @brief TableData::updateLCAStar, recompute the LCA Star
 * @param index
 */
void TableData::updateLCAStar(int index) {
     this->updateData(this->top, true);
}


/**
 * @brief TableData::updateData, used for loading partial rows of the table to give the illusion to the
 * user of having the entire table loaded at once
 * @param top
 * @param reload
 */
void TableData::updateData(double top, bool reload){
    int pivot =  top;
    if (largeTable->getPivot() -0.9*dw >=  pivot || pivot >= largeTable->getPivot()  + 0.9 *dw || reload){
         largeTable->setPivot(pivot);
         int from = pivot > dw ? pivot -dw : 0;
         largeTable->setLCAStarAlpha(this->alpha->currentText().toDouble()/100);
         largeTable->setLCAStarDepth(this->lcaDepth->value());
         largeTable->getData(bigdata, from, dw);
         this->populateTable(from );
    }
}

/**
 * @brief TableData::setupFromFile, loads the dat from the tab delimited file and populates
 * the tab widget for the largetable
 * @param file
 */
void TableData::setupFromFile(const QString &file){

        TableManager *tableManager = TableManager::getTableManager();

        largeTable = tableManager->getTable(file);

        if( largeTable ==0 ) {
           largeTable = new LargeTable(file, '\t', true, true, types);
           if( this->addLCAStar ) {
               this->addLCAStarColumn();
               largeTable->sampleName = this->sampleName;
           }

           largeTable->copyWholeToFilter();

           if (CACHE) tableManager->setTable(file, largeTable);


        }
        largeTable->setLCAStarAlpha(this->alpha->currentText().toDouble()/100);
     //   largeTable->setLCAStarDepth(this->);

        largeTable->getData(bigdata, 0,dw);
        tableWidget->setRowCount(largeTable->tableData.length());
        tableWidget->setColumnCount(largeTable->colNames.length());
        headers = largeTable->colNames;

        if(!bigdata.empty() ) {
           this->populateTable(top);
        }

        //this->exportBox = new ExportBox(this);
}

/**
 * @brief TableData::setupFromFile, reads the table from the file
 */
void TableData::setupFromFile(){
        largeTable->setLCAStarAlpha(this->alpha->currentText().toDouble()/100);
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

        largeTable->setLCAStarAlpha(this->alpha->currentText().toDouble()/100);
        largeTable->getData(bigdata, 0, dw);
        tableWidget->setRowCount(largeTable->tableData.length());
        tableWidget->setColumnCount(largeTable->colNames.length());
        headers = largeTable->colNames;

        if(!bigdata.empty() ) {
            this->populateTable(this->top);
        }

      //  this->exportBox = new ExportBox(this);
}

/**
 * @brief TableData::computeLCAStarValue, computes the lcastar value and the tooltips for the
 * the rows that are to be displayed
 * @param lcainfo
 */
void TableData::computeLCAStarValue(QHash<QString, LCASTARINFO> &lcainfoHash) {
    unsigned int col1 = 4;

    QHash<QString, QStringList> taxonsForContigs;
    LCAStar *lcastar = LCAStar::getLCAStar();
    DataManager *datamanager = DataManager::getDataManager();
    foreach( ROW *datum,  bigdata) {
        try{
            QString contigName = datum->strVar.at(largeTable->index[col1]);

            if( !taxonsForContigs.contains(contigName) ) {
                taxonsForContigs[contigName] = QStringList();
                QList<ORF *> orfs = datamanager->getORFList(this->sampleName, Utilities::getShortContigId(contigName));
                foreach(ORF *orf,  orfs) {
                   taxonsForContigs[contigName].append(orf->attributes[TAXON]->name);
                }
            }
        }
        catch(...) {
            continue;
        }
    }


    lcainfoHash.clear();
    foreach(QString contigName, taxonsForContigs.keys() ) {
        if( lcainfoHash.contains(contigName )) continue;
        LCASTARINFO lcainfo;
        lcainfo.lcaStar =   lcastar->lca_star(taxonsForContigs[contigName]);

        FREQUENCY __lca__star;
        __lca__star.name = lcainfo.lcaStar.taxon;
        __lca__star.count = lcainfo.lcaStar.count;

        lcainfo.tooltip = lcastar->getToolTipText(taxonsForContigs[contigName], __lca__star) ;

        lcainfoHash[contigName] = lcainfo;
    }
}
/**
 * @brief TableData::populateTable Creates the actual table widget the data given to it, and populates
 * the table with data and headers given with the pointer to the table given.
 * @param table
 * @param data
 * @param headers
 * @param from
 */
void TableData::populateTable(int top){

    QHash<QString, LCASTARINFO> lcainfo;
//    qDebug() << this->headers;
    QStringList headers = this->headers;

    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);
    QHeaderView *HorzHdr = tableWidget->horizontalHeader();
    HorzHdr->setStretchLastSection(true);

    int k = top;
    tableWidget->clearContents();



    if(this->addLCAStar) {
        computeLCAStarValue(lcainfo);
    }

    QTableWidgetItem *item;
    NCBITree *ncbitree = NCBITree::getNCBITree();

    QString string;

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
                  string = datum->strVar.at(largeTable->index[i]);
                  if(i==8) string=ncbitree->getLineage(string);
                  tableWidget->setItem(k,i,new QTableWidgetItem(string));
              }

          }
          catch(...) {
              qDebug() << " Failed at " << i << datum << " double " << datum->doubleVar << " int " << datum->intVar << " str " << datum->strVar;

          }
      }


     if(this->addLCAStar) {
            try{
               item = tableWidget->item(k, largeTable->numCols -1);
               item->setToolTip( lcainfo[datum->strVar.at(largeTable->index[4]) ].tooltip);
             //  item->setText(ncbitree->getLineage(lcainfo[datum->strVar.at(largeTable->index[4]) ].lcaStar.taxon));
               QString lineage =  ncbitree->getLineage(lcainfo[datum->strVar.at(largeTable->index[4]) ].lcaStar.taxon);
               item->setText(lineage);
            }
            catch(...) {

            }
      }
      k++;
    }
    tableWidget->resizeColumnsToContents();
    tableWidget->horizontalHeader()->setStretchLastSection(true);
}




/*
 * Parses a result file. If HAS_COMMENT is true, then it expects that there are lines with DELIM in certain lines
 * of the text file.
 */
/**
 * @brief TableData::parseFile, Parses a result file. If HAS_COMMENT is true,
 * then it expects that there are lines with DELIM in certain lines
 * of the text file
 * @param file
 * @param data
 * @param DELIM
 * @param HAS_COMMENT
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



/**
 * @brief TableData::saveSequencesToFile, saves the sequences for the current sample samplename in to
 * the file fileName
 * @param fileName, the file where the sequences are save
 * @param type, the type of table the sequences are related to
 * @return
 */
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

/**
 * @brief TableData::setSampleName, sets the current sample name
 * @param sampleName
 */
void TableData::setSampleName(QString sampleName){
    this->sampleName = sampleName;
}


/**
 * @brief TableData::getSampleName, gets the current sample name
 * @return
 */
QString TableData::getSampleName() {
    return this->sampleName;
}

TableData::~TableData()
{
    delete this->largeTable;
    delete this->tableWidget;
    delete ui;
}
