#include "resultwindow.h"

ResultWindow *ResultWindow::resultWindow=0;

ResultWindow *ResultWindow::getResultWindow() {
    if( ResultWindow::resultWindow == 0 ) {
        ResultWindow::resultWindow = new ResultWindow();
    }

    return ResultWindow::resultWindow;


}

ResultWindow::ResultWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MetaPathways - Results and Data");


    this->rundata = RunData::getRunData();

    resultTabs = this->findChild<QTabWidget *>("resultTabs");
    sampleSelect = this->findChild<QComboBox *>("sampleSelect");
    checkComparativeMode = this->findChild<QCheckBox *>("checkComparativeMode");
    selectSamplesButton = this->findChild<QPushButton *>("selectSamplesButton");
    currentSampleLabel = this->findChild<QLabel *>("currentSampleLabel");
    reindex = this->findChild<QCheckBox *>("reindex");
    loadResults = this->findChild<QPushButton *>("loadResults");

    selectSamplesButton->setEnabled(false);


    resultTabs->setUsesScrollButtons ( true );
    resultTabs->clear();
    resultTabs->setUsesScrollButtons(true);
    getFileNames = new QTimer();
    getFileNames->start(500);

    connect(sampleSelect,SIGNAL(activated(QString)),this,SLOT(sampleChanged(QString)));
   // connect(getFileNames, SIGNAL(timeout()),this,SLOT(updateFileNames()));
    //connect(this,SIGNAL(fileChanged(QString)),this->progress,SLOT(selectedFileChanged(QString)));
    connect(checkComparativeMode, SIGNAL(stateChanged(int)), this, SLOT(setVisible(int)) );
    connect(selectSamplesButton, SIGNAL(clicked()), this, SLOT(clickedSelectSample()  ) );
    connect(loadResults, SIGNAL(clicked()), this, SLOT(_loadResults()) );

//    tables["PATHWAYS"] = new TableData();
//    tables["REACTIONS"] = new TableData();
   // tables["FUNC & TAX"] = new TableData();
   // tables["FUNC & TAX"]->useLCAStar(true);

    tables["rRNA"] = new TableData();

    displayInfos["FUNC & TAX"] = new DisplayInfo();


    graphs["CONT LEN HIST"] = new GraphData();
    graphs["ORF LEN HIST"] = new GraphData();

#ifdef MEGAN_VIEW
    meganviews["MEGAN_TAXONOMIC"]= new MeganView();
    DataManager *datamanager = DataManager::getDataManager();
    datamanager->createDataModel();
#endif

}

/**
 * @brief ResultWindow::_loadResults, loads the samplewise results from the output folder
 */
void ResultWindow::_loadResults() {
    if(  this->rundata->getParams()["folderOutput"].size()==0) {
        QMessageBox::warning(this, "Output Folder Missing", "Output folder not set in the <b>Stages</b> tab", QMessageBox::Ok);
        return;
    }


    QFileInfo output(this->rundata->getParams()["folderOutput"]);

    if( !output.exists()) {
        QMessageBox::warning(this, "Output Folder does not exist", "Output folder specified in Stages tab does not exist!", QMessageBox::Ok);
        return;
    }

    this->disableSampleChanged = true;

    DataManager *datamanager = DataManager::getDataManager();
    /***/
    datamanager->destroyAllORFs();
    datamanager->destroyAllContigs();
    datamanager->deleteAllConnectors();
    datamanager->destroyAllHTrees();
    datamanager->destroyAllAttributes();

    TableManager *tableManager = TableManager::getTableManager();
    tableManager->deleteAllGraphData();
    tableManager->deleteAllTables();
    /***/
    datamanager->setDataModelCreated(false);

    this->rundata->loadOutputFolders();
    this->updateFileNames();
    this->rundata->updateCurrentFileList();

    this->disableSampleChanged = false;

    //  datamanager->setORFsUptoDateAll(false);
    if (this->sampleSelect->count() > 0) {
        this->sampleSelect->setCurrentIndex(0);
    }
}

/**
 * @brief ResultWindow::indexSamples, index the nucleotide, amino acid and orfs fasta files
 * for rapid access
 * @param userResourceFolder, folder to put the index file
 * @param reindex, reindexs the files even if it is already indexed
 */
void ResultWindow::indexSamples(bool userResourceFolder, bool reindex) {
    ProgressView progressBar("Indexing samples...", 0, files.size(), this);

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
    samplercmgr->setUseResourceFolder(userResourceFolder);

    unsigned int i =0;
    foreach(QString file, this->rundata->getFileList(this->rundata->getCurrentInputFormat()) ) {
        samplercmgr->createFileIndex(file, NUCFASTA);
        samplercmgr->createFileIndex(file, AMINOFAA);
        samplercmgr->createFileIndex(file, NUCFNA);
        progressBar.updateprogress(++i); qApp->processEvents();  progressBar.update();
    }

    progressBar.hide();
}

