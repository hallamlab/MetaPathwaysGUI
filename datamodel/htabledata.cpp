#include "htabledata.h"
#include "ui_htabledata.h"
#include <QTableWidget>


HTableData::HTableData(QWidget *parent, int spinBoxValue, bool _showHierachy, bool _hideZeroRows, VALUETYPE _valueType) :
    QDialog(parent),
    ui(new Ui::HTableData)
{
    ui->setupUi(this);

    this->multiSampleMode = false;

    tableWidget = this->findChild<QTableWidget *>("tableWidget");
    showHierarchy  = this->findChild<QCheckBox *>("showHierarchy");
    showDepth = this->findChild<QSpinBox *>("showDepth");
    numOrfsLabel  = this->findChild<QLabel *>("numOrfsLabel");
    categorySelector = this->findChild<QComboBox *>("categorySelector");
    subCategoryName = this->findChild<QLabel *>("subCategoryName");
    depthLabelValue =this->findChild<QLabel *>("depthLabelValue");
    hideZeroRows = this->findChild<QCheckBox *>("hideZeroRows");
   // showRPKM = this->findChild<QCheckBox *>("showRPKM");
    valueType = this->findChild<QComboBox *>("valueType");
    alpha  = this->findChild<QComboBox *>("alphaValue");
    lcaDepth  = this->findChild<QSpinBox *>("lcaDepth");


#ifdef SECTION
    tableWidget->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
#else
  //  tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    tableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

#ifndef LCASTAR
    alpha->hide();
    lcaDepth->hide();
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
    a.attrType = CAZY; a.sampleName="CAZY";
    htableIdentities << a;

    foreach(HTABLEIDENTITY identity, htableIdentities) {
       categorySelector->addItem(identity.sampleName);   ///sample name is a short cut and wrong
    }

    depthLabelValue->hide();
    subCategoryName->hide();
    numOrfsLabel->hide();
    categorySelector->hide();
    this->subWindow = false;
    this->level = 0;

    this->hideZeroRows->setChecked(_hideZeroRows);
    this->valueType->setCurrentIndex(_valueType);
    this->matchAlphaVisible();
    this->showHierarchy->setChecked(_showHierachy);
    this->showDepth->setValue(spinBoxValue);


    connect(this->showDepth, SIGNAL(valueChanged(int))  , this, SLOT(showHierarchyOrZeroRowToggleChanged(int)) );
    connect(this->showHierarchy, SIGNAL(stateChanged(int)), this, SLOT(showHierarchyOrZeroRowToggleChanged(int)) );
    connect(this->hideZeroRows, SIGNAL(stateChanged(int)), this, SLOT(showHierarchyOrZeroRowToggleChanged(int)) );
    connect(valueType, SIGNAL(currentIndexChanged(int) ), this, SLOT( showHierarchyOrZeroRowToggleChanged(int)) );

    connect(tableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT( showInformativeTable(QTableWidgetItem *) ));
    connect(categorySelector, SIGNAL(currentIndexChanged(int)), this, SLOT(switchCategory(int)) );


    colors  << QBrush(Qt::black) <<  QBrush(Qt::red) << QBrush(Qt::blue) << QBrush(QColor(0,153,0)) << QBrush(QColor(255,128,0)) <<  QBrush(QColor(102,0,102)) << QBrush(QColor(76,0,183)) << QBrush(QColor(204,0,102))   << QBrush(Qt::gray);
    indents << QString("")       << QString("\t")    << QString("\t\t")  << QString("\t\t\t")       << QString("\t\t\t\t")       <<  QString("\t\t\t\t\t")     << QString("\t\t\t\t\t\t")  << QString("\t\t\t\t\t\t\t")   << QString("\t\t\t\t\t\t\t\t")   ;

    searchButton = this->findChild<QPushButton *>("searchButton");
    exportButton = this->findChild<QPushButton *>("exportButton");


    connect(searchButton, SIGNAL(clicked()), this, SLOT(searchButtonPressed()));
    connect(exportButton, SIGNAL(released()), this, SLOT(exportButtonPressed()));

#ifdef LCASTAR
    connect(valueType, SIGNAL(currentIndexChanged(int)), this, SLOT( toggleAlpha(int) )   );
    connect(alpha, SIGNAL(currentIndexChanged(int)) , this, SLOT( showHierarchyOrZeroRowToggleChanged(int)) );
    connect(lcaDepth, SIGNAL(valueChanged(int)) , this, SLOT( showHierarchyOrZeroRowToggleChanged(int)) );
#endif

    // setup ...
    Q_FOREACH( QSpinBox * sp, findChildren<QSpinBox*>() ) {
            sp->installEventFilter( this );
            sp->setFocusPolicy( Qt::StrongFocus );
    }

}

bool HTableData::eventFilter( QObject * o, QEvent * e ) {
   if ( e->type() == QEvent::Wheel &&
        qobject_cast<QAbstractSpinBox*>( o ) )
   {
       e->ignore();
       return true;
   }
   return QWidget::eventFilter( o, e );
}


HTableData::~HTableData()
{
    delete ui;
}


/**
 * @brief HTableData::disableAlpha, hides the alpha value
 */
void HTableData::toggleAlpha(int vType) {
#ifdef LCASTAR
    if( vType==LCASTAR) {
        this->alpha->setEnabled(true); this->alpha->show();
        this->lcaDepth->setEnabled(true); this->lcaDepth->show();
    }
    else {
        this->alpha->setEnabled(false); this->alpha->hide();
        this->lcaDepth->setEnabled(false); this->lcaDepth->hide();
    }
    return;
#endif
}


