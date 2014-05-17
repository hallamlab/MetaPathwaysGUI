#include "htabledata.h"
#include "ui_htabledata.h"
#include <QTableWidget>

HTableData::HTableData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HTableData)
{
    ui->setupUi(this);

    this->multiSampleMode = false;

    tableWidget = this->findChild<QTableWidget *>("tableWidget");
    showHierarchy  = this->findChild<QCheckBox *>("showHierarchy");
    showDepth = this->findChild<QSpinBox *>("showDepth");
    viewToggleBox  = this->findChild<QCheckBox *>("viewToggleBox");
    categorySelector = this->findChild<QComboBox *>("categorySelector");
    subCategoryName = this->findChild<QLabel *>("subCategoryName");
    depthLabelValue =this->findChild<QLabel *>("depthLabelValue");
    hideZeroRows = this->findChild<QCheckBox *>("hideZeroRows");


#ifdef SECTION
    tableWidget->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
#else
  //  tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

    HTABLEIDENTITY a;
   // order is important
    a.attrType = KEGG; a.sampleName="KEGG";
    htableIdentities << a;
    a.attrType = COG; a.sampleName="COG";
    htableIdentities << a;
    a.attrType = METACYC; a.sampleName="METACYC";
    htableIdentities << a;
    a.attrType = SEED; a.sampleName="SEED";
    htableIdentities << a;

    foreach(HTABLEIDENTITY identity, htableIdentities) {
       categorySelector->addItem(identity.sampleName);   ///sample name is a short cut and wrong
    }

    depthLabelValue->hide();
    subCategoryName->hide();
    viewToggleBox->hide();
    categorySelector->hide();

    this->subWindow = false;

    this->level = 0;

    connect(showDepth, SIGNAL(valueChanged(int) ), this, SLOT(spinBoxChanged(int) ) );
    /*
    connect(showHierarchy, SIGNAL(stateChanged(int)), this, SLOT(showHierarchyChanged(int) ) );
    connect(hideZeroRows, SIGNAL(stateChanged(int)), this, SLOT(hideZeroRowsChanged(int) ) );
    */

    connect(showHierarchy, SIGNAL(stateChanged(int)), this, SLOT(showHierarchyOrZeroRowToggleChanged()) );
    connect(hideZeroRows, SIGNAL(stateChanged(int)), this, SLOT(showHierarchyOrZeroRowToggleChanged()) );


    connect(tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT( showInformativeTable(QTableWidgetItem *) ));
    connect(categorySelector, SIGNAL(currentIndexChanged(int)), this, SLOT(switchCategory(int)) );


    colors  << QBrush(Qt::black) <<  QBrush(Qt::red) << QBrush(Qt::blue) << QBrush(QColor(0,153,0)) << QBrush(QColor(255,128,0)) <<  QBrush(QColor(102,0,102)) << QBrush(QColor(76,0,183)) << QBrush(QColor(204,0,102))   << QBrush(Qt::gray);
    indents << QString("")       << QString("\t")    << QString("\t\t")  << QString("\t\t\t")       << QString("\t\t\t\t")       <<  QString("\t\t\t\t\t")     << QString("\t\t\t\t\t\t")  << QString("\t\t\t\t\t\t\t")   << QString("\t\t\t\t\t\t\t\t")   ;

    searchButton = this->findChild<QPushButton *>("searchButton");
    exportButton = this->findChild<QPushButton *>("exportButton");


    connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButtonPressed()));
    connect(exportButton, SIGNAL(released()), this, SLOT(exportButtonPressed()));
}

HTableData::~HTableData()
{
    delete ui;
}

void HTableData::setMaxSpinBoxDepth(unsigned int maxDepth) {
    this->maxSpinBoxDepth  = maxDepth;
    showDepth->setMaximum(maxDepth);

    showDepth->setMinimum( maxDepth > 0 ? 1 :maxDepth);
}

void HTableData::setShowHierarchy(bool flag) {
    this->showHierarchy->setChecked(flag);
}


void HTableData::searchButtonPressed(){
    this->searchWidget = new SearchWidget(this);
    this->searchWidget->show();
}

void HTableData::exportButtonPressed(){
    this->exportBox = new ExportBox(this);
    this->exportBox->show();
}

void HTableData::setMultiSampleMode(bool multisample) {
    this->multiSampleMode = multisample;
}

void HTableData::addSampleName(QString sampleName) {
    if( !this->sampleNames.contains(sampleName))
        this->sampleNames.append(sampleName);
}

bool HTableData::isMultiSampleMode() {
    return this->multiSampleMode;
}


