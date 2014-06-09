
#include "ui_ProgressDialog.h"

#include <QProcess>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QMovie>
#include <QTextStream>
#include <QDir>

#include "helper/types.h"
#include "helper/utilities.h"
#include "helper/rundata.h"
#include "tabs/resultwindow.h"
#include "tabs/ProgressDialog.h"


/*
 * Default constructor, sets up private variables.
 */
ProgressDialog::ProgressDialog(QWidget *parent) : QWidget(parent), ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    this->rundata = RunData::getRunData();

    this->setWindowTitle("MetaPathways - Run Progress");
    cancelButton = this->findChild<QPushButton *>("cancelButton");
    logBrowser = this->findChild<QTextBrowser *>("logBrowser");
    logLabel = this->findChild<QLabel *>("logLabel");
    summaryTable = this->findChild<QTableWidget *>("summaryTable");
    summaryLabel = this->findChild<QLabel *>("summaryLabel");
    progressBar = this->findChild<QProgressBar *>("progressBar");
    progressLabel = this->findChild<QLabel *>("progressLabel");
    standardOut = this->findChild<QTextEdit *>("standardOut");
    runButton = this->findChild<QPushButton *>("runButton");
    showErrorsButton = this->findChild<QPushButton *>("showErrors");
    sampleSelect = this->findChild<QComboBox *>("sampleSelect");
    runVerbose = this->findChild<QCheckBox *>("runVerboseCheckBox");
    computeStats = this->findChild<QCheckBox *>("computeStats");

    summaryTable->setSortingEnabled(false);

    initMapping();

    timer = new QTimer(this);
    timer->start(1000);

    this->myProcess =0;

    cancelButton->setEnabled(false);

    connect(timer, SIGNAL(timeout()), this, SLOT(readStepsLog()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
    connect(runButton,SIGNAL(clicked()), this, SLOT(startRun()));
    connect(sampleSelect, SIGNAL(activated(QString)), this, SLOT(selectedFileChanged(QString)));
    connect(this->rundata, SIGNAL(loadSampleList()), this, SLOT(loadSampleList()));
    connect(this->showErrorsButton, SIGNAL(clicked()), this, SLOT(showErrors()));
}

void ProgressDialog::showErrors() {
  /* OLD WAY StatusView *statusview = StatusView::getStatusView();
   statusview->showTreeView();
   */
    RunData *rundata= RunData::getRunData();

    TreeModel *model = new TreeModel;
    model->setSampleNames(rundata->getFileList());
    model->readFiles();

    QTreeView *view = new QTreeView;
    view->setModel(model);
    view->expandAll();
    view->show();
    view->resizeColumnToContents(0);
    view->resizeColumnToContents(1);
    view->resizeColumnToContents(2);


}
void ProgressDialog::loadSampleList() {

    QStringList files = this->rundata->getFileList();

    sampleSelect->clear();
    foreach (QString f, files){
        sampleSelect->addItem(f); // sampleselect lets the user monitor the status of multiple samples
    }
    rundata->setCurrentSample(sampleSelect->currentText());

}

bool ProgressDialog::checkInputOutPutLocations() {
    QFileInfo input(this->rundata->getParams()["fileInput"]);
    QFileInfo output(this->rundata->getParams()["folderOutput"]);
    QFileInfo logFile(this->rundata->getParams()["folderOutput"]+ "/" + rundata->getCurrentSample() + "/metapathways_steps_log.txt");

    if( !input.exists() || !output.exists() || !logFile.exists() ) {
        if( !input.exists() )
            standardOut->append("No input folder found named as  " + input.absoluteFilePath() +"\n");
        if( !output.exists() )
            standardOut->append("No output folder found named as " + input.absoluteFilePath() + "\n");
        if(!logFile.exists()) {
            standardOut->append("No log file found for sample  " + rundata->getCurrentSample() + "\n");
            standardOut->append("Expected log file : " + logFile.absoluteFilePath());
        }
        return false;
    }

    return true;
}

/*
 * Function called every time the timer expires. Intended to be what updates the table based off the log
 * input for the current sample.
 */
void ProgressDialog::readStepsLog(){
    _stepsCompleted = 0;

    //if(!this->checkInputOutPutLocations()) return;

    QString OUTPUTPATH = this->rundata->getParams()["folderOutput"];
    QString pathToLog = OUTPUTPATH + QDir::separator() + rundata->getCurrentSample() + QDir::separator() + "metapathways_steps_log.txt";

    QFile inputFile(pathToLog);
    QHash<QString, QString> statusHash;
    QRegExp whiteSpace("\\s");
    QRegExp commentLine("#[^\"\\n\\r]*");

    logBrowser->clear();
    if (inputFile.exists() && inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();

            if (!commentLine.exactMatch(line) && line.length()>0){
                //if not a comment line
                logBrowser->append(line);
                QStringList splitList = line.split(whiteSpace,QString::SkipEmptyParts);
                QString step = splitList.at(0).trimmed();
                QString status = splitList.at(1).trimmed();


                if (!step.isEmpty())  statusHash.insert(step,status);
            }
       }
       inputFile.close();
    }else{
        logBrowser->append("The log file has not yet been generated. Please wait.");
    }

    // problem : several steps have an unknown number of databases (sub-steps) that run
    // so we pass these values off to another function to see what the overall progress is
    checkStepsWithDBS(&statusHash, "BLAST_REFDB_","BLAST_REFDB");
    checkStepsWithDBS(&statusHash, "PARSE_BLAST_", "PARSE_BLAST");
    checkStepsWithDBS(&statusHash, "SCAN_rRNA_", "SCAN_rRNA");
    checkStepsWithDBS(&statusHash, "STATS_", "STATS_rRNA");
    checkStepsWithDBS(&statusHash, "GENBANK_FILE", "GENBANK_FILE");

    // update icons for each step

    colorRunConfig(statusHash);
    // update progress bar

    updateProgressBar();

    if( myProcess !=0 ) {
        // dump out output from stdout to the other log
        QByteArray read = myProcess->readAll();
        if (!read.isEmpty()) standardOut->append(QString(read));
    }
}

