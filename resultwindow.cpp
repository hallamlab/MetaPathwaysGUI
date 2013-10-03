#include "resultwindow.h"


ResultWindow::ResultWindow(ProgressDialog *prog, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MetaPathways - Results and Data");
    this->run = RunData::getRunData();
    this->progress = prog;

    resultTabs = this->findChild<QTabWidget *>("resultTabs");
    sampleSelect = this->findChild<QComboBox *>("sampleSelect");
    checkComparativeMode = this->findChild<QCheckBox *>("checkComparativeMode");
    selectSamplesButton = this->findChild<QPushButton *>("selectSamplesButton");
    currentSampleLabel = this->findChild<QLabel *>("currentSampleLabel");
    selectSamplesButton->setEnabled(false);


    resultTabs->clear();

    getFileNames = new QTimer();
    getFileNames->start(500);

    connect(sampleSelect,SIGNAL(currentIndexChanged(QString)),this,SLOT(sampleChanged(QString)));
    connect(getFileNames, SIGNAL(timeout()),this,SLOT(updateFileNames()));
    connect(this,SIGNAL(fileChanged(QString)),this->progress,SLOT(selectedFileChanged(QString)));
    connect(checkComparativeMode, SIGNAL(stateChanged(int)), this, SLOT(setVisible(int)) );
    connect(selectSamplesButton, SIGNAL(clicked()), this, SLOT(clickedSelectSample()  ) );

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
    files = *this->run->files;

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
    TableData *t;

    QString METAPATH = this->run->getConfig()->operator []("METAPATHWAYS_PATH");
    QString OUTPUTPATH = this->run->getParams()->operator []("folderOutput");

    const QString nucFile = this->getFilePath(sampleName, OUTPUTPATH, NUCSTATS);
    const QString aminoFile = this->getFilePath(sampleName, OUTPUTPATH, AMINOSTATS);

    const QString contigLengthFile = this->getFilePath(sampleName, OUTPUTPATH, CONTIGLENGTH);

    const QString orfLengthFile = this->getFilePath(sampleName, OUTPUTPATH,ORFLENGTH);


    const QString nucFasta = this->getFilePath(sampleName, OUTPUTPATH, NUCFASTA);

    const QString aminoFasta = this->getFilePath(sampleName, OUTPUTPATH, AMINOFAA);

    const QString funAndTax = this->getFilePath(sampleName, OUTPUTPATH, FUNCTIONALTABLE);



    const QString meganTre = this->getFilePath(sampleName, OUTPUTPATH, MEGANTREE);


    const QString functionalSource1 = this->getFilePath(sampleName, OUTPUTPATH, FUNCTIONAL_SRC1);


    QString orfTableName = this->getFilePath(sampleName, OUTPUTPATH, ORFTABLE);


    const QString orfMetaCycTableName = this->getFilePath(sampleName, OUTPUTPATH, ORFMETACYC);

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

    DataManager *datamanager = DataManager::getDataManager();


    orfTableName = this->getFilePath(sampleName, OUTPUTPATH, ORFTABLE);
    datamanager->createORFs(sampleName, orfTableName);
    datamanager->addNewAnnotationToORFs(sampleName, orfMetaCycTableName);



    Connector *connect;


    HTableData *htable = new HTableData;
    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "KEGG Function" << "KEGG function (alias) " << "ORF Count";
    htable->clearConnectors();
    connect = datamanager->createConnector(sampleName, datamanager->getHTree(KEGG), KEGG);
    htable->setParameters(datamanager->getHTree(KEGG), types);
    htable->addConnector(connect);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(KEGG)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity(sampleName,KEGG);
    htable->showTableData();
    resultTabs->addTab(htable, "KEGG");



    htable = new HTableData;
    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "COG Category" << "COG Category (Alias) " << "ORF Count";
    htable->clearConnectors();
    connect = datamanager->createConnector(sampleName, datamanager->getHTree(COG), COG);
    htable->setParameters(datamanager->getHTree(COG),  types);
    htable->addConnector(connect);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(COG)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity(sampleName,COG);
    htable->showTableData();
    resultTabs->addTab(htable, "COG");


    htable = new HTableData;
    types.clear();
    types << STRING << STRING << INT;
    headers.clear();
    headers << "Pathway/Reaction" << "Common name" << "ORF Count";
    connect = datamanager->createConnector(sampleName, datamanager->getHTree(METACYC), METACYC);
    htable->setParameters(datamanager->getHTree(METACYC),  types);
    htable->addConnector(connect);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(METACYC)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity(sampleName,METACYC);
    htable->showTableData();
    resultTabs->addTab(htable, "METACYC");

/*
    DisplayInfo *p = displayInfos["FUNC & TAX"];
    p->removeFileIndexes();
    FileIndexManager *indexManager = FileIndexManager::getFileIndexManager();


    FileIndex *fileIndex = indexManager->getFileIndex(nucFasta, FASTA);
    p->addFileIndex(fileIndex,4);
    fileIndex = indexManager->getFileIndex(aminoFasta, FASTA);
    p->addFileIndex(fileIndex,0);

    types.clear();
    types << STRING << INT << INT << INT<<  STRING << INT << STRING << STRING << STRING << STRING;
    t = this->tables["FUNC & TAX"];
    t->setParameters(false, funAndTax, types, true);
    t->setPopupListener(p);
    resultTabs->addTab(t, "FUNC & TAX");
*/


    MeganView *m = this->meganviews["MEGAN_TAXONOMIC"];
    m->clearExistingTree();
    m->setDataFromFile(meganTre);
    resultTabs->addTab(m, "TAXONOMIC");
    return;

    types.clear();
    types << INT << INT << DOUBLE << INT;


 //   if (nucStats.exists()) resultTabs->addTab(new TableData(true, true, nucFile, types), "CONT LEN TAB");

   // if (aminoStats.exists()) resultTabs->addTab(new TableData(true, true, aminoFile, types), "CONT LEN TAB");
    GraphData *g;
    g = graphs["CONT LEN HIST"];

    g->setFile(contigLengthFile);
    g->prepareInput();
    resultTabs->addTab(g,"CONT LEN HIST");

    g  = graphs["ORF LEN HIST"];
    g->setFile(orfLengthFile);
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

/*

    //FUNCTION AND TAXONOMIC TABLE
    DisplayInfo *p = displayInfos["FUNC & TAX"];
    p->removeFileIndexes();
    FileIndexManager *indexManager = FileIndexManager::getFileIndexManager();


    FileIndex *fileIndex = indexManager->getFileIndex(nucFasta, FASTA);    
    p->addFileIndex(fileIndex,4);
    fileIndex = indexManager->getFileIndex(aminoFasta, FASTA);
    p->addFileIndex(fileIndex,0);

    types.clear();
    types << STRING << INT << INT << INT<<  STRING << INT << STRING << STRING << STRING << STRING;
    t = this->tables["FUNC & TAX"];
    t->setParameters(false, funAndTax, types, true);
    t->setPopupListener(p);
    resultTabs->addTab(t, "FUNC & TAX");
*/



    // MEGAN VIEW


    m = this->meganviews["MEGAN_TAXONOMIC"];
    m->clearExistingTree();
    m->setDataFromFile(meganTre);
    resultTabs->addTab(m, "TAXONOMIC");


    //resultTabs->addTab(this->graphicsRepresentation["MEGAN"], "MEGAN");

    // add the annotation sources table
    types.clear();
    types << STRING << STRING << STRING << STRING << DOUBLE;
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

QString ResultWindow::getFilePath(QString sampleName, QString OUTPUTPATH, INPUTFILETYPE type) {
    QString path;
    switch(type) {
        case NUCSTATS:
           path = OUTPUTPATH + "/" + sampleName + "/run_statistics/" + sampleName + ".nuc.stats";
           break;
        case AMINOSTATS:
           path = OUTPUTPATH + "/" + sampleName + "/run_statistics/" + sampleName + ".amino.stats";
           break;
        case CONTIGLENGTH:
           path = OUTPUTPATH + "/" + sampleName + "/run_statistics/" + sampleName + ".contig.lengths.txt";
           break;
        case ORFLENGTH:
           path = OUTPUTPATH + "/" + sampleName + "/run_statistics/" + sampleName + ".orf.lengths.txt";
           break;
        case NUCFASTA:
           path = OUTPUTPATH  +  "/" + sampleName + "/preprocessed/" + sampleName + ".fasta";
           break;
        case AMINOFAA:
           path = OUTPUTPATH + "/" + sampleName + "/orf_prediction/" + sampleName + ".faa";
           break;
        case FUNCTIONALTABLE:
           path = OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + "functional_and_taxonomic_table.txt";
           break;
        case MEGANTREE:
           path = OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + "megan_tree.tre";
           break;
        case FUNCTIONAL_SRC1:
           path = OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + sampleName + ".1.txt";
           break;
        case ORFTABLE:
           path = OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + "ORF_annotation_table.txt";
           break;
        case ORFMETACYC:
           path =OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + "ORF_annotation_metacyc_table.txt";
           break;
        default:
           path = "";
           break;
    }
    return path;

}

void ResultWindow::switchToComparativeMode() {
    resultTabs->clear();
    QString METAPATH = this->run->getConfig()->operator []("METAPATHWAYS_PATH");
    QString OUTPUTPATH = this->run->getParams()->operator []("folderOutput");

    QString orfTableName;
    QString orfMetaCycTableName;

    qDebug() << "comparative mode";

    QList<enum TYPE> types;
    QList<unsigned int> columns;

    QStringList headers;

    DataManager *datamanager = DataManager::getDataManager();
   //// datamanager->createORFs(changed, orfTableName);

   //// datamanager->addNewAnnotationToORFs(changed, orfMetaCycTableName);

    Connector *connect;
    ///connect = datamanager->createConnector(changed, datamanager->getHTree(KEGG), KEGG);

    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue;
        orfTableName = this->getFilePath(files[i], OUTPUTPATH, ORFTABLE);
        datamanager->createORFs(files[i], orfTableName);
        orfMetaCycTableName = this->getFilePath(files[i], OUTPUTPATH, ORFMETACYC);
        datamanager->addNewAnnotationToORFs(files[i], orfMetaCycTableName);
    }


    HTableData *htable = new HTableData;
    types.clear();
    types << STRING << STRING;
    headers.clear();
    headers << "COG Category" << "COG Category (Alias) ";
    htable->clearConnectors();

    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(COG), COG);
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
    headers << "KEGG Function" << "KEGG function (alias) " ;
    htable->clearConnectors();

    for(unsigned int i=0; i < files.size(); i++) {
       if( !this->selectedSamples[i])  continue;
       connect = datamanager->createConnector(files[i], datamanager->getHTree(KEGG), KEGG);
       htable->addConnector(connect);
       headers << files[i];
       types << INT;
    }
    htable->setParameters(datamanager->getHTree(KEGG),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(KEGG)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity("KEGG",KEGG);
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
       connect = datamanager->createConnector(files[i], datamanager->getHTree(METACYC), METACYC);
       htable->addConnector(connect);
       headers << files[i];
       types << INT;
    }
    htable->setParameters(datamanager->getHTree(METACYC),  types);
    htable->setMaxSpinBoxDepth(datamanager->getHTree(METACYC)->getTreeDepth());
    htable->setShowHierarchy(true);
    htable->setHeaders(headers);
    htable->setTableIdentity("METACYC",METACYC);
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