/**
 * @brief HTableData::matchAlphaVisible, this function shows or hides the alpha value spinbox
 * depending on the current choice of valueType
 */
void HTableData::matchAlphaVisible() {
    VALUETYPE vType = this->getValueType();

#ifdef LCASTAR
    if( vType==LCASTAR) {
        this->alpha->setEnabled(true); this->alpha->show();
         this->lcaDepth->setEnabled(true); this->lcaDepth->show();
    }
    else {
        this->alpha->setEnabled(false); this->alpha->hide();
        this->lcaDepth->setEnabled(false); this->lcaDepth->hide();
    }
#endif
    return;
}

/**
 * @brief HTableData::setMaxSpinBoxDepth
 * @param maxDepth
 */
void HTableData::setMaxSpinBoxDepth(unsigned int maxDepth) {
    this->maxSpinBoxDepth  = maxDepth;
    showDepth->setMaximum(maxDepth);

    showDepth->setMinimum( maxDepth > 0 ? 1 :maxDepth);
}

void HTableData::setShowHierarchy(bool flag) {
    this->showHierarchy->setChecked(flag);
}

/**
 * @brief HTableData::searchButtonPressed, shows the search button widget to be
 * able to search on the table
 */
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

void HTableData::addSampleName(QString sampleName, bool clearPrev) {
    if( clearPrev) this->sampleNames.clear();
    if( !this->sampleNames.contains(sampleName))
        this->sampleNames.append(sampleName);
}

bool HTableData::isMultiSampleMode() {
    return this->multiSampleMode;
}

/**
 * @brief HTableData::getSampleNames
 * @return list of samples in the htable
 */
QStringList HTableData::getSampleNames() {
    return this->sampleNames;
}

/**
 * @brief HTableData::clearSampleNames
 * @return list of samples in the htable
 */
void HTableData::clearSampleNames() {
    return this->sampleNames.clear();
}

/**
 * @brief HTableData::setSampleNames, set the list of samples in the htable
 * @param sampleNames
 */
void HTableData::setSampleNames(QStringList sampleNames) {
    this->sampleNames = sampleNames;
}


/**
 * @brief HTableData::getSampleNumber, computes the number of the samples that
 * are represented by the HTableData
 * @param sampleName, the name of the sample
 * @return  the index of the sample
 */
unsigned int HTableData::getSampleNumber(QString sampleName) {
    unsigned int i =0;
    QHash<QString, unsigned int> sampleNumbers;
    foreach(QString _sampleName, this->sampleNames) {
        sampleNumbers.insert(_sampleName, i);
        i++;
    }

    if( sampleNumbers.contains(sampleName) ) return sampleNumbers[sampleName];
    return 0;
}

/**
 * @brief HTableData::getSampleName, gets the name of the ith sample
 * @param i, sample index
 * @return QString representing the samplename
 */
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
    foreach(ATTRTYPE attr, this->allConnectors.keys())
        this->allConnectors[attr].clear();
}

void HTableData::addConnector(Connector *connector, ATTRTYPE attr) {
     this->connectors.append(connector);
     this->allConnectors[attr].append(connector);
}

void HTableData::setNumCols(unsigned int numCols) {
    this->numCols = numCols;
}

QStringList HTableData::getHeaders(){

    Labels *labels = Labels::getLabels();

    QStringList _headers = labels->getHtableHeader(this->id.attrType, this->getValueType());

    if(multiSampleMode) {
        _headers = this->multiSampleHeaders();
    }

    return _headers;
}

QString HTableData::getHeader(unsigned int i){

    Labels *labels = Labels::getLabels();

    QStringList _headers = labels->getHtableHeader(this->id.attrType, this->getValueType());

    if(multiSampleMode) {
        _headers = this->multiSampleHeaders();
    }

    if(i >= _headers.size()) return QString();
    return _headers.at(i);
}

enum TYPE HTableData::getFieldType(unsigned int i) {
    if( i >= this->types.size() )
        return UNKNOWN;
    return this->types.at(i);
}

/*
void HTableData::setHeaders(QStringList &headers) {
   ;// this->headers = headers;
}
*/
void HTableData::setTableIdentity(QString sampleName, ATTRTYPE attrType) {
   this->id.attrType = attrType;
   this->id.sampleName = sampleName;

}

/**
 * @brief HTableData::getattrType, get attr type
 * @return
 */
ATTRTYPE HTableData::getattrType() {
  return this->id.attrType;
}

/**
 * @brief HTableData::getScrollToFunction, for an ATTRTYPE it gets the function that was selected if any
 * @param attrType
 * @return
 */
QString HTableData::getScrollToFunction(ATTRTYPE attrType) {
  if( this->scrollToFunction.contains(attrType)) return this->scrollToFunction[attrType];
  return QString();
}
/**
 * @brief HTableData::setScrollToFunction, for an ATTRYPE it sets the function that was selected
 * @param attrType
 * @param function
 */
void HTableData::setScrollToFunction(ATTRTYPE attrType, QString function) {
  this->scrollToFunction[attrType] = function.trimmed();
}


void HTableData::spinBoxChanged(int depth) {
        this->showTableData();
}


void HTableData::showTableData() {
    this->fillData();
}


void HTableData::showHierarchyOrZeroRowToggleChanged(int i) {

    if( this->tableWidget->selectedItems().size() > 0) {
        this->setScrollToFunction(this->getattrType(), this->tableWidget->selectedItems()[0]->text());
    }

    if(this->subWindow) {
        this->showSelectedTableData();
    }
    else {
        this->showTableData();
    }
}


