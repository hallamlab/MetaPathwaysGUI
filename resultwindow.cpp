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


    this->run = RunData::getRunData();

    qDebug() << "rundata";
    qDebug() << run->getConfig();
    qDebug() << run->getParams();

    if( this->run->getConfig().isEmpty() || this->run->getParams().isEmpty()  ) {
        QMessageBox warning;
        warning.warning(0, "Configuration Invalid!","Ooops! \nYou appear to have a configuration file for me, but parameters may be missing! Please check that all required executables and folders are specified through setup.", QMessageBox::Ok);
        return ;
    }


    resultTabs = this->findChild<QTabWidget *>("resultTabs");
    sampleSelect = this->findChild<QComboBox *>("sampleSelect");
    checkComparativeMode = this->findChild<QCheckBox *>("checkComparativeMode");
    selectSamplesButton = this->findChild<QPushButton *>("selectSamplesButton");
    currentSampleLabel = this->findChild<QLabel *>("currentSampleLabel");
    loadResults = this->findChild<QPushButton *>("loadResults");

    selectSamplesButton->setEnabled(false);


    resultTabs->clear();
    getFileNames = new QTimer();
    getFileNames->start(500);

    connect(sampleSelect,SIGNAL(currentIndexChanged(QString)),this,SLOT(sampleChanged(QString)));
    connect(getFileNames, SIGNAL(timeout()),this,SLOT(updateFileNames()));
    //connect(this,SIGNAL(fileChanged(QString)),this->progress,SLOT(selectedFileChanged(QString)));
    connect(checkComparativeMode, SIGNAL(stateChanged(int)), this, SLOT(setVisible(int)) );
    connect(selectSamplesButton, SIGNAL(clicked()), this, SLOT(clickedSelectSample()  ) );
    connect(loadResults, SIGNAL(clicked()), this, SLOT(_loadResults()) );

    tables["PATHWAYS"] = new TableData();
    tables["REACTIONS"] = new TableData();
    tables["FUNC & TAX"] = new TableData();
    tables["FUNC SRC"] = new TableData();
    tables["FUNC KEGG 1"] = new TableData();
    tables["FUNC KEGG 2"] = new TableData();
    tables["FUNC KEGG 3"] = new TableData();
    tables["FUNC KEGG 4"] = new TableData();
    tables["FUNC COG 1"] = new TableData();
    tables["FUNC COG 2"] = new TableData();
    tables["FUNC COG 3"] = new TableData();

    displayInfos["FUNC & TAX"] = new DisplayInfo();
    displayInfos["FUNC SRC"] = new DisplayInfo();
    displayInfos["FUNC KEGG 1"] = new DisplayInfo();
    displayInfos["FUNC KEGG 2"] = new DisplayInfo();
    displayInfos["FUNC KEGG 3"] = new DisplayInfo();
    displayInfos["FUNC KEGG 4"] = new DisplayInfo();
    displayInfos["FUNC COG 1"] = new DisplayInfo();
    displayInfos["FUNC COG 2"] = new DisplayInfo();
    displayInfos["FUNC COG 3"] = new DisplayInfo();

    graphs["CONT LEN HIST"] = new GraphData();
    graphs["ORF LEN HIST"] = new GraphData();

    meganviews["MEGAN_TAXONOMIC"]= new MeganView();


   // graphicsRepresentation["MEGAN"] = new GraphicsRepresentation();

    DataManager *datamanager = DataManager::getDataManager();
    datamanager->createDataModel();
}

void ResultWindow::_loadResults() {
    this->disableSampleChanged = true;
    this->updateFileNames();
    this->disableSampleChanged = false;
}


void ResultWindow::indexSamples(bool userResourceFolder) {
    QLabel *waitScreen = Utilities::ShowWaitScreen("Please wait while we index the FASTA files!");
    QProgressBar progressBar;
    progressBar.setRange(0, files.size());
    progressBar.setWindowFlags(Qt::WindowStaysOnTopHint);
    progressBar.show();

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
    samplercmgr->setUseResourceFolder(userResourceFolder);

    unsigned int i =0;
    foreach(QString file, files ) {
        samplercmgr->getFileIndex(file, NUCFASTA);
        samplercmgr->getFileIndex(file, AMINOFAA);
        progressBar.setValue(++i); qApp->processEvents();  progressBar.update();
    }
    waitScreen->hide();
    progressBar.hide();
}