QStringList HTableData::getSampleNames() {
    return this->sampleNames;
}

void HTableData::setSampleNames(QStringList sampleNames) {
    this->sampleNames = sampleNames;
}


QString HTableData::getSampleName(unsigned int i) {
   if( this->sampleNames.size() > i )  {
       return this->sampleNames[i];
   }
   else
       return QString("");
}


bool HTableData::setParameters(HTree *htree,  QList<TYPE> _types) {

    this->setNumCols(_types.size());

    this->types = _types;

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

QStringList HTableData::getHeaders(){
    return this->headers;
}

QString HTableData::getHeader(unsigned int i){
    if(i >= this->headers.size()) return QString();
    return this->headers.at(i);
}

enum TYPE HTableData::getFieldType(unsigned int i) {
    if( i >= this->types.size() )
        return UNKNOWN;
    return this->types.at(i);
}

void HTableData::setHeaders(QStringList &headers) {
    this->headers = headers;
}

void HTableData::setTableIdentity(QString sampleName, ATTRTYPE attrType) {
   this->id.attrType = attrType;
   this->id.sampleName = sampleName;
}

void HTableData::spinBoxChanged(int depth) {
    if(this->subWindow)
       // this->showSelectedTableData(this->category);
    this->showTableData();
    else
        this->showTableData();
}


void HTableData::showTableData(bool hideZeroRows) {
    this->fillData((this->showHierarchy->checkState()==Qt::Checked), hideZeroRows);

}


void HTableData::showHierarchyOrZeroRowToggleChanged() {
    if(this->subWindow) {
        this->showSelectedTableData(this->category, (this->hideZeroRows->checkState()==Qt::Checked));
        qDebug() << "Show seleced Table Data";
    }
    else {
        qDebug() << "show Table Data";
        this->showTableData((this->hideZeroRows->checkState()==Qt::Checked));
    }
}


void HTableData::showHierarchyChanged(int state) {
    if(this->subWindow) {
        this->showSelectedTableData(this->category);
//    this->showTableData();
        qDebug() << "Show seleced Table Data";
    }
    else {
        qDebug() << "show Table Data";
        this->showTableData();
    }
}


/* This function toggels to show and display the zero rows of the functional
 *tables
 * @params true/false settings to enable or disable based on the Checkbox state
 **/
void HTableData::hideZeroRowsChanged(int state) {
    if(this->subWindow)
//        this->showSelectedTableData(this->category,  (this->hideZeroRows->checkState()==Qt::Checked));
    this->showTableData( (this->hideZeroRows->checkState()==Qt::Checked));

    else
        this->showTableData( (this->hideZeroRows->checkState()==Qt::Checked));
}


/* This function will get the data row to display in the funcational table
 \param  state is to hide or unhide zero rows
 \param hideZeroRows   if this bool is true then the zero rows are not included
 */

void HTableData::fillData(bool state, bool hideZeroRows) {
     QList<ROWDATA *> data;

     data =  this->htree->getRows(this->showDepth->value(), state, this->connectors);
     QList<ROWDATA *> newdata;

     if(hideZeroRows) {
        foreach(ROWDATA *r, data) {
            if( isNonZero(r)) newdata.append(r);
        }
        this->populateTable(newdata, this->headers, state );
     }
     else {
        this->populateTable(data, this->headers, state );

     }
}

unsigned int HTableData::showSelectedTableData(QString categoryName, bool hideZeroRows) {

     qDebug() << "show hieracrhy " << (this->showHierarchy->checkState()==Qt::Checked);
     return this->fillSelectedData(categoryName, this->showDepth->value(), (this->showHierarchy->checkState()==Qt::Checked), hideZeroRows );
}

bool HTableData::isNonZero(ROWDATA *r) {
    QVector<unsigned int>::const_iterator  it;
    for(it = r->counts.begin(); it!=r->counts.end(); it++) {
        if(*it > 0) return true;
    }
    return false;
}

unsigned int HTableData::fillSelectedData(QString categoryName, unsigned int maxDepth, bool showHierarchy, bool hideZeroRows) {
     QList<ROWDATA *> data;
     data =  this->htree->getRows(categoryName, maxDepth, showHierarchy, this->connectors);
     qDebug() << " Data size retrieved " << data.size() << " at depth " << maxDepth << " category " << categoryName;
     QList<ROWDATA *> newdata;
     if(hideZeroRows) {
        foreach(ROWDATA *r, data) {
            if( isNonZero(r)) newdata.append(r);
        }
        this->populateTable(newdata, this->headers, showHierarchy );
        return newdata.size();
     }
     else {
        this->populateTable(data, this->headers, showHierarchy );
        return data.size();
     }
}

void HTableData::populateTable( QList<ROWDATA *> &data, const QStringList &headers, bool hierarchyEnabled){

    tableWidget->clear();
    tableWidget->setColumnCount(headers.size());
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setRowCount(data.size());
    tableWidget->setColumnCount(types.size());
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


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
//    tableWidget->resize(tableWidget->sizeHint());
}


void HTableData::showInformativeTable(QTableWidgetItem *item) {
    unsigned int row = item->row();

    DataManager *datamanager = DataManager::getDataManager();
    HTableData *htable = new HTableData;
    htable->setAttribute(Qt::WA_DeleteOnClose); // frees up memory once it's closed
    htable->setMultiSampleMode(this->isMultiSampleMode());
    htable->setSampleNames(this->sampleNames);
    htable->clearConnectors();

    htable->viewToggleBox->setVisible(true);
    htable->categorySelector->setVisible(true);
    htable->categorySelector->setCurrentIndex(this->id.attrType);
    htable->subWindow = true;
    htable->category = tableWidget->item(row,0)->text().trimmed();
    htable->level = this->level +1;
    htable->types = this->types;


    htable->setWindowTitle(htable->category);
    HTree *htree = datamanager->getHTree(this->id.attrType);
    HNODE *hnode = htree->getHNODE(htable->category);



    htable->subCategoryName->setVisible(true);
    if(hnode != 0)
       htable->subCategoryName->setText(hnode->attribute->alias);
    else
       htable->subCategoryName->setText(htable->category);

    // set the htable levels
    htable->depthLabelValue->setText(QString::number(htable->level));
    htable->depthLabelValue->setVisible(true);
  //  htable->connectors = this->connectors;

    htable->connectors.clear();
    Connector *modConnector ;


    // create the subConnectors from the already existing connectors in the current
    // htable

    foreach(Connector *connector, this->connectors) {
        modConnector = datamanager->createSubConnector(datamanager->getHTree(this->id.attrType), hnode, connector, this->id.attrType);
        htable->allConnectors[this->id.attrType].append(modConnector);
      //  qDebug() << " submodCon size " << modConnector->getNumOfORFs() << " atr sub" << this->id.attrType ;
    }


    QList<ORF *>orfList;
    for(unsigned int i =0; i < this->htableIdentities.size(); i++) {
        if( this->id.attrType== this->htableIdentities[i].attrType) continue;
        foreach(Connector *connector, this->connectors) {
           orfList = connector->getORFList(htree->getLeafAttributesOf(hnode));
           modConnector = datamanager->createConnector("temp", datamanager->getHTree(this->htableIdentities[i].attrType), this->htableIdentities[i].attrType, &orfList);
           htable->allConnectors[this->htableIdentities[i].attrType].append(modConnector);
         //  qDebug() << " get modCon size " << modConnector->getNumOfORFs() << " atr " << this->htableIdentities[i].attrType ;

        }
    }

    htable->setHeaders( this->headers);
    htable->setTableIdentity(this->id.sampleName, this->id.attrType);
    htable->switchCategory(this->id.attrType);


    // kishori's changes
//    HTabWidget *htab = new HTabWidget(htable->category,  ":images/cross.png");
//    ToolBarManager *toolbarManager = ToolBarManager::getToolBarManager();
//    toolbarManager->addTab(htab,  htable);
//    WidgetStacker *wStacker = WidgetStacker::getWidgetStacker();
//    wStacker->stackWidget(htable);
  //  mdiAreaWidget->getMdiArea()->cascadeSubWindows();

}

/* this function is activated when the user switched the functional
 *category--KEGG, COG, MetaCyc, SEED are functional categories--
 *\param index : decides which of the categories is select from the drop down list at the
 *               top of the Htable
 **/
void HTableData::switchCategory(int index) {

    DataManager *datamanager = DataManager::getDataManager();

    if( this->htableIdentities.length() <= index  ) return;
    if( !this->allConnectors.contains(this->htableIdentities[index].attrType)) return;
    this->connectors = this->allConnectors[this->htableIdentities[index].attrType];

    if(datamanager->getHTree(htableIdentities[index].attrType)==NULL) return;
    this->setParameters(datamanager->getHTree(htableIdentities[index].attrType), this->types);

    this->setMaxSpinBoxDepth(datamanager->getHTree(htableIdentities[index].attrType)->getTreeDepth());
    this->setShowHierarchy(true);

    /*
    qDebug() << "KEGG " <<  this->allConnectors[KEGG][0]->getNumOfORFs();
    qDebug() << "COG " <<  this->allConnectors[COG][0]->getNumOfORFs();
    qDebug() << "MetaCyc " <<  this->allConnectors[METACYC][0]->getNumOfORFs();
    qDebug() << "SEED " <<  this->allConnectors[SEED][0]->getNumOfORFs();

    qDebug() << "Switched to " << this->id.attrType << "    " <<  this->htableIdentities[index].attrType;
*/
    this->level = datamanager->getHTree(htableIdentities[index].attrType)->getTreeDepth() + 1;


    if(this->id.attrType==this->htableIdentities[index].attrType) {
        if( this->showSelectedTableData(this->category) !=0 )
            this->show();
        else
            this->hide();
    }
    else {
        this->showTableData();
    }
    this->setTableIdentity(this->id.sampleName, this->htableIdentities[index].attrType);

}

bool HTableData::saveTableToFile(QString fileName, QChar delim, const QStringList &selectedHeaders) {
    QFile outFile(fileName);
    if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outFile);
        bool delimOn;

        QHash<QString, bool> includeColumn;
        foreach(QString header, this->headers) {
            includeColumn[header] = false;
        }

        foreach(QString header, selectedHeaders) {
            includeColumn[header]= true;
        }

        int j = 0;
        delimOn = false;
        foreach(QString header, this->headers) {
            if(delimOn)  out << delim;
            if( !includeColumn[header])
                continue;
            else
                delimOn = true;
            out << header;
        }
        out << "\n";


        for(int i =0; i < this->tableWidget->rowCount(); i++) {
            if( this->tableWidget->isRowHidden(i) ) continue;
             delimOn = false;
             for(int j =0; j < this->tableWidget->columnCount(); j++)  {
                 if( !includeColumn[this->headers[j]])
                     continue;
                 else {
                      if(delimOn)  out << delim;
                      delimOn = true;
                      out << this->tableWidget->item(i,j)->text();
                 }
             }
             out << "\n";
        }
        outFile.close();
    }
    return true;
}