void HTableData::showHierarchyChanged(int state) {
    if(this->subWindow) {
        this->showSelectedTableData();
//    this->showTableData();
       // qDebug() << "Show seleced Table Data";
    }
    else {
       // qDebug() << "show Table Data";
        this->showTableData();
    }
}



/**
 * @brief HTableData::hideZeroRowsChanged, This function toggels to show and
 * display the zero rows of the functional tables
 * @param state, true/false settings to enable or disable based on the Checkbox state
 */
void HTableData::hideZeroRowsChanged(int state) {
    if(this->subWindow)
//        this->showSelectedTableData(this->category,  (this->hideZeroRows->checkState()==Qt::Checked));
       this->showTableData();

    else
        this->showTableData();
}


/** This function will get the data row to display in the functional table
 \param  state is to hide or unhide zero rows
 \param hideZeroRows   if this bool is true then the zero rows are not included
 \param showRPKM, bool to show/hide RPKM values

 */

void HTableData::fillData() {
     QList<ROWDATA *> data;

     if( ( this->htree->getAttrType()!=METACYC && this->htree->getAttrType()!=METACYCBASE)
           &&  (
                 this->getValueType()==BASEPWY_ORF ||
                 this->getValueType()== BASEPWY_RPKM ||
                 this->getValueType()==BASEPWY_RXNCOV ||
                 this->getValueType()==BASEPWY_RXNTOT
             )
     )
     {

         QWidget *parent = static_cast<QWidget*>(this);
         Utilities::showInfo(parent, QString("BasePwy ORF, BasePwy PRKM, BasePwy RxnCov and BasePwy RxnTot are applicable only for  MetaCyc Functional Category"));
         return;
     }

     if( this->htree->getAttrType()==METACYC || this->htree->getAttrType()==METACYCBASE) {
            DataManager *datamanager = DataManager::getDataManager();
            if (
                 this->getValueType()==BASEPWY_ORF ||
                 this->getValueType()==BASEPWY_RPKM ||
                 this->getValueType()==BASEPWY_RXNCOV ||
                 this->getValueType()==BASEPWY_RXNTOT
             ){
                 this->htree = datamanager->getHTree(METACYCBASE);
             }
             else {
                 this->htree = datamanager->getHTree(METACYC);
             }
     }

     data =  this->htree->getRows(this->showDepth->value(),  this->connectors, this->showHierarchy->isChecked(),  this->getValueType()) ;

     Labels *labels = Labels::getLabels();

     QStringList _headers = labels->getHtableHeader(this->id.attrType,this->getValueType());

     if(multiSampleMode) {
         _headers = this->multiSampleHeaders();
     }

#ifdef LCASTAR
     if( this->getValueType()==LCASTAR ) {
         this->populateTable(data, _headers);
     }
     else
#endif     
     {
       // hides the zeros either when the box is checked or if the attribute type is CAZY
       // if the category selector is toggled to CAZY then it is automatically check to hideZeroRows
       if( this->getattrType()==CAZY) {
           this->hideZeroRows->setChecked(true);
           this->hideZeroRows->setEnabled(false);
           this->hideZeroRows->setToolTip("Disabled for CAZY");
       }
       else {
           this->hideZeroRows->setEnabled(true);
           this->hideZeroRows->setToolTip("Disabled for CAZY");
       }

       if(hideZeroRows->isChecked() || this->getattrType()==CAZY) {
         foreach(ROWDATA *r, data) {
            r->active =  isNonZero(r) ? true : false;
         }
       }
       this->populateTable(data, _headers);
     }

}

/**
 * @brief HTableData::getValueType, gets the value type of the Htable, it is
 * read from the valueType combobox
 * @return VALUETYPE of the htable
 */
VALUETYPE HTableData::getValueType() {
    VALUETYPE _valueType;


    switch( this->valueType->currentIndex()) {
         case ORFCOUNT:
               _valueType = ORFCOUNT;
               break;
#ifdef LCASTAR
         case 1:
               _valueType = LCASTAR;
               break;
#endif
         case RPKMCOUNT:
               _valueType = RPKMCOUNT;
               break;
         case BASEPWY_ORF:
               _valueType = BASEPWY_ORF;
               break;
         case BASEPWY_RPKM:
              _valueType = BASEPWY_RPKM;
              break;
         case BASEPWY_RXNCOV:
               _valueType = BASEPWY_RXNCOV;
               break;
         case BASEPWY_RXNTOT:
               _valueType = BASEPWY_RXNTOT;
               break;
         default:
               _valueType = ORFCOUNT;
               break;
    }

    return _valueType;
}


/** this function shows data for a functional category
 *\param categoryName, the functional to show the count/rpkm values for
 *\param maxDepth, the maximum depth of the corresponding functional category
 *\param showHierarchy, a boolean to hide/show the hiearchy
 *\param hideZeroRows, a bool to hide/show the rows with zero values
 *\return the size of the data selected
 **/