/**
 * @brief ResultWindow::indexSample, creates the indexes for the sequences
 * @param sampleName
 * @param userResourceFolder
 */
void ResultWindow::indexSample(QString sampleName, bool userResourceFolder) {
    ProgressView progressBar(QString("Indexing sample...") + sampleName, 0, 1, this);

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
    samplercmgr->setUseResourceFolder(userResourceFolder);

    unsigned int i =0;
    foreach(QString file, this->rundata->getFileList( this->rundata->getCurrentInputFormat() ) ) {
        samplercmgr->createFileIndex(file, NUCFASTA);
        samplercmgr->createFileIndex(file, AMINOFAA);
        samplercmgr->createFileIndex(file, NUCFNA);
        progressBar.updateprogress(++i); qApp->processEvents();  progressBar.update();
    }

    progressBar.hide();
}

/**
 * @brief ResultWindow::clickedSelectSample, loads the output results for the
 * comparative mode
 */
void ResultWindow::clickedSelectSample(){
    this->selectWindow = new SelectSamples;
    this->selectWindow->setReceiver(this);
    this->selectWindow->addSamples(this->rundata->getOutputFolders());
    this->selectWindow->show();
}


/**
 * @brief ResultWindow::setVisible, sets the resulttab to visible
 * @param i
 */
void ResultWindow::setVisible(int i) {
    if( i ) {
        this->selectSamplesButton->setEnabled(true);
        sampleSelect->setEnabled(false);
    }
    else {
        this->selectSamplesButton->setEnabled(false);
        sampleSelect->setEnabled(true);
    }
}

/**
 * @brief ResultWindow::updateFileNames, updates the filenames
 */
void ResultWindow::updateFileNames(){

    files = this->rundata->getOutputFolders();

    if( files.isEmpty()) return;

    sampleSelect->clear();
    QStringList existing;

    for (int i=0;i<sampleSelect->count();i++){
        existing.append(sampleSelect->itemText(i));
    }

    sampleSelect->addItem(SELECT_SAMPLE_TAG);
    foreach (QString f, files){
        if (!existing.contains(f)){
            sampleSelect->addItem(f);
            sampleSelect->setItemData(sampleSelect->count()-1, f, Qt::ToolTipRole);
        }
    }
    rundata->setCurrentSample(files[0]);

}


/**
  * @brief ResultWindow::receiveSelection, receives the set of selection to load the samples
  * @param selection
  */

 void ResultWindow::receiveSelection(QList<unsigned int> &selection) {
     this->selectedSamples = selection;
     this->switchToComparativeMode();
 }

 /**
 * @brief ResultWindow::sampleChanged, this function loads a new sample and
 * is triggered through a sample change on the combobox
 * @param sampleName
 */
