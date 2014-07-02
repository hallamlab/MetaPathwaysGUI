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
    tables["FUNC & TAX"] = new TableData();
    tables["rRNA"] = new TableData();

    displayInfos["FUNC & TAX"] = new DisplayInfo();


    graphs["CONT LEN HIST"] = new GraphData();
    graphs["ORF LEN HIST"] = new GraphData();

#ifdef MEGAN_VIEW
    meganviews["MEGAN_TAXONOMIC"]= new MeganView();
    DataManager *datamanager = DataManager::getDataManager();
    datamanager->createDataModel();
#endif

    /*
    tableParams.headers[KEGG] << "KEGG Function" << "KEGG function (alias) " << "ORF Count";
    tableParams.headers[COG] << "COG Category" << "COG Category (Alias) " << "ORF Count";
    tableParams.headers[METACYC] << "Pathway/Reaction" << "Common name" << "ORF Count";
    tableParams.headers[SEED] << "SEED Subsystem Category" << "SEED Subsystem (Alias) " << "ORF Count";
*/
}

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


void ResultWindow::clickedSelectSample(){
    this->selectWindow = new SelectSamples;
    this->selectWindow->setReceiver(this);
    this->selectWindow->addSamples(this->rundata->getCurrentFileList());
    this->selectWindow->show();
}


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

void ResultWindow::updateFileNames(){

  //  qDebug() << this->rundata->getFileList();

    files = this->rundata->getOutputFolders();
    qDebug() << "name of file s" << files;
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
        }
    }
    rundata->setCurrentSample(files[0]);

}

 void ResultWindow::receiveSelection(QList<bool> &selection) {
     this->selectedSamples = selection;
     DataManager *datamanager = DataManager::getDataManager();
     datamanager->createDataModel();
     this->switchToComparativeMode();

 }


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


    resultTabs->clear();
    for (unsigned int i=0; i< resultTabs->count(); i++){
        delete resultTabs->widget(i);
    }

    QList<enum TYPE> types;
    QList<unsigned int> columns;

    QStringList headers;

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
    datamanager->createORFs(sampleName, samplercmgr->getFilePath(sampleName, ORFTABLE) );
    datamanager->addNewAnnotationToORFs(sampleName, samplercmgr->getFilePath(sampleName, ORFMETACYC));
    datamanager->addRPKMToORFs(sampleName, samplercmgr->getFilePath(sampleName, ORFRPKM) );

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


    HTableData *htable;

    types.clear();
    types << STRING << STRING << INT;
    //headers = this->tableParams.headers[KEGG];
    headers << "KEGG Function" << "KEGG function (alias) " << "ORF Count";
    htable = this->getHTableData(sampleName, KEGG, types, headers, datamanager);
    htable->addSampleName(sampleName, true);
    htable->setMultiSampleMode(false);
    resultTabs->addTab(htable, "KEGG");
    resultTabs->setTabToolTip(resultTabs->count()-1, "KEGG");

    if( !this->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
       this->htablesAddSignals.insert(htable, true);
    }


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

#ifdef MEGAN_VIEW
    MeganView *m = this->meganviews["MEGAN_TAXONOMIC"];
    m->clearExistingTree();
    m->setDataFromFile(samplercmgr->getFilePath(sampleName, MEGANTREE));
    resultTabs->addTab(m, "TAXONOMIC");
#endif

    TableData *func_tax_table;


    func_tax_table = this->getFunctionalAndTaxTable(sampleName);
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
            t->setParameters(false, rRNATableFile, types, columns,  false, QRegExp("^[^\\#]"));
            this->simpleTabGroups.addTable(t, sampleName, rRNATableFile);
        }
        t->setExportType(rRNATABLEEXP);
        t->setType(rRNATABLE);
        t->setSampleName(sampleName);
        t->setAuxName(rRNADatabase);

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
            t->setParameters(false, tRNATableFile, types, columns,  false, QRegExp("^[^\\#]"));
            this->simpleTabGroups.addTable(t, sampleName, tRNATableFile);
        }
        t->setExportType(tRNATABLEEXP);
        t->setType(tRNATABLE);
        t->setSampleName(sampleName);
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