/*
 * There is no mapping from the table to the groupboxes. Therefore, this function will declare
 * the mapping of a row index to its groupbox.
 */
void ProgressDialog::initMapping(){
    TABLE_MAPPING = new QHash<int,QString>();

    TABLE_MAPPING->operator [](0) = "PREPROCESS_FASTA";
    TABLE_MAPPING->operator [](1) = "ORF_PREDICTION";
    TABLE_MAPPING->operator [](2) = "GFF_TO_AMINO";
    TABLE_MAPPING->operator [](3) = "FILTERED_FASTA";
    TABLE_MAPPING->operator [](4) = "COMPUTE_REFSCORE";
    TABLE_MAPPING->operator [](5) = "BLAST_REFDB";
    TABLE_MAPPING->operator [](6) = "PARSE_BLAST";
    TABLE_MAPPING->operator [](7) = "SCAN_rRNA";
    TABLE_MAPPING->operator [](8) = "STATS_rRNA";
    TABLE_MAPPING->operator [](9) = "SCAN_tRNA";
    TABLE_MAPPING->operator [](10) = "ANNOTATE";
    TABLE_MAPPING->operator [](11) = "PATHOLOGIC_INPUT";
    TABLE_MAPPING->operator [](12) = "GENBANK_FILE";
    TABLE_MAPPING->operator [](13) = "CREATE_SEQUIN_FILE";
    TABLE_MAPPING->operator [](14) = "CREATE_REPORT_FILES";
    TABLE_MAPPING->operator [](15) = "MLTREEMAP_CALCULATION";
    TABLE_MAPPING->operator [](16) = "MLTREEMAP_IMAGEMAKER";
    TABLE_MAPPING->operator [](17) = "PATHOLOGIC";
}

/*
 * You know.
 */
void ProgressDialog::updateProgressBar(){
    progressBar->setMinimum(0);
    progressBar->setValue(_stepsCompleted);

    _totalSteps = this->countTotalNumberOfSteps();
    progressBar->setMaximum(_totalSteps);

}


unsigned int ProgressDialog::countTotalNumberOfSteps() {
    RunData *rundata = RunData::getRunData();

    QStringList candidate_keys = rundata->getPARAMS().keys();
    QRegExp metapathway_steps("^metapaths_steps*");

    QRegExp metapathway_steps_BLAST_REDB("^metapaths_steps:BLAST_REFDB");
    QRegExp metapathway_steps_SCAN_rRNA("^metapaths_steps:SCAN_rRNA");

    unsigned int count=0;
    /* count the steps that are set to redo and yes in the config */
    foreach( QString candidate_step, candidate_keys ) {
        if( metapathway_steps.indexIn(candidate_step,0) != -1 ) {
            if(QString("redo").compare(rundata->getPARAMS()[candidate_step]) ==0 ||\
               QString("yes").compare(rundata->getPARAMS()[candidate_step]) ==0 )  {
                   if( metapathway_steps_BLAST_REDB.indexIn(candidate_step,0) != -1 ) {
                       count += rundata->getNumADB();
                   }
                   else if( metapathway_steps_SCAN_rRNA.indexIn(candidate_step,0) != -1 ) {
                       count += rundata->getNumRRNADB();
                   }
                   else
                       count++;
            }
        }
    }
    return rundata->getFileList().size()*count;
}