void ResultWindow::sampleChanged(QString sampleName){
    QRegExp select_sample_regexp = QRegExp("Select sample");
    if( select_sample_regexp.indexIn(sampleName) != -1 ) return;

    this->rundata->setCurrentSample(sampleName);

    QString OUTPUTPATH = this->rundata->getParams()["folderOutput"];

    DataManager *datamanager = DataManager::getDataManager();
    datamanager->createDataModel();

    SampleResourceManager *sampleResourceManager = SampleResourceManager::getSampleResourceManager();
    sampleResourceManager->setOutPutPath(OUTPUTPATH);

    if( reindex->isChecked() ) {
        this->indexSamples(true, true);
        reindex->setChecked(false);
    }

    ProgressView progressbar("Please wait! Loading sample " + sampleName + "...", 0, 0, this);
    QString pgdbname = sampleName.toLower().replace(QRegExp("[.]"), "_");
    pgdbname = pgdbname.at(0).isLetter() ? pgdbname : QString("e") + pgdbname;


    resultTabs->setStyleSheet( "QTabBar::tab { min-width: 20ex;}");
    resultTabs->clear();
    for (unsigned int i=0; i< resultTabs->count(); i++){
        delete resultTabs->widget(i);
    }

    QList<enum TYPE> types;
    QList<unsigned int> columns;

    QStringList headers;

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
    datamanager->createORFs(sampleName );
    datamanager->addNewAnnotationToORFs(sampleName, samplercmgr->getFilePath(sampleName, ORFMETACYC));
    datamanager->addRPKMToORFs(sampleName, samplercmgr->getFilePath(sampleName, ORFRPKM) );

    QString functionalTable = samplercmgr->getFilePath(sampleName, FUNCTIONALTABLE);
    datamanager->addTaxons(functionalTable);
    datamanager->addTaxonToORFs( sampleName, functionalTable);


    QStringList selectedFileNames;
    selectedFileNames.append(sampleName);

    RunDataStats *rundatamodel =  CreateWidgets::getRunDataStats();
    rundatamodel->clear();
    rundatamodel->setFileNames(selectedFileNames);
    rundatamodel->readStatFiles();

    QTableView *statTableView = CreateWidgets::getStatsTableView();
    statTableView->setModel(rundatamodel);
    statTableView->setAlternatingRowColors(true);
    statTableView->show();
    resultTabs->addTab(statTableView, "RUN STATS");
    resultTabs->setTabToolTip(resultTabs->count()-1, "RUN STATS");


    QString contigLengthsFile = samplercmgr->getFilePath(sampleName, CONTIGLENGTH);
    QFile file(contigLengthsFile);

    GraphData *g;


    if( file.exists() ) {
        if( this->simpleGraphGroups.tableExists(sampleName, contigLengthsFile)) {
            g = this->simpleGraphGroups.getTable(sampleName, contigLengthsFile);
        }
        else { // table has to be created a new
            g = new GraphData;
            g->setFile(contigLengthsFile);
            g->prepareInput("Contig Length (nucleotides)", "Frequencey");
            this->simpleGraphGroups.addTable(g, sampleName, contigLengthsFile);
        }
        resultTabs->addTab(g,"CONT LEN HIST");
        resultTabs->setTabToolTip(resultTabs->count()-1, "Contig Length Distributions");
    }


    QString orfsLengthsFile = samplercmgr->getFilePath(sampleName, ORFLENGTH);
    file.setFileName(orfsLengthsFile);
    if( file.exists() ) {
        if( this->simpleGraphGroups.tableExists(sampleName, orfsLengthsFile)) {
            g = this->simpleGraphGroups.getTable(sampleName, orfsLengthsFile);
        }
        else { // table has to be created a new
            g = new GraphData;
            g->setFile(orfsLengthsFile);
            g->prepareInput("ORF Length (amino)", "Frequencey");
            this->simpleGraphGroups.addTable(g, sampleName, orfsLengthsFile);
        }

      //  g->show();

        resultTabs->addTab(g,"ORF LEN HIST");
        resultTabs->setTabToolTip(resultTabs->count()-1, "ORF Length (amino) Distribution");
    }


 #ifdef SECTION
     statTableView->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
     statTableView->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
 #else
     statTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
     statTableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
     statTableView->verticalHeader()->setResizeMode(QHeaderView::Stretch);
 #endif

    //////////////  FUNCTIONAL CATEGORIES KEGG, COG, METACYC, SEED, CAZY //////////////////
    HTableData *htable;
    Labels *labels = Labels::getLabels();


    QList<ATTRTYPE> functionTables;
    functionTables << KEGG << COG << METACYC << SEED << CAZY;

    foreach(ATTRTYPE functionAttr, functionTables) {
        types.clear();
        types << STRING << STRING << INT;
        headers = labels->getHtableHeader(functionAttr, ORFCOUNT);
        htable = this->getHTableData(sampleName, functionAttr, types, headers, datamanager);
        htable->addSampleName(sampleName, true);
        htable->setMultiSampleMode(false);
        resultTabs->addTab(htable,  labels->getTabName(functionAttr));
        resultTabs->setTabToolTip(resultTabs->count()-1, labels->getTabName(functionAttr));

        if( !this->htablesAddSignals.contains(htable)) {
           htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
           connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
           connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
           this->htablesAddSignals.insert(htable, true);
        }
    }


#ifdef __REMOVE
    types.clear();
    types << STRING << STRING << INT;

    headers.clear();
    headers << "COG Category" << "COG Category (Alias) " << "ORF Count";
    htable = this->getHTableData(sampleName, COG, types, headers,  datamanager);
    htable->addSampleName(sampleName, true);
    htable->setMultiSampleMode(false);
    resultTabs->addTab(htable, "COG");
    resultTabs->setStyleSheet( "QTabBar::tab:selected { min-width: 100ex;}");



    resultTabs->setTabToolTip(resultTabs->count()-1, "COG");
    if( !this->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
       this->htablesAddSignals.insert(htable, true);
    }


    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "Pathway/Reaction" << "Common name" << "ORF Count";
    htable = this->getHTableData(sampleName, METACYC, types, headers, datamanager);
    htable->addSampleName(sampleName, true);
    htable->setMultiSampleMode(false);
    resultTabs->addTab(htable, "METACYC");
    resultTabs->setTabToolTip(resultTabs->count()-1, "METACYC");

    if( !this->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
       qDebug() << " connecting showtable metacyc";
       this->htablesAddSignals.insert(htable, true);
    }
    resultTabs->setStyleSheet( "QTabBar::tab { min-width: 25ex;}");


    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "SEED Subsystem Category" << "SEED Subsystem (Alias) " << "ORF Count";
    htable = this->getHTableData(sampleName, SEED, types, headers, datamanager);
    htable->addSampleName(sampleName, true);
    htable->setMultiSampleMode(false);

    resultTabs->addTab(htable, "SEED");
    resultTabs->setTabToolTip(resultTabs->count()-1, "SEED");
    if( !this->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
       this->htablesAddSignals.insert(htable, true);
    }

    //////// CAZY ////////

    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "CAZY Family" << "CAZY Family (Alias) " << "ORF Count";
    htable = this->getHTableData(sampleName, CAZY, types, headers, datamanager);
    htable->addSampleName(sampleName, true);
    htable->setMultiSampleMode(false);

    resultTabs->addTab(htable, "CAZY");
    resultTabs->setTabToolTip(resultTabs->count()-1, "CAZY");
    if( !this->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
       this->htablesAddSignals.insert(htable, true);
    }

#endif
#ifdef MEGAN_VIEW
    MeganView *m = this->meganviews["MEGAN_TAXONOMIC"];
    m->clearExistingTree();
    m->setDataFromFile(samplercmgr->getFilePath(sampleName, MEGANTREE));
    resultTabs->addTab(m, "TAXONOMIC");
#endif


    TableData *func_tax_table;
    /*func_tax_table = this->getFunctionalAndTaxTable(sampleName,true, true);
    this->simpleTabGroups.addTable(func_tax_table, sampleName, "FUNC & TAX");
    func_tax_table->loadData();
    */

    func_tax_table = this->_createFunctionalAndTaxTable(sampleName);
    resultTabs->addTab(func_tax_table, "FUNC & TAX");
    resultTabs->setTabToolTip(resultTabs->count()-1, "FUNCTIONAL & TAXONOMIC TABLE");
    progressbar.hide();


    ////////////////////////
    //   rRNA  TABLE

    QStringList rRNATableFiles = samplercmgr->getFilePaths(sampleName, rRNATABLES);

    TableData *t;
    foreach(QString rRNATableFile, rRNATableFiles) {
        types.clear();
        types << STRING << INT<< INT << STRING << STRING << STRING << STRING;
        columns.clear();
        columns << 0 << 1 << 2 << 3 << 4<<  5 << 6;
        QString rRNADatabase = samplercmgr->extractDBName(sampleName, rRNATableFile, rRNA);
        if(rRNADatabase.size() == 0) return;

        if( this->simpleTabGroups.tableExists(sampleName, rRNATableFile)) {
            t = this->simpleTabGroups.getTable(sampleName, rRNATableFile);
        }
        else { // table has to be created a new
            t = new TableData;
            t->useLCAStar(false);
            t->setParameters(false, rRNATableFile, types, columns,  false, QRegExp("^[^\\#]"));
            this->simpleTabGroups.addTable(t, sampleName, rRNATableFile);

        }
        t->setExportType(rRNATABLEEXP);
        t->setType(rRNATABLE);
        t->setSampleName(sampleName);
        t->setAuxName(rRNADatabase);
        t->hideLCAControl();
        t->hideFunctionalPopupControl();

        resultTabs->addTab(t, rRNADatabase);
        resultTabs->setTabToolTip(resultTabs->count()-1, rRNADatabase);
    }

    // tRNA TABLE
    QStringList tRNATableFiles = samplercmgr->getFilePaths(sampleName, tRNATABLES);

    foreach(QString tRNATableFile,tRNATableFiles) {
        types.clear();
        types << STRING << STRING<< STRING << INT << INT << STRING ;
        columns.clear();
        columns << 0 << 1 << 2 << 3 << 4<<  5 ;

        if( this->simpleTabGroups.tableExists(sampleName, tRNATableFile)) {
            t = this->simpleTabGroups.getTable(sampleName, tRNATableFile);
        }
        else { // table has to be created a new
            t = new TableData;
            t->useLCAStar(false);
            t->setParameters(false, tRNATableFile, types, columns,  false, QRegExp("^[^\\#]"));
            this->simpleTabGroups.addTable(t, sampleName, tRNATableFile);
        }
        t->setExportType(tRNATABLEEXP);
        t->setType(tRNATABLE);
        t->setSampleName(sampleName);
        t->hideLCAControl();
        t->hideFunctionalPopupControl();

        resultTabs->addTab(t, "tRNA-Scan");
        resultTabs->setTabToolTip(resultTabs->count()-1, "tRNA-Scan");
    }




//    // PATHWAY TABLE
//     types.clear();
//     types << STRING << STRING<< INT << INT<< INT;
//     columns.clear();
//     columns << 1 << 2 << 3 << 4<<  5;

//     t = this->tables["PATHWAYS"];
//     t->setParameters(false, pathwaysTable, types, columns, false,  QRegExp("^PWY:"));
//     resultTabs->addTab(t, "PATHwAYS");

//     // REACTION TABLE
//     types.clear();
//     types << STRING << STRING<< INT;
//     columns.clear();
//     columns << 1 << 2 << 5;

//     t = this->tables["REACTIONS"];
//     t->setParameters(false, pathwaysTable, types, columns, false, QRegExp("^RXN:"));
//     resultTabs->addTab(t, "REACTIONS");


    //resultTabs->addTab(this->graphicsRepresentation["MEGAN"], "MEGAN");

    // add the annotation sources table
   // types.clear();
 //   types << STRING << STRING << STRING << STRING << DOUBLE;
  //  t->setParameters(true, functionalSource1, types, true);
  //  t= this->tables["FUNC SRC"];
 //   p = new DisplayInfo();
  //  t->setPopupListener(p);
  //  resultTabs->addTab(t, "FUNC SRC");
  //  fileIndex = indexManager->getFileIndex(aminoFasta, FASTA);
   // p->addFileIndex(fileIndex,0);
    //return;

    emit fileChanged(sampleName);
}