/**
 * @brief ResultWindow::addFunctionalAndTaxTable, add the functional and taxonomic table for the sample sampleName
 * @param sampleName, name of the sample
 * @return  ponter to the functiona and tax table
 */
TableData *ResultWindow::getFunctionalAndTaxTable(QString sampleName, bool useCache, bool loadData) {


    TableData *func_tax_table;
    //FUNCTION AND TAXONOMIC TABLE
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

    if(useCache )
       func_tax_table = this->tables["FUNC & TAX"];
    else {
       func_tax_table = new TableData;
       func_tax_table->largeTable  = new LargeTable();
    }
    func_tax_table->setSampleName(sampleName);
    func_tax_table->setParameters(false, samplercmgr->getFilePath(sampleName, FUNCTIONALTABLE), types, true);

    // if loaded then it always loads from cache table
    if(loadData)  func_tax_table->loadData();
    func_tax_table->setPopupListener(p);
    func_tax_table->setExportType(OTHERSTABLEEXP);
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

    qDebug() << " something index";
    //context->exec(*position);
}



/**
 * @brief ResultWindow::showTable, shows the table
 * @param sampleName
 * @param attrType
 */
void ResultWindow::showTable(QString sampleName,  ATTRTYPE attrType) {

    TableData *originalTable  = getFunctionalAndTaxTable(sampleName);

    TableData *newTable  = getFunctionalAndTaxTable(sampleName, false, false);

    GlobalDataTransit *globalDataTransit = GlobalDataTransit::getGlobalDataTransit();

    newTable->setWindowTitle("Functional and Taxonomic Table - " +  sampleName);
    newTable->largeTable->tableData.clear();
    this->copyROWData(originalTable->largeTable->tableData,  newTable->largeTable->tableData, globalDataTransit->orfList);
    this->copyROWData(originalTable->largeTable->wholeTableData,  newTable->largeTable->wholeTableData, globalDataTransit->orfList);

    newTable->types = originalTable->types;
    newTable->largeTable->types = originalTable->largeTable->types;
    newTable->largeTable->colNames  = originalTable->largeTable->colNames;
    newTable->largeTable->index = originalTable->largeTable->index;
    newTable->largeTable->numCols = originalTable->largeTable->numCols;
    newTable->setupFromFile( );
    newTable->show();

}


