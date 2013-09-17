#include "resultwindow.h"


ResultWindow::ResultWindow(ProgressDialog *prog, RunData *run, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MetaPathways - Results and Data");
    this->run = run;
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


    graphicsRepresentation["MEGAN"] = new GraphicsRepresentation();



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

    qDebug() << "Input path " << METAPATH;
    const QString nucFile = METAPATH + "/output/" + changed + "/run_statistics/" + changed + ".nuc.stats";
    const QString aminoFile = METAPATH + "/output/" + changed + "/run_statistics/" + changed + ".amino.stats";
    const QString funcFile = METAPATH + "/output/" + changed + "/results/annotation_table" + "/functional_and_taxonomic_table.txt";
    const QString nucFasta = METAPATH + "/output/" + changed + "/preprocessed/" + changed + ".fasta";
    const QString aminoFasta = METAPATH + "/output/" + changed + "/orf_prediction/" + changed + ".faa";
    const QString funAndTax = METAPATH + "/output/" + changed + "/results/annotation_table/" + "functional_and_taxonomic_table.txt";
    const QString functionalSource1 = METAPATH + "/output/" + changed + "/results/annotation_table/" + changed + ".1.txt";



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

    g->setFile("/Users/michaelwu/workspace/MetaPathways2-build-Desktop-Release/MetaPathways2.app/Contents/MacOS/contig_lengths.txt");
    g->prepareInput();
    resultTabs->addTab(g,"CONT LEN HIST");

    g  = graphs["ORF LEN HIST"];
    g->setFile("/Users/michaelwu/workspace/MetaPathways2-build-Desktop-Release/MetaPathways2.app/Contents/MacOS/orf_lengths.txt");
    g->prepareInput();
    resultTabs->addTab(g,"ORF LEN HIST");


    DisplayInfo *p = displayInfos["FUNC & TAX"];
    p->removeFileIndexes();
    FileIndexManager *indexManager = FileIndexManager::getFileIndexManager();
    FileIndex *fileIndex = indexManager->getFileIndex(nucFasta, FASTA);    
    p->addFileIndex(fileIndex,1);
    fileIndex = indexManager->getFileIndex(aminoFasta, FASTA);
    p->addFileIndex(fileIndex,0);

    types.clear();
    types << STRING << STRING << INT << INT << STRING << STRING << STRING << STRING;
    TableData *t = this->tables["FUNC & TAX"];
    t->setParameters(false, funAndTax, types, true);
    t->setPopupListener(p);
    resultTabs->addTab(t, "FUNC & TAX");


    resultTabs->addTab(this->graphicsRepresentation["MEGAN"], "MEGAN");

    return;

    // add the annotation sources table
    types.clear();
    types << STRING << STRING << STRING << STRING << DOUBLE;
  //  t =  new TableData(true, false, functionalSource1, types);
    p = new DisplayInfo();
    t->setPopupListener(p);
    resultTabs->addTab(t, "FUNC SRC");
    fileIndex = indexManager->getFileIndex(aminoFasta, FASTA);
    p->addFileIndex(fileIndex,0);


    // add the annotation sources table KEGG 1
     const QString functionalKegg1 = METAPATH + "/output/" + changed + "/results/annotation_table/" + "KEGG_stats_1.txt";
    types.clear();
    types << STRING << STRING <<  INT;
  //  t =  new TableData(true, false, functionalKegg1, types);
    resultTabs->addTab(t, "FUNC KEGG 1");

    // add the annotation sources table KEGG 2
    const QString functionalKegg2 = METAPATH + "/output/" + changed + "/results/annotation_table/" + "KEGG_stats_2.txt";
    types.clear();
    types << STRING << STRING <<  INT;
   // t =  new TableData(true, false, functionalKegg2, types);
    resultTabs->addTab(t, "FUNC KEGG 2");

    // add the annotation sources table KEGG 3
    const QString functionalKegg3 = METAPATH + "/output/" + changed + "/results/annotation_table/" + "KEGG_stats_4.txt";
    types.clear();
    types << STRING << STRING <<  INT;
  //  t =  new TableData(true, false, functionalKegg3, types);
    resultTabs->addTab(t, "FUNC KEGG 3");

    // add the annotation sources table KEGG 4
    const QString functionalKegg4 = METAPATH + "/output/" + changed + "/results/annotation_table/" + "KEGG_stats_4.txt";
    types.clear();
    types << STRING << STRING <<  INT;
  //  t =  new TableData(true, false, functionalKegg4, types);
    resultTabs->addTab(t, "FUNC KEGG 4");

    // add the annotation sources table COG 1
     const QString functionalCOG1 = METAPATH + "/output/" + changed + "/results/annotation_table/" + "COG_stats_1.txt";
    types.clear();
    types << STRING << STRING <<  INT;
   // t =  new TableData(true, false, functionalCOG1, types);
    resultTabs->addTab(t, "FUNC COG 1");

    // add the annotation sources table COG 2
     const QString functionalCOG2 = METAPATH + "/output/" + changed + "/results/annotation_table/" + "COG_stats_2.txt";
    types.clear();
    types << STRING << STRING <<  INT;
  //  t =  new TableData(true, false, functionalCOG2, types);
    resultTabs->addTab(t, "FUNC COG 2");

    // add the annotation sources table COG 3
    const QString functionalCOG3 = METAPATH + "/output/" + changed + "/results/annotation_table/" + "COG_stats_3.txt";
    types.clear();
    types << STRING << STRING <<  INT;
  //  t =  new TableData(true, false, functionalCOG3, types);
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