/*
void ResultWindow::displayMenu(const QPoint &pos)
{
    QMenu menu(this);
    QAction *u = menu.addAction("remove"); // there can be more than one
    QAction *a = menu.exec(ui->tableWidget->viewport()->mapToGlobal(pos));
    if (a == u)
    {
        // do what you want or call another function
    }
}
*/

TableData *ResultWindow::_createFunctionalAndTaxTable(const QString &sampleName) {
    TableData *func_tax_table;

    DisplayInfo *p = this->displayInfos["FUNC & TAX"];
    p->removeFileIndexes();

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();

    FileIndex *fileIndex = samplercmgr->getFileIndex(sampleName, NUCFASTA);
    p->addFileIndex(fileIndex,4);
    fileIndex = samplercmgr->getFileIndex(sampleName, AMINOFAA);
    p->addFileIndex(fileIndex,0);
    QList<enum TYPE> types;
    types.clear();
    types << STRING << INT << INT << INT<<  STRING << INT << STRING << STRING << STRING << STRING;


    func_tax_table = new TableData;
    func_tax_table->useLCAStar(false);
    func_tax_table->largeTable  = new LargeTable();
    func_tax_table->largeTable->addLCAStar = false;

    func_tax_table->setSampleName(sampleName);
    func_tax_table->setParameters(false, samplercmgr->getFilePath(sampleName, FUNCTIONALTABLE), types, true);
    this->simpleTabGroups.addTable(func_tax_table, sampleName, "FUNC & TAX");

    func_tax_table->setPopupListener(p);
    func_tax_table->setExportType(OTHERSTABLEEXP);
    func_tax_table->loadData();
  //  resultTabs->addTab(func_tax_table, "FUNC & TAX");
  //  resultTabs->setTabToolTip(resultTabs->count()-1, "FUNCTIONAL & TAXONOMIC TABLE");
    return func_tax_table;
}