unsigned int HTableData::fillSelectedData() {
     QList<ROWDATA *> data;

     data =  this->htree->getRows(this->showDepth->value(),  this->connectors, this->showHierarchy->isChecked(),  this->getValueType() ) ;

     Labels *labels = Labels::getLabels();
     QStringList _headers = labels->getHtableHeader(this->id.attrType, this->getValueType());


     if(multiSampleMode) {
         _headers = this->multiSampleHeaders();
     }

#ifdef LCASTAR
     if( this->getValueType()==LCASTAR ) {
         this->populateTable(data, _headers);
     }
     else
#endif
     {

         // hides the zeros either when the box is checked or if the attribute type is CAZY
         // if the category selector is toggled to CAZY then it is automatically check to hideZeroRows
       if( this->getattrType()==CAZY) {
             this->hideZeroRows->setChecked(true);
             this->hideZeroRows->setEnabled(false);
             this->hideZeroRows->setToolTip("Disabled for CAZY");
        }
        else {
             this->hideZeroRows->setEnabled(true);
             this->hideZeroRows->setToolTip("Disabled for CAZY");
       }
       if(hideZeroRows->isChecked() || this->getattrType()==CAZY) {
         foreach(ROWDATA *r, data) {
            r->active =  isNonZero(r) ? true : false;
         }
       }

       this->populateTable(data, _headers);
       return data.size();

     }
}

/**
 * @brief HTableData::multiSampleHeaders creates headers for the multisample mode by
 * adding the list of samples names
 * @return
 */
QStringList HTableData::multiSampleHeaders() {
     QStringList headers;

     Labels *labels = Labels::getLabels();

     headers.append(labels->getHtableHeader(this->id.attrType,this->getValueType())[0]);

     headers.append(labels->getHtableHeader(this->id.attrType,this->getValueType())[1]);


     foreach( QString sampleName, this->sampleNames)
         headers.append(sampleName);

     return headers;
}

unsigned int HTableData::showSelectedTableData() {
     return this->fillSelectedData();
}


/**
 * @brief HTableData::isNonZero, checks across a row if it is zero or now
 * @param r
 * @return
 */
bool HTableData::isNonZero(ROWDATA *r) {
    QVector<unsigned int>::const_iterator  it;
    for(it = r->counts.begin(); it!=r->counts.end(); it++) {
        if(*it > 0) return true;
    }
    return false;
}


/** This function populates the QTableWidget wit the selected data
 *\param data is the rows of data to be displayed
 *\param headers headers for the table
 *\param hierachyEnabled, a boolean used to hide/show the hierarchy of the functional group
 **/
void HTableData::populateTable( QList<ROWDATA *> &data, const QStringList &headers){

    tableWidget->clear();
    tableWidget->setColumnCount(headers.size());

    tableWidget->setHorizontalHeaderLabels(headers);

    unsigned int size =0;
    foreach( ROWDATA * datum,  data)  if(datum->active) size++;
    tableWidget->setRowCount(size);
    QString scrollToName = this->getScrollToFunction(this->getattrType());
    int scrollToRow = 0;


    tableWidget->setColumnCount(types.size());
    tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    if( this->showHierarchy->isChecked() )
       tableWidget->setSortingEnabled(false);
    else
       tableWidget->setSortingEnabled(true);


    QTableWidgetItem *item;
    int k = 0;

#ifdef LCASTAR
    if(this->getValueType()==LCASTAR) {
        this->computeLCAStar(data);
    }
#endif

    foreach( ROWDATA * datum,  data) {

        if( !datum->active) continue; // hide zero
        if(datum->name== scrollToName) scrollToRow = k;



        if(this->showHierarchy->isChecked())
            item = new QTableWidgetItem(this->indents[datum->depth] + QString(datum->name));
        else
            item = new QTableWidgetItem( QString(datum->name));

        item->setForeground(this->colors[datum->depth]);
        tableWidget->setItem(k,0,item);

        tableWidget->setItem(k,1,new QTableWidgetItem(QString(datum->alias)));

#ifdef LCASTAR
        LCAStar *lcastar = LCAStar::getLCAStar();
        QList<FREQUENCY> freq;
        VALUETYPE _valueType = this->getValueType();
        if(_valueType==LCASTAR) {
            for(unsigned int j=0; j <  datum->taxons.size(); j++) {
               freq.clear();
               item =  new QTableWidgetItem(datum->taxons[j]);
               FREQUENCY __lca_star__;
               __lca_star__.name = datum->taxons[j];
               __lca_star__.count = datum->counts[j];
               __lca_star__.total = datum->lcaresults[j].total;
               __lca_star__.valid = datum->lcaresults[j].valid;

               unsigned int totalorfs;
               unsigned int maxCount = this->getTaxDistribution(datum->name, j, freq, totalorfs);
               __lca_star__.percent = (float)__lca_star__.count*100/((float)totalorfs);

               if( maxCount > 0) {
                   tooltip = lcastar->getTaxonsDistribtionTooltipTxt(freq, maxCount, __lca_star__);
                  item->setToolTip(tooltip);
               }

               tableWidget->setItem(k,2 +j, item);
            }
        }
        else
#endif
          {
            item->setToolTip("");
            for(unsigned int j=0; j <  datum->counts.size(); j++) {
                item = new QTableWidgetItem(QString::number(datum->counts[j]));
                item->setToolTip("");
                tableWidget->setItem(k,2 +j, item);
            }

        }
      k++;
    }
    tableWidget->resizeColumnsToContents();
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->scrollToItem( tableWidget->item(scrollToRow,0), QAbstractItemView::PositionAtCenter);
    tableWidget->setCurrentCell(scrollToRow,0);
}


/**
 * @brief HTableData::computeLCAStar, computes the LCAStar values for the each of the rows
 * @param data, where the rows for the table are stored
 */