/*
 * Some special handling of certain steps to ascern their true states.
 */
void ProgressDialog::checkStepsWithDBS(QHash<QString,QString> *statusHash, QString stepName, QString realStepName){
    QHash<QString,QString>::iterator it;

    // GENBANK_FILE, CREATE_SEQUIN_FILE, AND PATHOLOGIC_INPUT are considered to be really all the same step
    // so if any one of them fails, they all fail, any one is done, they're all done, etc
    if(stepName=="GENBANK_FILE"){
        if(statusHash->operator []("GENBANK_FILE")=="FAILED"){
            statusHash->operator []("CREATE_SEQUIN_FILE") = "FAILED";
            statusHash->operator []("PATHOLOGIC_INPUT") = "FAILED";
            _stepsCompleted++; _stepsCompleted++; _stepsCompleted++;
        }else if(statusHash->operator []("GENBANK_FILE")=="RUNNING"){
            statusHash->operator []("CREATE_SEQUIN_FILE") = "RUNNING";
            statusHash->operator []("PATHOLOGIC_INPUT") = "RUNNING";
            _stepsCompleted++; _stepsCompleted++; _stepsCompleted++;
        }else if(statusHash->operator []("GENBANK_FILE")=="SUCCESS"){
            statusHash->operator []("CREATE_SEQUIN_FILE") = "SUCCESS";
            statusHash->operator []("PATHOLOGIC_INPUT") = "SUCCESS";
            _stepsCompleted++; _stepsCompleted++; _stepsCompleted++;
        }else if(statusHash->operator []("GENBANK_FILE")=="SKIPPED"){
            statusHash->operator []("CREATE_SEQUIN_FILE") = "SKIPPED";
            statusHash->operator []("PATHOLOGIC_INPUT") = "SKIPPED";
            _stepsCompleted++; _stepsCompleted++; _stepsCompleted++;
        }else if(statusHash->operator []("GENBANK_FILE")=="ALREADY_COMPUTED"){
            statusHash->operator []("CREATE_SEQUIN_FILE") = "ALREADY_COMPUTED";
            statusHash->operator []("PATHOLOGIC_INPUT") = "ALREADY_COMPUTED";
            _stepsCompleted++; _stepsCompleted++; _stepsCompleted++;
        }
        return;
    }

    // checking through the hash for one of our problem steps
    // stepName is just the substring of what a substep would look like
    // eg realStepName = BLAST_REFDB, stepName = BLAST_REFDB_
    for (it=statusHash->begin();it!=statusHash->end();it++){
        QString k = it.key();
        if (k.contains(stepName)){
            // one of our problem steps with multiple databases to go through
            QString v = it.value();
            //qDebug() << k << v << stepName << realStepName;

            if (v == "FAILED") {
                statusHash->insert(realStepName,"FAILED");
                break;
            }
            else if (v == "RUNNING"){
                statusHash->insert(realStepName, "RUNNING");
            }
            else if (v == "SUCCESS") {
                statusHash->insert(realStepName,"SUCCESS");
            }
            else if (v == "ALREADY_COMPUTED") {
                statusHash->insert(realStepName,"ALREADY_COMPUTED");
            }
            else if (v == "SKIPPED"){
                statusHash->insert(realStepName,"SKIPPED");
            }

        }
    }
}

/*
 * Crayons, ma. Update the table with the appropriate widget depending on the status for that step.
 */
