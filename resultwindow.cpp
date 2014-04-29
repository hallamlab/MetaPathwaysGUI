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


    resultTabs->clear();
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
//    tables["FUNC SRC"] = new TableData();
//    tables["FUNC KEGG 1"] = new TableData();
//    tables["FUNC KEGG 2"] = new TableData();
//    tables["FUNC KEGG 3"] = new TableData();
//    tables["FUNC KEGG 4"] = new TableData();
//    tables["FUNC COG 1"] = new TableData();
//    tables["FUNC COG 2"] = new TableData();
//    tables["FUNC COG 3"] = new TableData();

    displayInfos["FUNC & TAX"] = new DisplayInfo();
//    displayInfos["FUNC SRC"] = new DisplayInfo();
//    displayInfos["FUNC KEGG 1"] = new DisplayInfo();
//    displayInfos["FUNC KEGG 2"] = new DisplayInfo();
//    displayInfos["FUNC KEGG 3"] = new DisplayInfo();
//    displayInfos["FUNC KEGG 4"] = new DisplayInfo();
//    displayInfos["FUNC COG 1"] = new DisplayInfo();
//    displayInfos["FUNC COG 2"] = new DisplayInfo();
//    displayInfos["FUNC COG 3"] = new DisplayInfo();

//    graphs["CONT LEN HIST"] = new GraphData();
//    graphs["ORF LEN HIST"] = new GraphData();

#ifdef MEGAN_VIEW
    meganviews["MEGAN_TAXONOMIC"]= new MeganView();
    DataManager *datamanager = DataManager::getDataManager();
    datamanager->createDataModel();
#endif
}