void HTableData::computeLCAStar(QList<ROWDATA *> &data) {

    ProgressView progressbar("Recomputing LCA* values", 0, 0, 0);
  //  LCAStar *lcastar = LCAStar::getLCAStar();

    unsigned int value =55;
    unsigned int depth = 1;
    try {
    bool ok;
        alpha->currentText().toUInt(&ok);
        if(ok)
           value = alpha->currentText().toUInt(&ok);

        depth = this->lcaDepth->value();
    }
    catch(...) {
       value = 55;
       depth = 1;
    }




#define THREAD_LCASTAR
#ifdef THREAD_LCASTAR

    LCA_THREAD_DATA lca_thread_data;
    lca_thread_data.data = &data;
    lca_thread_data.connectors = &this->connectors;
    lca_thread_data.attrType =this->id.attrType;

    lca_thread_data.numThreads =  static_cast<unsigned int>( 0.8* static_cast<double>(QThread::idealThreadCount()));
    if( lca_thread_data.numThreads < 1 ) lca_thread_data.numThreads = 1;

    LCAStar * lcastars =  new LCAStar[lca_thread_data.numThreads];


    for(unsigned int i =0 ; i < lca_thread_data.numThreads; i++) {
       lcastars[i].setParameters(1, depth, static_cast<double>(value)/100.0);
       lca_thread_data.threadid = i;
       lcastars[i].setData(lca_thread_data);
       lcastars[i].start();
    }
    for(unsigned int i =0 ; i < lca_thread_data.numThreads; i++) {
        lcastars[i].wait();

    }


    delete []lcastars;

   // lcastar->lca_star(data,this->connectors, this->id.attrType )    ;

#else
    QStringList taxonList;
    foreach(ROWDATA *datum, data) {
        for(unsigned int i =0; i < datum->taxons.size(); i++ ) {
            taxonList.clear();
            this->getTaxonList(datum, i, taxonList);
            datum->taxons[i] = lcastar->lca_star(taxonList);
        }
    }
#endif

    progressbar.hide();

}


/**
 * @brief HTableData::showInformativeTable this creates a new table from the item, that what selected
 * @param item, the item that was clicked.
 */
void HTableData::spawnInformativeTable(const QString &sampleName, const  QList<ORF *> &orfList) {


    DataManager *datamanager = DataManager::getDataManager();

    HTableData *htable = new HTableData(0, 1, false, false);
    htable->show();
    // this showdepth value is passed now before a valueChanged signal is connected to the spinbox slot

    //HTableData *htable = new HTableData(0, this->showDepth->value()+ 1 , this->showHierarchy->isChecked(), this->hideZeroRows->isChecked(), this->getValueType());
    htable->setAttribute(Qt::WA_DeleteOnClose); // frees up memory once it's closed
    htable->setMultiSampleMode(false);
    htable->sampleNames.append(sampleName);

    htable->clearConnectors();

    htable->categorySelector->setVisible(true);
    htable->categorySelector->setCurrentIndex(KEGG);
    htable->subWindow = true;
    htable->level = 1;
    htable->types << STRING << STRING << INT;
    htable->id.attrType = KEGG;
    htable->id.sampleName = sampleName;

    QString title = "Selected ORFs";

    htable->setWindowTitle(title );

    HTree *htree = datamanager->getHTree(KEGG);
    HNODE *hnode = htree->getRootHNODE();

    htable->subCategoryName->setVisible(true);

    if(hnode != 0)
       htable->subCategoryName->setText(hnode->attribute->alias);
    else
       htable->subCategoryName->setText(htable->category);

    htable->depthLabelValue->setText(QString::number(htable->level));
    htable->depthLabelValue->setVisible(true);
    //htable->hideZeroRows->setChecked(hideZeroRows->isChecked());
   // htable->hideZeroRows->setEnabled(this->hideZeroRows->isEnabled());


    htable->clearConnectors();

    QList<ATTRTYPE> attrs;
    attrs << KEGG << COG <<  METACYC << SEED;

    QList<ORF *> *_orfList = new QList<ORF *>;
    foreach(ORF *orf, orfList)
      _orfList->append(orf);


    foreach( ATTRTYPE attr, attrs) {
       Connector *connector = datamanager->_createConnector(htable->sampleNames[0], datamanager->getHTree(attr), attr, _orfList);
       htable->allConnectors[attr].append(connector);
    }


   // qDebug() << "Category depth " << this->id.attrType << "  " << datamanager->getHTree(this->id.attrType)->getTreeDepth();
    htable->numOrfsLabel->setText(QString("ORFs count : " +  QString::number(orfList.size()) ));
    htable->setToolTip("Total number of ORFs | Number of distinct ORFs");
    htable->numOrfsLabel->setVisible(true);

    // this also starts loading the data
    htable->switchCategory(htable->id.attrType);

    /*
    ResultWindow *resultwindow = ResultWindow::getResultWindow();

    if( !resultwindow->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), resultwindow, SLOT( showTable(QString, ATTRTYPE)  ));
       resultwindow->htablesAddSignals.insert(htable, true);
    }*/


}




/**
 * @brief HTableData::showInformativeTable this creates a new table from the item, that what selected
 * @param item, the item that was clicked.
 */
