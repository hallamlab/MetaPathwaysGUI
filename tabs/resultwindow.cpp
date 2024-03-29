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
    resultViewMode = this->findChild<QComboBox *>("resultViewMode");
    selectSamplesButton = this->findChild<QPushButton *>("selectSamplesButton");
    currentSampleLabel = this->findChild<QLabel *>("currentSampleLabel");
    addResultFolder = this->findChild<QToolButton *>("addResultFolder");
    deleteResultFolder = this->findChild<QToolButton *>("deleteResultFolder");
    resultFolders = this->findChild<QComboBox *>("resultFolders");
    statusOnlyBox = this->findChild<QCheckBox *>("statusBox");

    functionBoxes[KEGG] = this->findChild<QCheckBox *>("keggBox");
    functionBoxes[COG] = this->findChild<QCheckBox *>("cogBox");
    functionBoxes[METACYC] = this->findChild<QCheckBox *>("metacycBox");
    functionBoxes[CUSTOM] = this->findChild<QCheckBox *>("customBox");
    functionBoxes[SEED] = this->findChild<QCheckBox *>("seedBox");
    functionBoxes[CAZY] = this->findChild<QCheckBox *>("cazyBox");


    this->setVisible(0);
    selectSamplesButton->hide();

#ifdef REINDEX
    reindex = this->findChild<QCheckBox *>("reindex");
    reindex->hide();
#endif
    loadResults = this->findChild<QPushButton *>("loadResults");
    loadResults->hide();

    selectSamplesButton->setEnabled(false);


    resultTabs->setUsesScrollButtons ( true );
    resultTabs->clear();
    resultTabs->setUsesScrollButtons(true);
    getFileNames = new QTimer();
    getFileNames->start(500);


    connect(sampleSelect,SIGNAL(activated(QString)),this,SLOT(sampleChanged(QString)));
   // connect(getFileNames, SIGNAL(timeout()),this,SLOT(updateFileNames()));
    //connect(this,SIGNAL(fileChanged(QString)),this->progress,SLOT(selectedFileChanged(QString)));
    connect(resultViewMode,  SIGNAL(currentIndexChanged(int)), this, SLOT(useCurrentFolderText() ) );
    connect(resultViewMode,  SIGNAL(currentIndexChanged(int)), this, SLOT(setVisible(int)) );


    connect(selectSamplesButton, SIGNAL(clicked()), this, SLOT(clickedSelectSample()  ) );
    connect(loadResults, SIGNAL(clicked()), this, SLOT(_loadResults()) );
    connect(addResultFolder, SIGNAL(clicked()), this, SLOT(browseFolder()));
    connect(deleteResultFolder, SIGNAL(clicked()), this, SLOT(removeFolder()));

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

    this->loadStatsOnly = true;

}

void ResultWindow::browseFolder(){
    QString folderSelect = QFileDialog::getExistingDirectory(this, tr("Select a directory of processed output."));

    QHash<QString, bool> itemTexts;
    for(int i =0; i < resultFolders->count(); i++ )
        itemTexts.insert(resultFolders->itemText(i), true);

    if(!folderSelect.isEmpty() && !itemTexts.contains(folderSelect)){
        this->resultFolders->addItem(folderSelect);
        this->_loadResults();
    }

  //  this->rundata->setValue("folderOutput", selectedFolder, _PARAMS);
    //send a signal to the parent to enable the continue button
 /*   if (!selectedFolder.isEmpty()){
        if(!selectedFiles.isEmpty()) emit fileSet();
    } */
}



void ResultWindow::removeFolder(){
    QString currentFolder = this->resultFolders->currentText();
    if( currentFolder.isEmpty() ) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Test", QString("Stop loading results from  result folder ") + currentFolder + QString(" ?"),
                                   QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        this->resultFolders->removeItem(this->resultFolders->currentIndex());
        this->_loadResults();
    }
}


/**
 * @brief ResultWindow::_loadResults, loads the samplewise results from the output folder
 */