void ProgressDialog::colorRunConfig(QHash<QString,QString> &statusHash){

    QHash<QString,QString>::iterator it;
    _stepsCompleted = 0;
   // this->summaryTable->clearContents();
    QTableWidgetItem *item;

    ProgressDisplayData *progressdisplaydata = ProgressDisplayData::getProgressDisplayData();
    progressdisplaydata->destroyWidgets();
    progressdisplaydata->createWidgets(statusHash);

    for(it=statusHash.begin();it!=statusHash.end();it++){
        QString stepName = it.key();
        QString status = it.value();

        if( !progressdisplaydata->isValidKey(stepName)) continue;
        if (stepName.contains(QRegExp("BLAST_REFDB_"))) continue;
        if (stepName.contains(QRegExp("PARSE_BLAST_"))) continue;
        if (stepName.contains(QRegExp("SCAN_rRNA_"))) continue;
        if (stepName.contains(QRegExp("STATS_"))) continue;

        int _row = TABLE_MAPPING->key(stepName);

        //clear old cell
      //  this->summaryTable->removeCellWidget(_row,0);
        this->summaryTable->setItem(_row, 0, NULL);

        if (status.operator ==("FAILED")){
            item = progressdisplaydata->getTableWidgetItem(stepName, REDCROSS);
            this->summaryTable->setItem(_row, 0, item);
            _stepsCompleted++;

        }else if (status.operator ==("RUNNING")){
            //loading->start();
            item = progressdisplaydata->getTableWidgetItem(stepName, LOADING);
            this->summaryTable->setItem(_row,0,item);
        }

        else if (status.operator ==("SUCCESS") ||
                  status.operator ==("ALREADY_COMPUTED") ||
                  status.operator ==("SKIPPED"))  {

            //item->setData(Qt::DecorationRole, QPixmap::fromImage(img).scaled(12,12));
            item = progressdisplaydata->getTableWidgetItem(stepName, GREENCHECK);
            this->summaryTable->setItem(_row,0, item);
            _stepsCompleted++;
        }
    }

}

/*
 * When the user presses the run button, this function is called.
 */
void ProgressDialog::startRun(){
    QString rRNArefdbs = this->rundata->getParams()["rRNA:refdbs"];
    QString annotationDBS = this->rundata->getParams()["annotation:dbs"];

    // qDebug() << rRNArefdbs << annotationDBS;

    if(rRNArefdbs.isEmpty()){
        Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] + "/" + RunData::TEMPLATE_PARAM, "PARAMS", "rRNA:refdbs","",false,false);
    }
    if(annotationDBS.isEmpty()){
        Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] + "/" + RunData::TEMPLATE_PARAM,"PARAMS", "annotation:dbs","",false,false);
    }

    if (this->rundata->getParams()["fileInput"].isEmpty()
        || this->rundata->getParams()["folderOutput"].isEmpty()){
        // check to see if they're jonesing us
        QMessageBox::warning(0,"Input/Output Not Set!","Missing input or output folders!\nPlease set input and output folders in Stages tab.",QMessageBox::Ok );
    }
    else{
        // otherwise start off the process, clear out a bunch of statuses in case there was a previous run
        this->resetRunTab();
        initProcess();
    }

}


void ProgressDialog::resetRunTab() {
    cancelButton->setEnabled(true);
    runButton->setDisabled(true);
    standardOut->clear();
    logBrowser->clear();
    progressBar->setValue(0);

 //   timer->start(1000); // refresh rate of 1 sec for the log

    _stepsCompleted = 0;
    _totalSteps = TABLE_MAPPING->size(); // used for progress bar



}

/*
 * A bunch of hard core setup leading up to the firing off of the python process.
 */