/**
 * @brief ResultWindow::addFunctionalAndTaxTable, add the functional and taxonomic table for the sample sampleName
 * @param sampleName, name of the sample
 * @return  ponter to the functiona and tax table
 */
TableData *ResultWindow::getFunctionalAndTaxTable(QString sampleName, bool useCache,  bool useLCAStar) {

  //  qDebug() << "Get functional table samplename " << sampleName;
    TableData *func_tax_table;
    if(  useCache ) {
        if(this->simpleTabGroups.tableExists(sampleName, "FUNC & TAX") ) {
            func_tax_table = this->simpleTabGroups.getTable(sampleName, "FUNC & TAX");
        }
        else {
            func_tax_table= this->_createFunctionalAndTaxTable(sampleName);
        }
    }
    else { // table has to be created a new

        DisplayInfo *p = this->displayInfos["FUNC & TAX"];
        p->removeFileIndexes();

        SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();

        FileIndex *fileIndex = samplercmgr->getFileIndex(sampleName, NUCFASTA);
        p->addFileIndex(fileIndex,4);
        fileIndex = samplercmgr->getFileIndex(sampleName, AMINOFAA);
        p->addFileIndex(fileIndex,0);

        QList<enum TYPE> types;
        types.clear();
        types << STRING << INT << INT << INT<<  STRING << INT << STRING << STRING << STRING << STRING;
        func_tax_table = new TableData;
        func_tax_table->useLCAStar(useLCAStar);
        func_tax_table->largeTable  = new LargeTable();
        func_tax_table->largeTable->addLCAStar =useLCAStar;

        func_tax_table->setSampleName(sampleName);
        func_tax_table->setParameters(false, samplercmgr->getFilePath(sampleName, FUNCTIONALTABLE), types, true);

    // if loaded then it always loads from cache table
        func_tax_table->setPopupListener(p);
        func_tax_table->setExportType(OTHERSTABLEEXP);
       // this->simpleTabGroups.addTable(func_tax_table, sampleName,"FUNC & TAX" );
       // func_tax_table->loadData();
      }

    return func_tax_table;
}