void ResultWindow::_loadResults() {
 /*   if( this->resultFolders->count()==0) {
        QMessageBox::warning(this, "Result folders not specified", "Add folders that has processed output", QMessageBox::Ok);
        return;
    }
*/

    this->disableSampleChanged = true;
    DataManager *datamanager = DataManager::getDataManager();
    datamanager->destroyDataModel();
    datamanager->setDataModelCreated(false);
    /***/

    TableManager *tableManager = TableManager::getTableManager();
    tableManager->deleteAllGraphData();
    tableManager->deleteAllTables();


    this->rundata->setResultFolders(this->getResultFoldersList());
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
 * @brief ResultWindow::getResultFoldersList, gets the list of result folders to look into
 * @return
 */

QStringList ResultWindow::getResultFoldersList() {
    QStringList resultFoldersList;

    for(int i =0; i < this->resultFolders->count(); i++)
        resultFoldersList.append(this->resultFolders->itemText(i));

    return resultFoldersList;
}


/**
 * @brief ResultWindow::useCurrentFolderText, makes sure that the current folder is used
 *
 */
void ResultWindow::useCurrentFolderText() {
    // pick up any new folder pasted
    QString currentFolder = this->resultFolders->currentText();

    QHash<QString, bool> itemTexts;
    for(int i =0; i < resultFolders->count(); i++ )
        itemTexts.insert(resultFolders->itemText(i), true);

    if(!currentFolder.isEmpty() && !itemTexts.contains(currentFolder)){
        this->resultFolders->addItem(currentFolder);
        this->_loadResults();
    }

}

/**
 * @brief ResultWindow::showFlatSampleSelect, shows the menu to select the flatSample Selection
 * widget
 */
void ResultWindow::showFlatSampleSelect(){
    this->useCurrentFolderText();

    this->selectWindow = new SelectSamples;
    this->selectWindow->setReceiver(this);

    this->rundata->setResultFolders(this->getResultFoldersList());
    this->selectWindow->addSamplesUnchecked(this->rundata->getOutputFolders());

    this->selectWindow->show();
}

/**
 * @brief ResultWindow::showHierarchicalSampleSelect, show the hierarchical menu to select samples
 */
void ResultWindow::showHierarchicalSampleSelect() {
    this->rundata->setResultFolders(this->getResultFoldersList());

    QHash<QString, HIERARCHICAL_SAMPLE> activesamples;

    QFileInfo fileInfo;

    // create the structure of the active files
    foreach( QString folder, this->rundata->getResultFolders()) {
        foreach(QString sample, this->rundata->getOutputSampleFolderPairs(folder)) {
            if( !activesamples.contains(sample)) {
                HIERARCHICAL_SAMPLE name;
                name.valid = false;
                name.name = sample;
                activesamples[sample] = name;
            }
            activesamples[sample].folders.append(sample);
        }
    }


    // report the duplicates
    QStringList allsamples = activesamples.keys();
    foreach(QString sample, allsamples) {
        if( activesamples[sample].folders.size() > 1) activesamples.remove(sample);
    }


    // read the metadata
    foreach( QString folder, this->rundata->getResultFolders()) {
        QString metadatafile = folder + QDir::separator() + QString("metadata.txt");


        fileInfo.setFile(metadatafile);
        if( !fileInfo.exists() ) continue;
        QHash<QString, QStringList> samplesinfo;
        Utilities::read_metadata_file(metadatafile, samplesinfo);

        foreach(QString sample, samplesinfo.keys()) {
            // ignore non-existant samples
            if( !activesamples.contains(sample)) continue;

            // if the sample is validated then do not worry
            if( activesamples[sample].valid ) continue;

            // validate it if the metadata folder and sample folder matches
            QDir dir1, dir2;
            dir1.absoluteFilePath(folder);
            dir2.absoluteFilePath(activesamples[sample].folders[0]);
            if( dir1.absolutePath().compare(dir1.absolutePath())==0) {
                activesamples[sample].valid = true;
                activesamples[sample].categories = samplesinfo[sample];
            }
        }
    }



    QHash<QString, QStringList> sampletodisplay ;
    foreach( QString sample, activesamples.keys()) {
        sampletodisplay[sample] = activesamples[sample].categories;
    }



    HierarchicalView *hierarchicalview = HierarchicalView::getHierarchicalView();
    hierarchicalview->setReceiver(this);
    hierarchicalview->showTreeView(sampletodisplay);

}


/**
 * @brief ResultWindow::clickedSelectSample, loads the output results for the
 * comparative mode
 */
void ResultWindow::clickedSelectSample(){

    if(this->resultViewMode->currentIndex() == 1) this->showFlatSampleSelect();

    if(this->resultViewMode->currentIndex() == 2) this->showHierarchicalSampleSelect();
}


/**
 * @brief ResultWindow::setVisible, sets the resulttab to visible
 * @param i
 */
void ResultWindow::setVisible(int i) {
    if( i > 0 ) {
        this->selectSamplesButton->setEnabled(true);
        this->selectSamplesButton->show();
        this->sampleSelect->setEnabled(false);
        this->sampleSelect->hide();
    }
    else {
        this->selectSamplesButton->setEnabled(false);
        this->selectSamplesButton->hide();
        this->sampleSelect->setEnabled(true);
        this->sampleSelect->show();
    }
}

/**
 * @brief ResultWindow::updateFileNames, updates the filenames
 */
void ResultWindow::updateFileNames(){

    files = this->rundata->getOutputFolders();
    sampleSelect->clear();
    if( files.isEmpty()) return;

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

 void ResultWindow::receiveSelection(QList<unsigned int> selection, QStringList categories) {
     this->selectedSamples = selection;
     this->categories = categories;
     this->switchToComparativeMode();
 }


 /**
  * @brief ResultWindow::getActiveCategories, get the set of active categories
  * @param activeCategories
  */
 void ResultWindow::getActiveCategories(QHash<ATTRTYPE, bool> &activeCategories) const {

     QList<ATTRTYPE> functionTables;
     functionTables << KEGG << COG << METACYC << SEED << CAZY << CUSTOM;

     foreach(ATTRTYPE functionAttr, functionTables) {
         activeCategories[functionAttr] = this->functionBoxes[functionAttr]->isChecked();
     }

 }


 /**
 * @brief ResultWindow::sampleChanged, this function loads a new sample and
 * is triggered through a sample change on the combobox
 * @param sampleName
 */
void ResultWindow::sampleChanged(QString sampleName){

  //  qDebug() << " load me";

    QRegExp select_sample_regexp = QRegExp("Select sample");
    if( select_sample_regexp.indexIn(sampleName) != -1 ) return;

    this->rundata->setCurrentSample(sampleName);

    SampleResourceManager *sampleResourceManager = SampleResourceManager::getSampleResourceManager();
    DataManager *datamanager = DataManager::getDataManager();
    sampleResourceManager->setOutPutFolders(this->rundata->getSampleFolderMap());
    if(this->needsReloading() ) this->_loadResults();
    else qDebug() << "no realoading";
    QHash<ATTRTYPE, bool> activeCategories;
    this->getActiveCategories(activeCategories);
    qDebug() << "active " << activeCategories.keys();
    datamanager->createDataModel(activeCategories);

    sampleResourceManager->setOutPutFolders(this->rundata->getSampleFolderMap());

#ifdef REINDEX
    if( reindex->isChecked() ) {
        this->indexSamples(true, true);
        reindex->setChecked(false);
    }
#endif

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

    datamanager->loadEquivalentORFs(samplercmgr->getFilePath(sampleName, ORFREDUCED));
    datamanager->addNewAnnotationToORFs(sampleName, samplercmgr->getFilePath(sampleName, ORFMETACYC));
    datamanager->addCustomAnnotationToORFs(sampleName, samplercmgr->getFilePath(sampleName, CUSTOMTABLE));

    QFile inforpkm(samplercmgr->getFilePath(sampleName, ORFRPKM));

    if(inforpkm.exists() ) {
       datamanager->addRPKMToORFs(sampleName, samplercmgr->getFilePath(sampleName, ORFRPKM) );
    }
    else {
       datamanager->addRPKMToORFs(sampleName, samplercmgr->getFilePath(sampleName, CLUSTERWT) );
    }

    QString functionalTable = samplercmgr->getFilePath(sampleName, FUNCTIONALTABLE);
    QFileInfo info(functionalTable);
    if(!info.exists()) functionalTable = samplercmgr->getFilePath(sampleName, FUNCTIONALTABLELONG);

    datamanager->addTaxons(functionalTable);

    // disable the taxons
    //datamanager->addTaxonToORFs( sampleName, functionalTable);

    QStringList selectedFileNames;
    selectedFileNames.append(sampleName);

    RunDataStats *rundatamodel = CreateWidgets::getRunDataStats();
    rundatamodel->clear();
    rundatamodel->setFileNames(selectedFileNames);
    rundatamodel->readStatFiles();

    StatsQGroupBox *statsTableWidget = CreateWidgets::getStatsTableCompleteView();
    statsTableWidget->setModel(rundatamodel);
    resultTabs->addTab(statsTableWidget, "RUN STATS");
    resultTabs->setTabToolTip(resultTabs->count()-1, "RUN STATS");


    // now the contig and orf length distributions

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


    if( this->statusOnlyBox->isChecked()) return;

    //////////////  FUNCTIONAL CATEGORIES KEGG, COG, METACYC, SEED, CAZY //////////////////

    HTableData *htable;
    Labels *labels = Labels::getLabels();


    QList<ATTRTYPE> functionTables;
    functionTables << KEGG << COG << METACYC << SEED << CAZY << CUSTOM;

    foreach(ATTRTYPE functionAttr, functionTables) {

        if( !this->functionBoxes[functionAttr]->isChecked() ) continue;

        types.clear();
        types << STRING << STRING << MP_INT;
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

   //  emit fileChanged(sampleName);
   // return;

    ////////////////////////
    //   rRNA  TABLE

    QStringList rRNATableFiles = samplercmgr->getFilePaths(sampleName, rRNATABLES);

    TableData *t;
    foreach(QString rRNATableFile, rRNATableFiles) {
        types.clear();
        types << STRING << MP_INT<< MP_INT << STRING << STRING << STRING << STRING;
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
            t->setParameters(false, rRNATableFile, QString(""), types, columns,  false, QRegExp("^[^\\#]"));
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
        types << STRING << STRING<< STRING << MP_INT << MP_INT << STRING ;
        columns.clear();
        columns << 0 << 1 << 2 << 3 << 4<<  5 ;

        if( this->simpleTabGroups.tableExists(sampleName, tRNATableFile)) {
            t = this->simpleTabGroups.getTable(sampleName, tRNATableFile);
        }
        else { // table has to be created a new
            t = new TableData;
            t->useLCAStar(false);
            // tRNATable does not yet need a name mapping
            t->setParameters(false, tRNATableFile, QString(""), types, columns,  false, QRegExp("^[^\\#]"));
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
    types << STRING << MP_INT << MP_INT << MP_INT<<  STRING << STRING << MP_INT << STRING << STRING << STRING << STRING;


    func_tax_table = new TableData;
    func_tax_table->useLCAStar(false);
    func_tax_table->largeTable  = new LargeTable();
    func_tax_table->largeTable->addLCAStar = false;

    QString functionTableName ;
    QFileInfo fileinfo;

    functionTableName = samplercmgr->getFilePath(sampleName, FUNCTIONALTABLE);
    fileinfo.setFile(functionTableName);
    if( !fileinfo.exists()) {
           functionTableName = samplercmgr->getFilePath(sampleName, FUNCTIONALTABLELONG);
    }

    func_tax_table->setSampleName(sampleName);
    func_tax_table->setParameters(false, functionTableName,  samplercmgr->getFilePath(sampleName,RENAMEMAP), types, true);
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
    else { //A NEW table has to be created

        DisplayInfo *p = this->displayInfos["FUNC & TAX"];
        p->removeFileIndexes();

        SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();

        FileIndex *fileIndex = samplercmgr->getFileIndex(sampleName, NUCFASTA);
        p->addFileIndex(fileIndex,4);
        fileIndex = samplercmgr->getFileIndex(sampleName, AMINOFAA);
        p->addFileIndex(fileIndex,0);

        QList<enum TYPE> types;
        types.clear();
        types << STRING << MP_INT << MP_INT << MP_INT<<  STRING << STRING << MP_INT << STRING << STRING << STRING << STRING;
        func_tax_table = new TableData;
        func_tax_table->useLCAStar(useLCAStar);
        func_tax_table->largeTable  = new LargeTable();
        func_tax_table->largeTable->addLCAStar =useLCAStar;

        func_tax_table->setSampleName(sampleName);
        func_tax_table->setParameters(false, samplercmgr->getFilePath(sampleName, FUNCTIONALTABLE), samplercmgr->getFilePath(sampleName, RENAMEMAP), types, true);

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

     /*   QMenu menu(this);
        QAction *u = menu.addAction(tr("remove"));
*/
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


bool ResultWindow::needsReloading(QString sampleName) {

   SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
   QString steps_log_file = samplercmgr->getFilePath(sampleName, STEPS_LOG);


   QFile inputFile(steps_log_file);
   QRegExp commentLine("#[^\"\\n\\r]*");
   QRegExp runIdPattern("(\\d+:\\d+:\\d+)");

   QString runid = QString("00:00:00");

   if (inputFile.exists() && inputFile.open(QIODevice::ReadOnly))
   {
      QTextStream in(&inputFile);
      while ( !in.atEnd() )
      {
         QString line = in.readLine();
         if (!commentLine.exactMatch(line) && line.length()>0){   //if not a comment line
            if(line.contains(QString("======")) && line.contains(QString("BLOCK")) ) {
                int pos  = runIdPattern.indexIn(line);
                if( pos > -1 ) {
                    runid = runIdPattern.cap(1);
                }
            }
         }
      }
      inputFile.close();
   }

   if( this->runids.contains(sampleName) &&\
       runid.compare(this->runids[sampleName]) ==0 ) {
     return false;
   }

   this->runids[sampleName] = runid;
   return true;
}

bool ResultWindow::needsReloading()  {

   foreach(int i, selectedSamples) {
       if(this->needsReloading(this->files[i])) return true;
   }

   return false;

}

void ResultWindow::switchToComparativeMode() {
    resultTabs->clear();

  //  qDebug() << " Beginning comparative mode";
    //if no sample has been loaded then load the results

    if(files.size()==0)  _loadResults();

    QString orfTableName;
    QString orfMetaCycTableName;

    QList<enum TYPE> types;

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
    DataManager *datamanager = DataManager::getDataManager();

    samplercmgr->setOutPutFolders(this->rundata->getSampleFolderMap());
    if(this->needsReloading()) this->_loadResults();

    QStringList selectedFileNames;
    foreach(unsigned int i, selectedSamples) {
        selectedFileNames.append(files[i]);
    }

    RunDataStats *rundatamodel = CreateWidgets::getRunDataStats();
    rundatamodel->clear();
    rundatamodel->setFileNames(selectedFileNames);
    rundatamodel->setCategories(this->categories);
    rundatamodel->readStatFiles();



    StatsQGroupBox *statsTableWidget = CreateWidgets::getStatsTableCompleteView();
    statsTableWidget->setModel(rundatamodel);

    resultTabs->addTab(statsTableWidget, "RUN STATS");
    resultTabs->setTabToolTip(resultTabs->count()-1, "RUN STATS");


    if( this->statusOnlyBox->isChecked()) return;

    ProgressView *progressbar = new ProgressView("Loading functional hierarchies ", 0, 0, this);
    progressbar->show();
    QApplication::processEvents();

    QHash<ATTRTYPE, bool> activeCategories;
    this->getActiveCategories(activeCategories);
    datamanager->createDataModel(activeCategories);


    progressbar->hide();
    delete progressbar;

    Connector *connect;
    // create the orfs and add the metacyc annotation to the ORFs

    progressbar = new ProgressView("Creating ORFs for samples ", 0, this->selectedSamples.size(), this);
    progressbar->show();
    QApplication::processEvents();

    unsigned int _i = 0;


    foreach(int i, selectedSamples) {

        orfTableName = samplercmgr->getFilePath(files[i], ORFTABLE);
        datamanager->createORFs(files[i]);

        orfMetaCycTableName = samplercmgr->getFilePath(files[i], ORFMETACYC);
        datamanager->loadEquivalentORFs(samplercmgr->getFilePath(files[i], ORFREDUCED));
        datamanager->addNewAnnotationToORFs(files[i], orfMetaCycTableName);
        datamanager->addCustomAnnotationToORFs(files[i], samplercmgr->getFilePath(files[i], CUSTOMTABLE));

        QFile inforpkm(samplercmgr->getFilePath(files[i], ORFRPKM));
        if(inforpkm.exists() ) {
            datamanager->addRPKMToORFs(files[i], samplercmgr->getFilePath(files[i], ORFRPKM) );
        }
        else {
            datamanager->addRPKMToORFs(files[i], samplercmgr->getFilePath(files[i], CLUSTERWT) );
        }


        progressbar->updateprogress(_i+1);
        qApp->processEvents();
        progressbar->update();
        _i++;
    }

    progressbar->hide();
    delete progressbar;


#ifdef LOAD_FUNCTIONAL_TABLE
    progressbar = new ProgressView("Creating Taxons for samples ", 0, this->selectedSamples.size(), this);
    progressbar->show();
    _i = 0;
    foreach(unsigned int i,  selectedSamples) {
       //if( !this->selectedSamples[i])  continue;
        QString functionalTable = samplercmgr->getFilePath(files[i], FUNCTIONALTABLE);
        QFileInfo info(functionalTable);
        if(!info.exists()) functionalTable = samplercmgr->getFilePath(files[i], FUNCTIONALTABLELONG);

        datamanager->addTaxons(functionalTable);
        progressbar->updateprogress(_i+1);
        qApp->processEvents();
        progressbar->update();
        _i++;
    }
    progressbar->hide();
    delete progressbar;
#endif

    progressbar = new ProgressView("Link  Taxons to ORFs for samples ", 0, this->selectedSamples.size(), this);
    progressbar->show();
    _i =0;
    foreach(unsigned int i, selectedSamples) {
        QString functionalTable = samplercmgr->getFilePath(files[i], FUNCTIONALTABLE);
        QFileInfo info(functionalTable);
        if(!info.exists()) functionalTable = samplercmgr->getFilePath(files[i], FUNCTIONALTABLELONG);

       // do not add taxons
       // datamanager->addTaxonToORFs( files[i], functionalTable);
        progressbar->updateprogress(i+1);
        qApp->processEvents();
        progressbar->update();
        _i++;
    }
    progressbar->hide();
    delete progressbar;



   ////////////////////////   KEGG, COG, METACYC, SEED, CAZY //////////////////
    HTableData *htable;
    QList<ATTRTYPE> functionTables;
    functionTables << KEGG << COG << METACYC << SEED << CAZY << CUSTOM;
    Labels *labels = Labels::getLabels();

    foreach(ATTRTYPE functionAttr, functionTables) {
        if( !this->functionBoxes[functionAttr]->isChecked() ) continue;

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
           htable->addSampleName(files[i]);
           types << MP_INT;
        }

        htable->setParameters(datamanager->getHTree(functionAttr),  types);
        htable->setMaxSpinBoxDepth(datamanager->getHTree(functionAttr)->getTreeDepth());
     //   htable->setShowHierarchy(true);

        htable->setHeaderCategories(this->categories);
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

}

RunData* ResultWindow::getRunData(){
    return this->rundata;
}


ResultWindow::~ResultWindow()
{
    delete ui;
}