void HTableData::showInformativeTable(QTableWidgetItem *item) {
    unsigned int row = item->row();

    DataManager *datamanager = DataManager::getDataManager();
    // this showdepth value is passed now before a valueChanged signal is connected to the spinbox slot
    HTableData *htable = new HTableData(0, this->showDepth->value()+ 1 , this->showHierarchy->isChecked(), this->hideZeroRows->isChecked(), this->getValueType());
    htable->setAttribute(Qt::WA_DeleteOnClose); // frees up memory once it's closed
    htable->setMultiSampleMode(this->isMultiSampleMode());
    htable->setSampleNames(this->sampleNames);

 //   htable->setMaxSpinBoxDepth(datamanager->getHTree(this->id.attrType)->getTreeDepth());
    htable->clearConnectors();

    htable->categorySelector->setVisible(true);
    htable->categorySelector->setCurrentIndex(this->id.attrType);
    htable->subWindow = true;
    htable->category = tableWidget->item(row,0)->text().trimmed();
    htable->level = this->level +1;
    htable->types = this->types;
    htable->windowtitles = this->windowtitles;
    htable->scrollToFunction = this->scrollToFunction;
    htable->setScrollToFunction(this->getattrType(), this->tableWidget->item(row,0)->text());
    htable->hideZeroRows->setChecked(this->hideZeroRows->isChecked());
    htable->hideZeroRows->setEnabled(this->hideZeroRows->isEnabled());

    QString title;
    foreach( QString _title, htable->windowtitles) {
        title += _title + " | ";
    }
    title += htable->category;
    htable->windowtitles.append(htable->category);

    htable->setWindowTitle(title );
    HTree *htree = datamanager->getHTree(this->id.attrType);
    HNODE *hnode = htree->getHNODE(htable->category);

    htable->subCategoryName->setVisible(true);
    if(hnode != 0)
       htable->subCategoryName->setText(hnode->attribute->alias);
    else
       htable->subCategoryName->setText(htable->category);

    htable->depthLabelValue->setText(QString::number(htable->level));
    htable->depthLabelValue->setVisible(true);

    htable->connectors.clear();
    Connector *modConnector ;


    foreach(Connector *connector, this->connectors) {
        modConnector = datamanager->createSubConnector(datamanager->getHTree(this->id.attrType), hnode, connector, this->id.attrType);
        htable->allConnectors[this->id.attrType].append(modConnector);
      //  qDebug() << " submodCon size " << modConnector->getNumOfORFs() << " atr sub" << this->id.attrType ;
    }

    QList<ORF *>orfList;
    unsigned int totNumOrfs =0 , _totNumOrfs =0;

    for(unsigned int i =0; i < this->htableIdentities.size(); i++) {

        totNumOrfs = 0;
        _totNumOrfs = 0;
        foreach(Connector *connector, this->connectors) {
           orfList = connector->getORFList(htree->getLeafAttributesOf(hnode));
          // qDebug() << "Number of orfs transferring " << orfList.size();
           modConnector = datamanager->createConnector("temp", datamanager->getHTree(this->htableIdentities[i].attrType), this->htableIdentities[i].attrType, &orfList);
           _totNumOrfs += Utilities::uniqueORFsCount(orfList);
           totNumOrfs += orfList.size();
           htable->allConnectors[this->htableIdentities[i].attrType].append(modConnector);
        }
    }

   // qDebug() << "Category depth " << this->id.attrType << "  " << datamanager->getHTree(this->id.attrType)->getTreeDepth();
    htable->numOrfsLabel->setText(QString("ORFs count : " + QString::number( totNumOrfs) + " | " + QString::number(_totNumOrfs) ));
    htable->setToolTip("Total number of ORFs | Number of distinct ORFs");
    htable->numOrfsLabel->setVisible(true);
    htable->setTableIdentity(this->id.sampleName, this->id.attrType);

    // this also starts loading the data
    htable->switchCategory(this->id.attrType);

    ResultWindow *resultwindow = ResultWindow::getResultWindow();

    if( !resultwindow->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), resultwindow, SLOT( showTable(QString, ATTRTYPE)  ));
       resultwindow->htablesAddSignals.insert(htable, true);
    }

    /** kishori's changes
    HTabWidget *htab = new HTabWidget(htable->category,  ":images/cross.png");
    ToolBarManager *toolbarManager = ToolBarManager::getToolBarManager();
    toolbarManager->addTab(htab,  htable);
    WidgetStacker *wStacker = WidgetStacker::getWidgetStacker();
    wStacker->stackWidget(htable);
    mdiAreaWidget->getMdiArea()->cascadeSubWindows(); */
}

/** this function is activated when the user switched the functional
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
    //this->setShowHierarchy(true);


    if(this->id.attrType==this->htableIdentities[index].attrType) {
        if( this->showSelectedTableData() ) {
            this->show();
        }
        else {
            this->setTableIdentity(this->id.sampleName, this->htableIdentities[index].attrType);
            this->hide();
        }
    }
    else {
        this->setTableIdentity(this->id.sampleName, this->htableIdentities[index].attrType);
        this->showTableData();
    }


}

/**
 * @brief HTableData::saveTableToFile saves the table to the file
 * @param fileName, to save to
 * @param delim, the delimiter to use
 * @param selectedHeaders, the columns to export
 * @return
 */
bool HTableData::saveTableToFile(QString fileName, QChar delim, const QStringList &selectedHeaders) {
    QFile outFile(fileName);

    Labels *labels = Labels::getLabels();

    QStringList _headers = labels->getHtableHeader(this->id.attrType, this->getValueType());


    if(multiSampleMode) {
        _headers = this->multiSampleHeaders();
    }

    if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outFile);
        bool delimOn;

        QHash<QString, bool> includeColumn;
        foreach(QString header, _headers) {
            includeColumn[header] = false;
        }

        foreach(QString header, selectedHeaders) {
            includeColumn[header]= true;
        }

        int j = 0;
        delimOn = false;
        foreach(QString header, _headers) {
            if( !includeColumn[header])
                continue;
            else {
                if(delimOn)  out << delim;
                delimOn = true;
            }
            out << header;
        }
        out << "\n";


        for(int i =0; i < this->tableWidget->rowCount(); i++) {
            if( this->tableWidget->isRowHidden(i) ) continue;
             delimOn = false;
             for(int j =0; j < this->tableWidget->columnCount(); j++)  {
                 if( !includeColumn[_headers[j]])
                     continue;
                 else {
                      if(delimOn)  out << delim;
                      delimOn = true;
                      out << Utilities::prepareToPrint(this->tableWidget->item(i,j)->text(), delim);
                 }
             }
             out << "\n";
        }
        outFile.close();
    }
    return true;
}



