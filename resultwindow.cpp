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
    resultTabs->clear();

    getFileNames = new QTimer();
    getFileNames->start(500);

    connect(sampleSelect,SIGNAL(currentIndexChanged(QString)),this,SLOT(sampleChanged(QString)));
    connect(getFileNames, SIGNAL(timeout()),this,SLOT(updateFileNames()));
    connect(this,SIGNAL(fileChanged(QString)),this->progress,SLOT(selectedFileChanged(QString)));

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




void ResultWindow::sampleChanged(QString changed){
    QString METAPATH = this->run->getConfig()->operator []("METAPATHWAYS_PATH");
    QString OUTPUTPATH = this->run->getParams()->operator []("folderOutput");

    qDebug() << "Input path " << METAPATH;
    const QString nucFile = OUTPUTPATH + "/" + changed + "/run_statistics/" + changed + ".nuc.stats";
    const QString aminoFile = OUTPUTPATH + "/" + changed + "/run_statistics/" + changed + ".amino.stats";

    const QString contigLengthFile = OUTPUTPATH + "/" + changed + "/run_statistics/" + changed + ".contig.lengths.txt";
    const QString orfLengthFile = OUTPUTPATH + "/" + changed + "/run_statistics/" + changed + ".orf.lengths.txt";

    const QString nucFasta = OUTPUTPATH  +  "/" + changed + "/preprocessed/" + changed + ".fasta";
    const QString aminoFasta = OUTPUTPATH + "/" + changed + "/orf_prediction/" + changed + ".faa";
    const QString funAndTax = OUTPUTPATH + "/" + changed + "/results/annotation_table/" + "functional_and_taxonomic_table.txt";

    const QString meganTre = OUTPUTPATH + "/" + changed + "/results/annotation_table/" + "megan_tree.tre";
    const QString functionalSource1 = OUTPUTPATH + "/" + changed + "/results/annotation_table/" + changed + ".1.txt";

    resultTabs->clear();
    for (unsigned int i=0; i< resultTabs->count(); i++){
        delete resultTabs->widget(i);
    }

    QList<enum TYPE> types;
    types.clear();
    types << INT << INT << DOUBLE << INT;


 //   if (nucStats.exists()) resultTabs->addTab(new TableData(true, true, nucFile, types), "CONT LEN TAB");

   // if (aminoStats.exists()) resultTabs->addTab(new TableData(true, true, aminoFile, types), "CONT LEN TAB");
    GraphData *g;
    g = graphs["CONT LEN HIST"];

    qDebug() << " contig length file " << contigLengthFile;
    g->setFile(contigLengthFile);
    g->prepareInput();
    resultTabs->addTab(g,"CONT LEN HIST");

    g  = graphs["ORF LEN HIST"];
    g->setFile(orfLengthFile);
    g->prepareInput();
    resultTabs->addTab(g,"ORF LEN HIST");


    DisplayInfo *p = displayInfos["FUNC & TAX"];
    p->removeFileIndexes();
    FileIndexManager *indexManager = FileIndexManager::getFileIndexManager();


    FileIndex *fileIndex = indexManager->getFileIndex(nucFasta, FASTA);    
    p->addFileIndex(fileIndex,4);
    fileIndex = indexManager->getFileIndex(aminoFasta, FASTA);
    p->addFileIndex(fileIndex,0);

    types.clear();
    types << STRING << INT << INT << INT<<  STRING << INT << STRING << STRING << STRING << STRING;
    TableData *t = this->tables["FUNC & TAX"];
    t->setParameters(false, funAndTax, types, true);
    t->setPopupListener(p);
    resultTabs->addTab(t, "FUNC & TAX");


    MeganView *m = this->meganviews["MEGAN_TAXONOMIC"];
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



    // add the annotation sources table KEGG 1
    const QString functionalKegg1 = OUTPUTPATH + "/" + changed + "/results/annotation_table/" + "KEGG_stats_1.txt";
    types.clear();
    types << STRING  <<  INT;
    t=  this->tables["FUNC KEGG 1"];
    t->setParameters(false, functionalKegg1, types, true);
  //  t =  new TableData(true, false, functionalKegg1, types);
    resultTabs->addTab(t, "FUNC KEGG 1");

    // add the annotation sources table KEGG 2
    const QString functionalKegg2 = OUTPUTPATH + "/" + changed + "/results/annotation_table/" + "KEGG_stats_2.txt";
    types.clear();
    types << STRING << STRING <<  INT;
    t=  this->tables["FUNC KEGG 2"];
    t->setParameters(false, functionalKegg2, types, true);
    resultTabs->addTab(t, "FUNC KEGG 2");


    const QString functionalKegg3 = OUTPUTPATH + "/" + changed + "/results/annotation_table/" + "KEGG_stats_3.txt";
    types.clear();
    types << STRING << STRING <<  INT;
    t=  this->tables["FUNC KEGG 3"];
    t->setParameters(false, functionalKegg3, types, true);
    resultTabs->addTab(t, "FUNC KEGG 3");


    const QString functionalKegg4 = OUTPUTPATH + "/" + changed + "/results/annotation_table/" + "KEGG_stats_4.txt";
    types.clear();
    types << STRING << STRING <<  INT;
    t=  this->tables["FUNC KEGG 4"];
    t->setParameters(false, functionalKegg4, types, true);
    resultTabs->addTab(t, "FUNC KEGG 4");

    // add the annotation sources table COG 1
    const QString functionalCOG1 = OUTPUTPATH + "/" + changed + "/results/annotation_table/" + "COG_stats_1.txt";
    types.clear();
    types << STRING  <<  INT;
    t =  this->tables["FUNC COG 1"];
    t->setParameters(true, functionalCOG1, types, true);
    resultTabs->addTab(t, "FUNC COG 1");

    // add the annotation sources table COG 2
    const QString functionalCOG2 = OUTPUTPATH + "/" + changed + "/results/annotation_table/" + "COG_stats_2.txt";
    types.clear();
    types << STRING  << STRING <<  INT;
    t =  this->tables["FUNC COG 2"];
    t->setParameters(true, functionalCOG2, types, true);
    resultTabs->addTab(t, "FUNC COG 2");

    // add the annotation sources table COG 3
    const QString functionalCOG3 = OUTPUTPATH + "/" + changed + "/results/annotation_table/" + "COG_stats_3.txt";
    types.clear();
    types << STRING  <<  STRING << INT;
    t =  this->tables["FUNC COG 3"];
    t->setParameters(true, functionalCOG3, types, true);
    resultTabs->addTab(t, "FUNC COG 3");

    emit fileChanged(changed);
}

RunData* ResultWindow::getRunData(){
    return this->run;
}


ResultWindow::~ResultWindow()
{
    delete ui;
}