void ResultWindow::clickedSelectSample(){
    this->selectWindow = new SelectSamples;
    this->selectWindow->setReceiver(this);
    this->selectWindow->addSamples(files);
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
    files = this->run->files;
    QStringList existing;

    for (int i=0;i<sampleSelect->count();i++){
        existing.append(sampleSelect->itemText(i));
    }

    foreach (QString f, files){
        if (!existing.contains(f)){
            sampleSelect->addItem(f);
        }
    }
}

 void ResultWindow::receiveSelection(QList<bool> &selection) {
     this->selectedSamples = selection;

     this->switchToComparativeMode();

 }


void ResultWindow::sampleChanged(QString sampleName){

    if( disableSampleChanged==true) return;

    TableData *t;

    QString OUTPUTPATH = this->run->getParams()["folderOutput"];

    qDebug() << "before data model";
    DataManager *datamanager = DataManager::getDataManager();
    qDebug() << "after dtamanager ";
    datamanager->createDataModel();


   // const QString meganTre = this->getFilePath(sampleName, OUTPUTPATH, MEGANTREE);

    SampleResourceManager *sampleResourceManager = SampleResourceManager::getSampleResourceManager();
    sampleResourceManager->setOutPutPath(OUTPUTPATH);
    this->indexSamples(true);


    QLabel *waitScreen = Utilities::ShowWaitScreen("Please wait while we load the sample!");
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

    Connector *connect;


    HTableData *htable = new HTableData;
    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "KEGG Function" << "KEGG function (alias) " << "ORF Count";
    htable = this->getHTableData(sampleName, KEGG, types, headers, connect, datamanager);
    resultTabs->addTab(htable, "KEGG");
    qDebug() << "done kegg";

    htable = new HTableData;
    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "COG Category" << "COG Category (Alias) " << "ORF Count";
    htable = this->getHTableData(sampleName, COG, types, headers, connect, datamanager);
    resultTabs->addTab(htable, "COG");


    htable = new HTableData;
    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "Pathway/Reaction" << "Common name" << "ORF Count";
    htable = this->getHTableData(sampleName, METACYC, types, headers, connect, datamanager);
    resultTabs->addTab(htable, "METACYC");


    htable = new HTableData;
    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "SEED Subsystem Category" << "SEED Subsystem (Alias) " << "ORF Count";
    htable = this->getHTableData(sampleName, SEED, types, headers, connect, datamanager);
    resultTabs->addTab(htable, "SEED");

    MeganView *m = this->meganviews["MEGAN_TAXONOMIC"];
    m->clearExistingTree();
    m->setDataFromFile(samplercmgr->getFilePath(sampleName, MEGANTREE));
    resultTabs->addTab(m, "TAXONOMIC");


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
    t = this->tables["FUNC & TAX"];
    t->setParameters(false, samplercmgr->getFilePath(sampleName, FUNCTIONALTABLE), types, true);
    t->setPopupListener(p);
    resultTabs->addTab(t, "FUNC & TAX");


    waitScreen->hide();
    delete waitScreen;

    return;

 //   if (nucStats.exists()) resultTabs->addTab(new TableData(true, true, nucFile, types), "CONT LEN TAB");

   // if (aminoStats.exists()) resultTabs->addTab(new TableData(true, true, aminoFile, types), "CONT LEN TAB");
    GraphData *g;
    g = graphs["CONT LEN HIST"];

    g->setFile(samplercmgr->getFilePath(sampleName,  CONTIGLENGTH));
    g->prepareInput();
    resultTabs->addTab(g,"CONT LEN HIST");

    g  = graphs["ORF LEN HIST"];
    g->setFile(samplercmgr->getFilePath(sampleName, ORFLENGTH));
    g->prepareInput();
    resultTabs->addTab(g,"ORF LEN HIST");

    // PATHWAY TABLE
     types.clear();
     types << STRING << STRING<< INT << INT<< INT;
     columns.clear();
     columns << 1 << 2 << 3 << 4<<  5;

     t = this->tables["PATHWAYS"];
     t->setParameters(false, pathwaysTable, types, columns, false,  QRegExp("^PWY:"));
     resultTabs->addTab(t, "PATHwAYS");

     // REACTION TABLE
     types.clear();
     types << STRING << STRING<< INT;
     columns.clear();
     columns << 1 << 2 << 5;

     t = this->tables["REACTIONS"];
     t->setParameters(false, pathwaysTable, types, columns, false, QRegExp("^RXN:"));
     resultTabs->addTab(t, "REACTIONS");


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

HTableData *ResultWindow::getHTableData(QString sampleName, ATTRTYPE attr,  QList<enum TYPE> types, QStringList headers, Connector *connect, DataManager *datamanager) {
    HTableData *htable = new HTableData;
    htable->clearConnectors();
    connect = datamanager->createConnector(sampleName, datamanager->getHTree(attr), attr, datamanager->getORFList(sampleName));
    htable->setParameters(datamanager->getHTree(attr), types);
    htable->addConnector(connect);

    htable->setMaxSpinBoxDepth(datamanager->getHTree(attr)->getTreeDepth());


    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity(sampleName,attr);
    htable->showTableData();
    return htable;
}


void ResultWindow::switchToComparativeMode() {
    resultTabs->clear();

    QString orfTableName;
    QString orfMetaCycTableName;

    qDebug() << "comparative mode";

    QList<enum TYPE> types;
    QList<unsigned int> columns;

    QStringList headers;

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();

    DataManager *datamanager = DataManager::getDataManager();
   //// datamanager->createORFs(changed, orfTableName);

   //// datamanager->addNewAnnotationToORFs(changed, orfMetaCycTableName);

    Connector *connect;
    ///connect = datamanager->createConnector(changed, datamanager->getHTree(KEGG), KEGG);

    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue;
        orfTableName = samplercmgr->getFilePath(files[i], ORFTABLE);
        datamanager->createORFs(files[i], orfTableName);
        orfMetaCycTableName = samplercmgr->getFilePath(files[i], ORFMETACYC);
        datamanager->addNewAnnotationToORFs(files[i], orfMetaCycTableName);
    }


    HTableData *htable = new HTableData;
    types.clear();
    types << STRING << STRING;
    headers.clear();
    headers << "COG Category" << "COG Category (Alias)";
    htable->clearConnectors();

    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue; 
       connect = datamanager->createConnector(files[i], datamanager->getHTree(COG), COG, datamanager->getORFList(files[i]) );
       htable->addConnector(connect);
       headers << files[i];
       types << INT;
    }

    htable->setParameters(datamanager->getHTree(COG),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(COG)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity("COG", COG);
    htable->showTableData();
    resultTabs->addTab(htable, "COG");

    htable = new HTableData;
    types.clear();
    types << STRING << STRING;
    headers.clear();
    headers << "KEGG Function" << "KEGG function (alias)" ;
    htable->clearConnectors();

    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(KEGG), KEGG, datamanager->getORFList(files[i]));
       htable->addConnector(connect);
       headers << files[i];
       types << INT;
    }
    htable->setParameters(datamanager->getHTree(KEGG),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(KEGG)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity("KEGG", KEGG);
    htable->showTableData();
    resultTabs->addTab(htable, "KEGG");

    // METACYC
    htable = new HTableData;
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
       types << INT;
    }
    htable->setParameters(datamanager->getHTree(METACYC),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(METACYC)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity("METACYC", METACYC);
    htable->showTableData();
    resultTabs->addTab(htable, "METACYC");

}

RunData* ResultWindow::getRunData(){
    return this->run;
}


ResultWindow::~ResultWindow()
{
    delete ui;
}