/** Saves the sequeces to a local file
 * \param sampleName : name of the sample
 * \param fileName : name of the file/folder to save it in, creates a new folder
 * \param type: type of sequences to export to the file
 **/
bool HTableData::saveSequencesToFile(QString sampleName, QString fileName,  RESOURCE type) {
    QFile outFile(fileName);
    DataManager *datamanager = DataManager::getDataManager();
    HTree *htree = datamanager->getHTree(this->id.attrType);

    // take any connector, since they all have the same set of ORFs

    Connector *connector = this->allConnectors[this->id.attrType][this->getSampleNumber(sampleName)];

    if( datamanager==0 || htree ==0 || connector == 0 ) return false;
    QList<ORF *>orfList = connector->getORFList();

  //  connector->getORFList(htree->getLeafAttributesOf(hnode));
    QHash<QString, bool> keyNames;
    ProgressView progressbar("Saving sequences for sample : " + sampleName, 0, 0, this);
 //   qDebug() << "total orfs " << connector->getORFList().size();
   // foreach( ORF *o, connector->getORFList())
     //   qDebug() << o->name;


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

/** Saves the tabular BLAST/LAST results to to a local file for MEGAN export
 * \param sampleName : name of the sample
 * \param fileName : name of the file/folder to save it in, creates a new folder
 * \param type: type of sequences to export to the file, should be MEGANLASTFILE or MEGANBLASTFILE
 **/
bool HTableData::saveMeganExportToFile(QString sampleName, QString fileName,  RESOURCE type) {
    DataManager *datamanager = DataManager::getDataManager();
    HTree *htree = datamanager->getHTree(this->id.attrType);

    // take any connector, since they all have the same set of ORFs

    Connector *connector = this->allConnectors[this->id.attrType][this->getSampleNumber(sampleName)];

    if( datamanager==0 || htree ==0 || connector == 0 ) return false;
    QList<ORF *>orfList = connector->getORFList();

  //  connector->getORFList(htree->getLeafAttributesOf(hnode));
    QHash<QString, bool> orfNames;
    ProgressView progressbar("Export data to MEGAN for sample : " + sampleName, 0, 0, this);

    foreach(ORF *orf, orfList) {
         orfNames[orf->name] = true;
    }
   // }

    SampleResourceManager *sampleResourceManager = SampleResourceManager::getSampleResourceManager();

    QFile inputTable;

    inputTable.setFileName(sampleResourceManager->getFilePath(sampleName, type));

    if( inputTable.exists())
         Utilities::exportMeganCompatibleFile(inputTable.fileName(), fileName, orfNames);
    else {
        QMessageBox::warning(this, QString("Cannot find input file ") + fileName + QString(" to input from"), QString("Missing expected file ") + fileName);
    }
    progressbar.hide();
    return true;
}




/**
 * @brief TableData::clearSearchFilters, removes the search filters added so far
 */
void HTableData::clearSearchFilters() {
    this->searchFilters.clear();
}


/**
 * @brief TableData::initializeSearchFilter adds a search filter
 * @param query, the query
 * @param column, column chosen at the export box
 * @param caseSensitive
 */
void HTableData::addSearchFilter(QString query, int column) {

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
 * @brief HTableData::searchQuery, a slot that searches the table based on the for query and query columns selected
 * in the search box
 * @param query1
 * @param column1
 * @param query2
 * @param column2
 * @param query3
 * @param column3
 * @param query4
 * @param column4
 * @param type,  OR or AND
 * @param caseSensitive,  case sensitivity
 */

void HTableData::searchQuery(QString query1, int column1, QString query2, int column2, QString query3, int column3, QString query4, int column4, OPTYPE type,  bool caseSensitive){
    this->clearSearchFilters();
    if(query1.size() > 0) this->addSearchFilter(query1, column1);
    if(query2.size() > 0) this->addSearchFilter(query2, column2);
    if(query3.size() > 0) this->addSearchFilter(query3, column3);
    if(query4.size() > 0) this->addSearchFilter(query4, column4);
    this->makeSearch(type, caseSensitive);
}


/**
 * @brief HTableData::makeSearch, applies the multiple search options to the data
 * @param optype, it is of AND or OR type
 * @param caseSensitive, it is casesensitive one true
 */
void HTableData::makeSearch(OPTYPE optype,  bool caseSensitive) {

    enum DECISION hit;

    Qt::CaseSensitivity _caseSensitive =  caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    if(this->searchFilters.size()==0) {
       for (unsigned int i = 0;  i < tableWidget->rowCount(); i++){
            this->tableWidget->setRowHidden(i, false);
       }
       return;
    }

    //consider a row i in the table widget data
    for (unsigned int i = 0;  i < tableWidget->rowCount(); i++){
        //one search at a time
        hit = NO;
        foreach(SEARCH searchFilter, searchFilters) {
            //empty searchquery
            if( searchFilter.searchItem.size()==0 ) continue;

            //one column j in row i
            hit=NO;
            foreach(unsigned int j, searchFilter.searchCols) {
               if( this->tableWidget->item(i, j)->text().contains(searchFilter.searchItem, _caseSensitive)) {  hit= YES; break; }
            }
            if(hit==YES && optype == OR) break;
            if(hit==NO && optype == AND ) break;
        }

        if(hit == NO ) {
           this->tableWidget->setRowHidden(i, true);
        }
        else{
           this->tableWidget->setRowHidden(i, false);
        }
    }

}

/**
 * @brief HTableData::ProvideContexMenu, this function emits a showtable signal to the
 * results windwo with eventually triggers off the functional and taxonomic table
 * @param pos
 */

void HTableData::ProvideContexMenu(QPoint pos)
{
        unsigned int col = this->tableWidget->itemAt(pos)->column();
        unsigned int row = this->tableWidget->itemAt(pos)->row();

        if( col < 2 || col >= this->sampleNames.size() +  2) return;

        QString category = this->tableWidget->item(row, 0)->text();


        DataManager *datamanager = DataManager::getDataManager();
        HTree *htree = datamanager->getHTree(this->id.attrType);
        HNODE *hnode = htree->getHNODE(category);

        GlobalDataTransit *dataTransit = GlobalDataTransit::getGlobalDataTransit();


        if( htree==0 || hnode ==0 ) return;

        Connector *connector =   this->connectors[col - 2];
        dataTransit->orfList = connector->getORFList(htree->getLeafAttributesOf(hnode));


        emit showTable( this->sampleNames[col-2], this->id.attrType);

    //context->exec(*position);
}

void HTableData::getTaxonList(ROWDATA *datum, unsigned int sampleNum, QStringList &taxonList) {

    QString category = datum->name;



    DataManager *datamanager = DataManager::getDataManager();
    HTree *htree = datamanager->getHTree(this->id.attrType);
    HNODE *hnode = htree->getHNODE(category);
    if( htree==0 || hnode ==0 ) return;

    Connector *connector =   this->connectors[sampleNum];

    QList<ORF *> orfList = connector->getORFList(htree->getLeafAttributesOf(hnode));

    foreach(ORF *orf, orfList) {
      if( orf->attributes.contains(TAXON)  ) taxonList.append( orf->attributes[TAXON]->name);
    }
    return;

}


/**
 * @brief HTableData::LCAStarValue, computes the LCA star value for the given category for
 * a sample
 * @param category, the functional category
 * @param sampleNum, index of the sample
 * @return the LCAStar taxonomy
 */
QString HTableData::LCAStarValue(const QString &category, unsigned int sampleNum)
{
        DataManager *datamanager = DataManager::getDataManager();
        HTree *htree = datamanager->getHTree(this->id.attrType);
        HNODE *hnode = htree->getHNODE(category);
        if( htree==0 || hnode ==0 ) return  QString("-");

        Connector *connector =   this->connectors[sampleNum];

        QList<ORF *> orfList = connector->getORFList(htree->getLeafAttributesOf(hnode));

        LCAStar *lcastr = LCAStar::getLCAStar();

        QString lcavalue = QString("-");
        QStringList taxonList;



        if( orfList.size() > 0 ) {
            foreach(ORF *orf, orfList) {
               if( orf->attributes.contains(TAXON)  ) taxonList.append( orf->attributes[TAXON]->name);
            }
            lcavalue  =  lcastr->lca_star(taxonList).taxon;
        }
        return lcavalue;

    //context->exec(*position);
}

/**
 * @brief HTableData::getTaxDistribution, computes the distribution of each taxa
 * @param category
 * @param sampleNum
 * @param freq
 * @return
 */
unsigned int HTableData::getTaxDistribution(const QString &category, unsigned int sampleNum,  QList<FREQUENCY> &freq, unsigned int &totalorfs) {

    DataManager *datamanager = DataManager::getDataManager();
    HTree *htree = datamanager->getHTree(this->id.attrType);
    HNODE *hnode = htree->getHNODE(category);
    if( htree==0 || hnode ==0 ) return 0;

    Connector *connector =   this->connectors[sampleNum];

    QList<ORF *> orfList = connector->getORFList(htree->getLeafAttributesOf(hnode));

    QHash<QString, unsigned int>  hashCount;
    foreach(ORF *orf, orfList) {
       if( orf->attributes.contains(TAXON)  ) {
           if( !hashCount.contains( orf->attributes[TAXON]->name ) ) hashCount[orf->attributes[TAXON]->name] =0;
           hashCount[orf->attributes[TAXON]->name]++;
       }
    }

    QList<TaxonFreqQPair > taxonFreqList;
    double totalCount =0;
    foreach(QString key, hashCount.keys()) {
        taxonFreqList.append(TaxonFreqQPair(key, hashCount[key]) );
        totalCount += static_cast<double>(hashCount[key]);
    }
    if( totalCount ==0) totalCount = 1;
    totalorfs = totalCount;

    qSort(taxonFreqList.begin(), taxonFreqList.end(), Utilities::compareFreq);

    unsigned int maxCount =0;
    for(QList<TaxonFreqQPair>::iterator it = taxonFreqList.begin(); it!= taxonFreqList.end(); ++it) {
        FREQUENCY item;
        item.name = it->first;
        item.count = it->second;
        item.percent = (it->second/totalCount)*100;
        freq.append(item);
        if( item.count > maxCount) maxCount = item.count;
    }

    return maxCount;

}