/* Saves the sequeces to a local file
 * \param sampleName : name of the sample
 * \param fileName : name of the file/folder to save it in, creates a new folder
 * \param type: type of sequences to export to the file
 */
bool HTableData::saveSequencesToFile(QString sampleName, QString fileName,  RESOURCE type) {
    QFile outFile(fileName);
    DataManager *datamanager = DataManager::getDataManager();
    HTree *htree = datamanager->getHTree(this->id.attrType);

    // take any connector, since they all have the same set of ORFs
    Connector *connector  = this->connectors[0];
    if( datamanager==0 || htree ==0 || connector == 0 ) return false;
    QList<ORF *>orfList = connector->getORFList();

    QHash<QString, bool> keyNames;
    ProgressView progressbar("Saving sequences for sample : " + sampleName, 0, 0, this);
 //   qDebug() << "total orfs " << connector->getORFList().size();
   // foreach( ORF *o, connector->getORFList())
     //   qDebug() << o->name;

   // for(int i =0; i < this->tableWidget->rowCount(); i++) {
       //  HNODE *hnode = htree->getHNODE(this->tableWidget->item(i,0)->text().trimmed());
        // orfList = connector->getORFList(htree->getLeafAttributesOf(hnode));
         foreach(ORF *orf, orfList) {
             if( type == NUCFNA || type == AMINOFAA) {
              //   qDebug() << " orf name " << orf->name;
                 keyNames[orf->name] = true;
             }
             if( type==NUCFASTA) {
              //   qDebug() << "contig name " << orf->contig->name;
                 keyNames[orf->contig->name] = true;
             }
         }
   // }


    SampleResourceManager *sampleResourceManager = SampleResourceManager::getSampleResourceManager();
    FileIndex *fileIndex = sampleResourceManager->getFileIndex(sampleName, type);

    if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outFile);
        foreach(QString name, keyNames.keys()) {
           out << fileIndex->getDataToDisplay(name);
        }
        outFile.close();
    }

    progressbar.hide();
    return true;
}


void HTableData::searchQuery(QString query, int column, bool caseSensitive){

    qDebug() << "Searching in Htable ";
   // this->initializeSearchFilter(query, column, caseSensitive);

    for( int i = 0; i < tableWidget->rowCount(); ++i )
    {
        bool match = false;
        for( int j = 0; j < tableWidget->columnCount(); ++j )
        {
            QTableWidgetItem *item = tableWidget->item( i, j );
            if( item->text().contains(query) )
            {
                match = true;
                break;
            }
        }
        tableWidget->setRowHidden(i, !match );
    }

}

void HTableData::initializeSearchFilter(QString query, int column, bool caseSensitive) {

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