void ProgressDialog::initProcess(){

    //if(!this->checkInputOutPutLocations()) return;
    QFileInfo input(this->rundata->getParams()["fileInput"]);
    QFileInfo output(this->rundata->getParams()["folderOutput"]);

    if(input.exists()) this->checkFiles();

    QString program =  QDir::toNativeSeparators(rundata->getConfig()["PYTHON_EXECUTABLE"]);
    METAPATH = QDir::toNativeSeparators(this->rundata->getConfig()["METAPATHWAYS_PATH"]);

    QStringList arguments;


    qDebug() << " num of databases to run on " << rundata->getNumADB();
    if(this->rundata->getSamplesSubsetToRun().size()==0) {

       QMessageBox::warning(0,"ERROR", "At least one sample must be selected to process!\nPlease select samples in the Stages tab.", QMessageBox::Ok);
       this->processFinished(0,QProcess::NormalExit);
       this->resetRunTab();
       return;
    }

//    qDebug() << this->rundata->getNumADB();



    if(this->rundata->getNumADB()==0 && this->rundata->getParams().contains(PIPELINE_STEP_BLAST)\
            && this->rundata->getParams()[PIPELINE_STEP_BLAST].indexOf(QString("skip")) == -1 )  {
        QMessageBox::warning(0,QString("Error!\n") ,QString("No reference Database selected! \nPlease make sure you selected at least one in the Parameters tab."), QMessageBox::Ok);
        this->resetRunTab();
        return;
    }

    // arguments to the python execution process call
    // recall : python code is run as python MetaPathways.py -i input -o output -p paramfile -c configfile -r writemode
    arguments <<  QDir::toNativeSeparators(METAPATH + QDir::separator() + "MetaPathways.py");
    if(runVerbose->isChecked()) arguments << "-v";
    arguments << "-i" << QDir::toNativeSeparators(this->rundata->getParams()["fileInput"]);
    arguments << "-o" << QDir::toNativeSeparators(this->rundata->getParams()["folderOutput"]);
    arguments << "-p" << QDir::toNativeSeparators(METAPATH + QDir::separator() + "template_param.txt");
    arguments << "-c" << QDir::toNativeSeparators(METAPATH + QDir::separator() + "template_config.txt");
    arguments << "-r" << (this->rundata->getParams()["overwrite"]);
    if(computeStats->isChecked())  arguments << "--stats" << "on";
    //add the specific samples
    foreach(QString samplename, this->rundata->getSamplesSubsetToRun()) {
        arguments << "-s" << samplename;
    }
   // qDebug() << arguments;

    // set up paths and environment - essential for the python code to run
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("CURRDIR", QDir::toNativeSeparators(METAPATH));
    env.insert("METAPATH", QDir::toNativeSeparators(METAPATH));
    env.insert("METAPATHLIB", QDir::toNativeSeparators(METAPATH + QDir::separator() + "libs"));
    env.insert("STARCLUSTERLIB", QDir::toNativeSeparators(METAPATH + "libs" + QDir::separator() + "starcluster"));
    env.insert("PYTHONPATH", QDir::toNativeSeparators(METAPATH + QDir::separator() +  "libs:" + METAPATH + QDir::separator() + "libs" + QDir::separator() + "starcluster"));

    // the actual process setup
    myProcess = new QProcess();
    myProcess->setProcessEnvironment(env);
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(program, arguments);
    standardOut->append( arguments.join(" ") );

    rundata->setProcess(myProcess); // let rundata know what's good

    // set up the timer so the logs will be updated

    connect(myProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
}


void ProgressDialog::selectedFileChanged(QString file){
    rundata->setCurrentSample(file);
    //progressLabel->setText("Progress - " + rundata->getCurrentSample());
    logBrowser->clear();

}

void ProgressDialog::checkFiles(){
    QDir currentDir(this->rundata->getParams()["fileInput"]);
    QString fileType = this->rundata->getParams()["INPUT:format"];

    if( !currentDir.exists() ) return;

    currentDir.setFilter(QDir::Files);
    QStringList entries = currentDir.entryList();

    QList<QRegExp> regList;
    regList << QRegExp("[.][fF][aA][sS][tT][aA]$") << QRegExp("[.][fF][aA]$") << QRegExp("[.][fF][aA][aA]$") << QRegExp("[.][fF][aA][sS]") << QRegExp("[.][fF][nN][aA]$");

    filesDetected.clear();
    for( QStringList::ConstIterator entry=entries.begin(); entry!=entries.end(); ++entry )
    {
        QString temp = *entry;
        QStringList file = temp.split(".");

        if (fileType == "fasta"){
            foreach(QRegExp reg, regList ) {
               if(temp.indexOf(reg,0) > -1 ) {
                   filesDetected.append( temp.remove(reg).replace('.','_') );
                   break;
               }
            }
        }
        else if (fileType == "gbk-annotated" || fileType == "gbk-unannotated"){
            if (file.last() == "gbk"){
                filesDetected.append(file.first());
            }
        }
        else if (fileType == "gff-annotated" || fileType == "gff-unannotated"){
            if (file.last() == "gff"){
                filesDetected.append(file.first());
            }
        }
    }

    this->rundata->setFileList(filesDetected);
}

/*
 * Reset the state after its finished, but don't clear everything.
 */
void ProgressDialog::processFinished(int exitCode, QProcess::ExitStatus exitStatus){
    runButton->setEnabled(true);
    cancelButton->setEnabled(false);
    progressBar->setValue(0);
    myProcess = 0;
}


/*
 * Kill the current run. Clear everything.
 */
void ProgressDialog::terminateRun(){
    if(myProcess !=0 && myProcess->Running == QProcess::Running)  {
        myProcess->kill();
        // dump out output from stdout to the other log
        QByteArray read = myProcess->readAll();
        standardOut->append(QString("Received a TERMINATION/KILL signal"));
    }

    myProcess = 0;

   // summaryTable->clearContents();
    progressBar->setValue(0);
  //  rundata->setCurrentSample("");
    runButton->setEnabled(true);
}

ProgressDialog::~ProgressDialog()
{
    myProcess->kill();
    timer->stop();
    delete ui;
}