void ResultWindow::copyROWData( QList<ROW *> &src, QList<ROW *> &tar, QList<ORF *> &selectORFs) {

    QHash<QString, bool> orfNames;
    foreach(ORF *orf, selectORFs) {
        orfNames.insert(orf->name, true);
    }


    QString name;
    foreach(ROW *r , src) {
      //  qDebug() << r->strVar[0];
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
    htable->setShowHierarchy(true);
   // htable->setHeaders(headers);
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

  //  qDebug() << "comparative mode";

    QList<enum TYPE> types;


    QStringList headers;

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();

    DataManager *datamanager = DataManager::getDataManager();
   //// datamanager->createORFs(changed, orfTableName);

    QString OUTPUTPATH = this->rundata->getParams()["folderOutput"];
    datamanager->createDataModel();
    samplercmgr->setOutPutPath(OUTPUTPATH);


  //  this->indexSamples(true);

    //// datamanager->addNewAnnotationToORFs(changed, orfMetaCycTableName);

    Connector *connect;
    // create the orfs and add the metacyc annotation to the ORFs

    unsigned int numSamplesToLoad = 0;
    for(int i=0; i < this->selectedSamples.size(); i++) {
        if(this->selectedSamples[i])  numSamplesToLoad++;
    }
    ProgressView *progressbar = new ProgressView("creating ORFs for samples ", 0, numSamplesToLoad, this);
   // QProgressDialog progressdialog("linking ORFs to functional categories", "Cancel", 0, 100, this);
    progressbar->show();


    for(int i=0; i < selectedSamples.size(); i++) {
       if( !this->selectedSamples[i])  continue;

        orfTableName = samplercmgr->getFilePath(files[i], ORFTABLE);
        datamanager->createORFs(files[i], orfTableName);
        orfMetaCycTableName = samplercmgr->getFilePath(files[i], ORFMETACYC);
        datamanager->addNewAnnotationToORFs(files[i], orfMetaCycTableName);

        orfRPKMFileName = samplercmgr->getFilePath(files[i], ORFRPKM);
        datamanager->addRPKMToORFs(files[i], orfRPKMFileName);

        progressbar->updateprogress(i+1);
        qApp->processEvents();
        progressbar->update();

    }
    progressbar->hide();
    delete progressbar;


   QStringList selectedFileNames;
   for(unsigned int i=0; i < selectedSamples.size(); i++) {
       if( !this->selectedSamples[i])  continue;
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



    ////////////////////////   COG
    HTableData *htable = CreateWidgets::getHtableData(COG); //new HTableData;
    htable->clearConnectors();


    types.clear();
    types << STRING << STRING;

    progressbar = new ProgressView("linking ORFs  to COG categories ", 0, 0, this);
    progressbar->show();
    for(int i=0; i < this->selectedSamples.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(COG), COG, datamanager->getORFList(files[i]) );
       htable->addConnector(connect, COG);
       headers << files[i];
       htable->addSampleName(files[i]);
       types << INT;
    }
    progressbar->hide();
    delete progressbar;

    htable->setParameters(datamanager->getHTree(COG),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(COG)->getTreeDepth());
    htable->setShowHierarchy(true);
   // htable->setHeaders(headers);
    htable->setTableIdentity("COG", COG);
    htable->setMultiSampleMode(true);
    htable->showTableData();


    if( !this->htablesAddSignals.contains(htable)) {
       htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
       QObject::connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT( ProvideContexMenu(QPoint)  ));
       QObject::connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
       this->htablesAddSignals.insert(htable, true);
    }

    resultTabs->addTab(htable, "COG");


    /////////////////  KEGG
    htable = CreateWidgets::getHtableData(KEGG); //new HTableData;
    htable->clearConnectors();

    types.clear();
    types << STRING << STRING;

    /*
    headers.clear();
    headers << "KEGG Function" << "KEGG function (alias)" ;
    */
    htable->clearConnectors();
    progressbar = new ProgressView("linking ORFs  to KEGG categories ", 0, 0, this);
    progressbar->show();
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


  //  htable->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
   // QObject::connect(htable->tableWidget, SIGNAL(customContextMenuRequested(QPoint)), htable, SLOT(  ProvideContexMenu(QPoint) )  );
   // QObject::connect(htable, SIGNAL( showTable(QString, ATTRTYPE) ), this, SLOT( showTable(QString, ATTRTYPE)  ));
    resultTabs->addTab(htable, "KEGG");



    ////////////////// METACYC
    htable = CreateWidgets::getHtableData(METACYC); //new HTableData;
    htable->clearConnectors();

    types.clear();
    types << STRING << STRING;

    /*
    headers.clear();
    headers << "Pathway/Reaction" << "Common name" ;
    htable->clearConnectors();
*/


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

    /*
    headers.clear();
    headers << "SEED Subsystem Category" << "SEED Subsystem (Alias) ";
    */
    htable->clearConnectors();
    progressbar = new ProgressView("linking ORFs  to SEED subsystems", 0, 0, this);
    progressbar->show();
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


}

RunData* ResultWindow::getRunData(){
    return this->rundata;
}


ResultWindow::~ResultWindow()
{
    delete ui;
}