void ResultWindow::ProvideContexMenu(QPoint position)
{

        QMenu menu(this);
        QAction *u = menu.addAction(tr("remove"));

        /*
        QAction *a = menu.exec(tablewidget->viewport()->mapToGlobal(pos));
        if (a == u)
        {
        if (tablewidget->itemAt(pos).column() ==1) // your desired column
        {
            qDebug() << "clickecd on 1";
        }

         }
         */
    //context->exec(*position);
}



/**
 * @brief ResultWindow::showTable, shows the table for the functions and the taxa for the selected
 * set of orfs
 * @param sampleName
 * @param attrType
 */
void ResultWindow::showTable(QString sampleName,  ATTRTYPE attrType) {

    TableData *originalTable  = getFunctionalAndTaxTable(sampleName,true, false);
    //cache is true because we need the existing one

    TableData *newTable  = getFunctionalAndTaxTable(sampleName, false,  false);

    newTable->useLCAStar(true);

    GlobalDataTransit *globalDataTransit = GlobalDataTransit::getGlobalDataTransit();

    newTable->setWindowTitle("Functional and Taxonomic Table - " +  sampleName);

    /*
    foreach(ORF *o, globalDataTransit->orfList) {
        qDebug() << sampleName <<  o->name ;
    }*/

    newTable->largeTable->tableData.clear();
    this->copyROWData(originalTable->largeTable->tableData,  newTable->largeTable->tableData, globalDataTransit->orfList);
    this->copyROWData(originalTable->largeTable->wholeTableData,  newTable->largeTable->wholeTableData, globalDataTransit->orfList);

    newTable->types = originalTable->types;
    newTable->addLCAStar = originalTable->addLCAStar;
    newTable->largeTable->types = originalTable->largeTable->types;
    newTable->largeTable->colNames  = originalTable->largeTable->colNames;
    newTable->largeTable->index = originalTable->largeTable->index;
    newTable->largeTable->numCols = originalTable->largeTable->numCols;


    newTable->largeTable->lca_star_alpha = originalTable->largeTable->lca_star_alpha;
    newTable->largeTable->addLCAStar = false;

    newTable->largeTable->sampleName = sampleName;

    newTable->setSampleName( originalTable->getSampleName());

    newTable->setupFromFile( );
    newTable->show();
}

/**
 * @brief ResultWindow::copyROWData, copies the rows  from src table to a target table
 * corresponding to the selected ORFs only
 * @param src
 * @param tar
 * @param selectORFs
 */
void ResultWindow::copyROWData( QList<ROW *> &src, QList<ROW *> &tar, QList<ORF *> &selectORFs) {

    QHash<QString, bool> orfNames;
    foreach(ORF *orf, selectORFs) {
        orfNames.insert(orf->name, true);
       // qDebug() << "--->" << orf->name;
    }

    QString name;
   // qDebug() << "src size " << src.size();
    foreach(ROW *r , src) {
        name = Utilities::getShortORFId(r->strVar[0]);
        if( orfNames.contains(name)) tar.append(r);
    }
}


HTableData *ResultWindow::getHTableData(QString sampleName, ATTRTYPE attr,  QList<enum TYPE> types, QStringList headers,  DataManager *datamanager) {
    HTableData *htable = CreateWidgets::getHtableData(attr);

    htable->clearConnectors();
    Connector *connect = datamanager->createConnector(sampleName, datamanager->getHTree(attr), attr, datamanager->getORFList(sampleName));

    htable->setParameters(datamanager->getHTree(attr), types);

    htable->addConnector(connect, attr);

    htable->setMaxSpinBoxDepth(datamanager->getHTree(attr)->getTreeDepth());

    htable->addSampleName(sampleName);

    htable->setMultiSampleMode(false);
 //   htable->setShowHierarchy(true);

    htable->setTableIdentity(sampleName, attr);
    htable->showTableData();
    return htable;
}