void ResultWindow::_loadResults() {

    if( this->rundata->getParams()["fileInput"].size()==0 || this->rundata->getParams()["folderOutput"].size()==0) {
        QMessageBox::warning(this, "Input/Output Folder Missing", "Input or Output folder not set in the <b>Stages</b> tab", QMessageBox::Ok);
        return;
    }

    QFileInfo input(this->rundata->getParams()["fileInput"]);
    QFileInfo output(this->rundata->getParams()["folderOutput"]);
    if( !input.exists()) {
        QMessageBox::warning(this, "Input Folder does not exist", "Input folder specified in Stages tab does not exist!", QMessageBox::Ok);
        return;
    }
    if( !output.exists()) {
        QMessageBox::warning(this, "Output Folder does not exist", "Output folder specified in Stages tab does not exist!", QMessageBox::Ok);
        return;
    }

    this->disableSampleChanged = true;
    this->updateFileNames();
    this->disableSampleChanged = false;
    DataManager *datamanager = DataManager::getDataManager();
    /***/
    datamanager->destroyAllORFs();
    datamanager->deleteAllConnectors();
    datamanager->destroyAllHTrees();
    datamanager->destroyAllAttributes();

    TableManager *tableManager = TableManager::getTableManager();
    tableManager->deleteAllGraphData();
    tableManager->deleteAllTables();


    /***/
    datamanager->setDataModelCreated(false);

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
    foreach(QString file, this->rundata->getFileList() ) {
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
    foreach(QString file, this->rundata->getFileList() ) {
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
    this->selectWindow->addSamples(this->rundata->getFileList());
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

    files = this->rundata->getFileList();
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
    const QString pathwaysTable = OUTPUTPATH + "/" + sampleName + "/results/pgdb/" + pgdbname + ".pathway.txt";

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
    headers.clear();
    headers << "COG Category" << "COG Category (Alias) " << "ORF Count";
    htable = this->getHTableData(sampleName, COG, types, headers,  datamanager);
    htable->addSampleName(sampleName);
    htable->setMultiSampleMode(false);
    resultTabs->addTab(htable, "COG");

    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "KEGG Function" << "KEGG function (alias) " << "ORF Count";
    htable = this->getHTableData(sampleName, KEGG, types, headers, datamanager);
    htable->addSampleName(sampleName);
    htable->setMultiSampleMode(false);
    resultTabs->addTab(htable, "KEGG");

    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "Pathway/Reaction" << "Common name" << "ORF Count";
    htable = this->getHTableData(sampleName, METACYC, types, headers, datamanager);
    htable->addSampleName(sampleName);
    htable->setMultiSampleMode(false);
    resultTabs->addTab(htable, "METACYC");


    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "SEED Subsystem Category" << "SEED Subsystem (Alias) " << "ORF Count";
    htable = this->getHTableData(sampleName, SEED, types, headers, datamanager);
    htable->addSampleName(sampleName);
    htable->setMultiSampleMode(false);
    resultTabs->addTab(htable, "SEED");

#ifdef MEGAN_VIEW
    MeganView *m = this->meganviews["MEGAN_TAXONOMIC"];
    m->clearExistingTree();
    m->setDataFromFile(samplercmgr->getFilePath(sampleName, MEGANTREE));
    resultTabs->addTab(m, "TAXONOMIC");
#endif


    TableData *func_tax_table;
    //FUNCTION AND TAXONOMIC TABLE
    DisplayInfo *p = displayInfos["FUNC & TAX"];
    p->removeFileIndexes();
    FileIndex *fileIndex;
    fileIndex = samplercmgr->getFileIndex(sampleName, NUCFASTA);

    p->addFileIndex(fileIndex,4);
    fileIndex = samplercmgr->getFileIndex(sampleName, AMINOFAA);
    p->addFileIndex(fileIndex,0);

    types.clear();
    types << STRING << INT << INT << INT<<  STRING << INT << STRING << STRING << STRING << STRING;
    func_tax_table = this->tables["FUNC & TAX"];
    func_tax_table->setSampleName(sampleName);
    func_tax_table->setParameters(false, samplercmgr->getFilePath(sampleName, FUNCTIONALTABLE), types, true);
    func_tax_table->setPopupListener(p);
    resultTabs->addTab(func_tax_table, "FUNC & TAX");


    progressbar.hide();




 //   if (nucStats.exists()) resultTabs->addTab(new TableData(true, true, nucFile, types), "CONT LEN TAB");

   // if (aminoStats.exists()) resultTabs->addTab(new TableData(true, true, aminoFile, types), "CONT LEN TAB");
//    GraphData *g;
//    g = graphs["CONT LEN HIST"];

//    g->setFile(samplercmgr->getFilePath(sampleName,  CONTIGLENGTH));
//    g->prepareInput();
//    resultTabs->addTab(g,"CONT LEN HIST");

//    g  = graphs["ORF LEN HIST"];
//    g->setFile(samplercmgr->getFilePath(sampleName, ORFLENGTH));
//    g->prepareInput();
//    resultTabs->addTab(g,"ORF LEN HIST");

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

HTableData *ResultWindow::getHTableData(QString sampleName, ATTRTYPE attr,  QList<enum TYPE> types, QStringList headers,  DataManager *datamanager) {
    HTableData *htable = CreateWidgets::getHtableData(attr);

    htable->clearConnectors();
    Connector *connect = datamanager->createConnector(sampleName, datamanager->getHTree(attr), attr, datamanager->getORFList(sampleName));

    htable->setParameters(datamanager->getHTree(attr), types);

    htable->addConnector(connect);

    htable->setMaxSpinBoxDepth(datamanager->getHTree(attr)->getTreeDepth());

    htable->addSampleName(sampleName);
    htable->setMultiSampleMode(false);
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
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
    for(unsigned int i=0; i < files.size(); i++) {
        if(this->selectedSamples[i])  numSamplesToLoad++;
    }
    ProgressView *progressbar = new ProgressView("creating ORFs for samples ", 0, numSamplesToLoad, this);
   // QProgressDialog progressdialog("linking ORFs to functional categories", "Cancel", 0, 100, this);
    progressbar->show();

    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue;
        orfTableName = samplercmgr->getFilePath(files[i], ORFTABLE);        
        datamanager->createORFs(files[i], orfTableName);
        orfMetaCycTableName = samplercmgr->getFilePath(files[i], ORFMETACYC);
        datamanager->addNewAnnotationToORFs(files[i], orfMetaCycTableName);
        progressbar->updateprogress(i+1);
        qApp->processEvents();
        progressbar->update();
    }
    progressbar->hide();
    delete progressbar;




    HTableData *htable = new HTableData;
    types.clear();
    types << STRING << STRING;
    headers.clear();
    headers << "COG Category" << "COG Category (Alias)";
    htable->clearConnectors();

    progressbar = new ProgressView("linking ORFs  to COG categories ", 0, 0, this);
    progressbar->show();
    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue; 
       connect = datamanager->createConnector(files[i], datamanager->getHTree(COG), COG, datamanager->getORFList(files[i]) );
       htable->addConnector(connect);
       headers << files[i];
       htable->addSampleName(files[i]);
       types << INT;
    }
    progressbar->hide();
    delete progressbar;


   QStringList selectedFileNames;
   for(unsigned int i=0; i < files.size(); i++) {
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


    htable->setParameters(datamanager->getHTree(COG),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(COG)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity("COG", COG);
    htable->showTableData();
    htable->setMultiSampleMode(true);
    resultTabs->addTab(htable, "COG");

    htable = CreateWidgets::getHtableData(KEGG); //new HTableData;
    types.clear();
    types << STRING << STRING;
    headers.clear();
    headers << "KEGG Function" << "KEGG function (alias)" ;
    htable->clearConnectors();
    progressbar = new ProgressView("linking ORFs  to KEGG categories ", 0, 0, this);
    progressbar->show();
    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(KEGG), KEGG, datamanager->getORFList(files[i]));
       htable->addConnector(connect);
       headers << files[i];
       htable->addSampleName(files[i]);
       types << INT;
    }
    progressbar->hide();
    delete progressbar;

    htable->setParameters(datamanager->getHTree(KEGG),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(KEGG)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity("KEGG", KEGG);
    htable->showTableData();
    htable->setMultiSampleMode(true);
    resultTabs->addTab(htable, "KEGG");

    // METACYC
    htable = CreateWidgets::getHtableData(METACYC); //new HTableData;
    types.clear();
    types << STRING << STRING;
    headers.clear();
    headers << "Pathway/Reaction" << "Common name" ;
    htable->clearConnectors();

    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(METACYC), METACYC, datamanager->getORFList(files[i]));
       htable->addConnector(connect);
       headers << files[i];
       htable->addSampleName(files[i]);
       types << INT;
    }
    htable->setParameters(datamanager->getHTree(METACYC),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(METACYC)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity("METACYC", METACYC);
    htable->showTableData();
    htable->setMultiSampleMode(true);
    resultTabs->addTab(htable, "METACYC");


    // SEED
    htable = CreateWidgets::getHtableData(SEED);;
    types.clear();
    types << STRING << STRING;
    headers.clear();
    headers << "SEED Subsystem Category" << "SEED Subsystem (Alias) ";
    htable->clearConnectors();
    progressbar = new ProgressView("linking ORFs  to SEED subsystems", 0, 0, this);
    progressbar->show();
    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(SEED), SEED, datamanager->getORFList(files[i]) );
       htable->addConnector(connect);
       headers << files[i];
       htable->addSampleName(files[i]);
       types << INT;
    }
    progressbar->hide();
    delete progressbar;
    htable->setParameters(datamanager->getHTree(SEED),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(SEED)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity("SEED", SEED);
    htable->showTableData();
    htable->setMultiSampleMode(true);
    resultTabs->addTab(htable, "SEED");


}

RunData* ResultWindow::getRunData(){
    return this->rundata;
}


ResultWindow::~ResultWindow()
{
    delete ui;
}