void ResultWindow::switchToComparativeMode() {
    resultTabs->clear();

    //if no sample has been loaded then load the results
    if(files.size()==0)  _loadResults();

    QString orfTableName;
    QString orfMetaCycTableName;
    QString orfRPKMFileName;


    QList<enum TYPE> types;


    QStringList headers;

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();

    DataManager *datamanager = DataManager::getDataManager();


    QString OUTPUTPATH = this->rundata->getParams()["folderOutput"];
    ProgressView *progressbar = new ProgressView("Loading functional hierarchies ", 0, 0, this);
    progressbar->show();
    QApplication::processEvents();
    datamanager->createDataModel();
    progressbar->hide();
    delete progressbar;

    samplercmgr->setOutPutPath(OUTPUTPATH);


    Connector *connect;
    // create the orfs and add the metacyc annotation to the ORFs

    progressbar = new ProgressView("Creating ORFs for samples ", 0, this->selectedSamples.size(), this);
   // QProgressDialog progressdialog("linking ORFs to functional categories", "Cancel", 0, 100, this);
    progressbar->show();
    QApplication::processEvents();


    qDebug() << "selected XXX" << this->selectedSamples;

    unsigned int _i = 0;
    foreach(int i, selectedSamples) {
      //  if( !this->selectedSamples[i])  continue;

        orfTableName = samplercmgr->getFilePath(files[i], ORFTABLE);
        datamanager->createORFs(files[i]);
        orfMetaCycTableName = samplercmgr->getFilePath(files[i], ORFMETACYC);
        datamanager->addNewAnnotationToORFs(files[i], orfMetaCycTableName);

        orfRPKMFileName = samplercmgr->getFilePath(files[i], ORFRPKM);
        datamanager->addRPKMToORFs(files[i], orfRPKMFileName);

        progressbar->updateprogress(_i+1);
        qApp->processEvents();
        progressbar->update();
        _i++;

    }
    progressbar->hide();
    delete progressbar;


    progressbar = new ProgressView("Creating Taxons for samples ", 0, this->selectedSamples.size(), this);
   // QProgressDialog progressdialog("linking ORFs to functional categories", "Cancel", 0, 100, this);
    progressbar->show();
    _i = 0;
    foreach(unsigned int i,  selectedSamples) {
       //if( !this->selectedSamples[i])  continue;
        QString functionalTable = samplercmgr->getFilePath(files[i], FUNCTIONALTABLE   );
        datamanager->addTaxons(functionalTable);
        progressbar->updateprogress(_i+1);
        qApp->processEvents();
        progressbar->update();
        _i++;
    }
    progressbar->hide();
    delete progressbar;


    progressbar = new ProgressView("Link  Taxons to ORFs for samples ", 0, this->selectedSamples.size(), this);
   // QProgressDialog progressdialog("linking ORFs to functional categories", "Cancel", 0, 100, this);
    progressbar->show();
    _i =0;
    foreach(unsigned int i, selectedSamples) {
        QString functionalTable = samplercmgr->getFilePath(files[i], FUNCTIONALTABLE);
        datamanager->addTaxonToORFs( files[i], functionalTable);
        progressbar->updateprogress(i+1);
        qApp->processEvents();
        progressbar->update();
        _i++;
    }
    progressbar->hide();
    delete progressbar;

   QStringList selectedFileNames;
   foreach(unsigned int i, selectedSamples) {
       selectedFileNames.append(files[i]);
   }

   RunDataStats *rundatamodel = CreateWidgets::getRunDataStats();
   rundatamodel->clear();
   rundatamodel->setFileNames(selectedFileNames);
   rundatamodel->readStatFiles();

   QTableView *statTableView = CreateWidgets::getStatsTableView();
   resultTabs->addTab(statTableView, "RUN STATS");
   statTableView->setModel(rundatamodel);
   statTableView->setAlternatingRowColors(true);
   statTableView->show();

#ifdef SECTION
    statTableView->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    statTableView->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
#else
    statTableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    statTableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    statTableView->verticalHeader()->setResizeMode(QHeaderView::Stretch);
#endif


    ////////////////////////   KEGG, COG, METACYC, SEED, CAZY //////////////////

    HTableData *htable;
    QList<ATTRTYPE> functionTables;
    functionTables << KEGG << COG << METACYC << SEED << CAZY;
    Labels *labels = Labels::getLabels();

    foreach(ATTRTYPE functionAttr, functionTables) {
        htable= CreateWidgets::getHtableData(functionAttr); //new HTableData;
        htable->clearConnectors();

        types.clear();
        types << STRING << STRING;

        progressbar = new ProgressView( QString("Linking ORFs  to ") + labels->getTabName(functionAttr) + QString(" hierarchy"), 0, 0, this);
        progressbar->show();


        htable->clearSampleNames();
        foreach(unsigned int i, this->selectedSamples) {
           connect = datamanager->createConnector(files[i], datamanager->getHTree(functionAttr), functionAttr, datamanager->getORFList(files[i]) );
           htable->addConnector(connect, functionAttr);
           headers << files[i];
           htable->addSampleName(files[i]);
           types << INT;
        }

        htable->setParameters(datamanager->getHTree(functionAttr),  types);
        htable->setMaxSpinBoxDepth(datamanager->getHTree(functionAttr)->getTreeDepth());
     //   htable->setShowHierarchy(true);

        // htable->setHeaders(headers);
        htable->setTableIdentity(labels->getTabName(functionAttr), functionAttr);
        htable->setMultiSampleMode(true);

        htable->showTableData();


        if( !this->htablesAddSignals.contains(htable)) {
           htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
           QObject::connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
           QObject::connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
           this->htablesAddSignals.insert(htable, true);
        }
        resultTabs->addTab(htable, labels->getTabName(functionAttr));
        progressbar->hide();
        delete progressbar;
    } //for each functiona loop




#ifdef _REMOVE_FUNCTION
    /////////////////  KEGG
    htable = CreateWidgets::getHtableData(KEGG); //new HTableData;
    htable->clearConnectors();

    types.clear();
    types << STRING << STRING;
    htable->clearConnectors();
    progressbar = new ProgressView("linking ORFs  to KEGG categories ", 0, 0, this);
    progressbar->show();
    htable->clearSampleNames();
    for( int i=0; i < this->selectedSamples.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(KEGG), KEGG, datamanager->getORFList(files[i]));
       htable->addConnector(connect, KEGG);
       headers << files[i];
       htable->addSampleName(files[i]);
       types << INT;
    }
    progressbar->hide();
    delete progressbar;

    htable->setParameters(datamanager->getHTree(KEGG),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(KEGG)->getTreeDepth());

    htable->setShowHierarchy(true);

    htable->setTableIdentity("KEGG", KEGG);
    htable->setMultiSampleMode(true);
    htable->showTableData();

    if( !this->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       QObject::connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       QObject::connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
       this->htablesAddSignals.insert(htable, true);
    }
    resultTabs->addTab(htable, "KEGG");


    ////////////////// METACYC
    htable = CreateWidgets::getHtableData(METACYC); //new HTableData;
    htable->clearConnectors();

    types.clear();
    types << STRING << STRING;
    htable->clearSampleNames();
    for(int i=0; i < this->selectedSamples.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(METACYC), METACYC, datamanager->getORFList(files[i]));

       htable->addConnector(connect, METACYC);
       headers << files[i];
       htable->addSampleName(files[i]);
       types << INT;
    }
    htable->setParameters(datamanager->getHTree(METACYC),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(METACYC)->getTreeDepth());

    htable->setShowHierarchy(true);
    htable->setTableIdentity("METACYC", METACYC);
    htable->setMultiSampleMode(true);
    htable->showTableData();

    htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    if( !this->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       QObject::connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       QObject::connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
       this->htablesAddSignals.insert(htable, true);
    }
    resultTabs->addTab(htable, "METACYC");


    ////////////////////// SEED
    htable = CreateWidgets::getHtableData(SEED);;
    htable->clearConnectors();

    types.clear();
    types << STRING << STRING;
    htable->clearConnectors();
    progressbar = new ProgressView("linking ORFs  to SEED subsystems", 0, 0, this);
    progressbar->show();
    htable->clearSampleNames();
    for(int i=0; i < this->selectedSamples.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(SEED), SEED, datamanager->getORFList(files[i]) );
       htable->addConnector(connect, SEED);
       headers << files[i];
       htable->addSampleName(files[i]);
       types << INT;
    }
    progressbar->hide();
    delete progressbar;
    htable->setParameters(datamanager->getHTree(SEED),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(SEED)->getTreeDepth());
    htable->setShowHierarchy(true);

    htable->setTableIdentity("SEED", SEED);
    htable->setMultiSampleMode(true);
    htable->showTableData();


    if( !this->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       QObject::connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       QObject::connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
       this->htablesAddSignals.insert(htable, true);
    }
    resultTabs->addTab(htable, "SEED");

    ////// CAZY

    htable = CreateWidgets::getHtableData(CAZY);;
    htable->clearConnectors();

    types.clear();
    types << STRING << STRING;
    htable->clearConnectors();
    progressbar = new ProgressView("linking ORFs  to CAZY family", 0, 0, this);
    progressbar->show();
    htable->clearSampleNames();
    for(int i=0; i < this->selectedSamples.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(CAZY), CAZY, datamanager->getORFList(files[i]) );
       htable->addConnector(connect, CAZY);
       headers << files[i];
       htable->addSampleName(files[i]);
       types << INT;
    }
    progressbar->hide();
    delete progressbar;
    htable->setParameters(datamanager->getHTree(CAZY),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(CAZY)->getTreeDepth());
    htable->setShowHierarchy(true);

    htable->setTableIdentity("CAZY", CAZY);
    htable->setMultiSampleMode(true);
    htable->showTableData();


    if( !this->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       QObject::connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       QObject::connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
       this->htablesAddSignals.insert(htable, true);
    }
    resultTabs->addTab(htable, "CAZY");
#endif


}

RunData* ResultWindow::getRunData(){
    return this->rundata;
}


ResultWindow::~ResultWindow()
{
    delete ui;
}


