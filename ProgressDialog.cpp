#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"
#include "utilities.h"
#include "rundata.h"
#include "resultwindow.h"
#include <QProcess>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QMovie>
#include <QTextStream>
#include <QDir>

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
    sampleSelect = this->findChild<QComboBox *>("sampleSelect");
    runVerbose = this->findChild<QCheckBox *>("runVerboseCheckBox");
    computeStats = this->findChild<QCheckBox *>("computeStats");

    summaryTable->setSortingEnabled(false);

    initMapping();

    timer = new QTimer(this);
    this->myProcess =0;

    cancelButton->setEnabled(false);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
    connect(runButton,SIGNAL(clicked()), this, SLOT(startRun()));
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

                //qDebug() << line << splitList <<   " step " << step << " status " << status;
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
    colorRunConfig(&statusHash);
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
    progressBar->setMaximum(_totalSteps);
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
void ProgressDialog::colorRunConfig(QHash<QString,QString> *statusHash){

    QHash<QString,QString>::iterator it;
    _stepsCompleted = 0;
    this->summaryTable->clearContents();

    for(it=statusHash->begin();it!=statusHash->end();it++){
        QString stepName = it.key();
        QString status = it.value();

        if (stepName.contains(QRegExp("BLAST_REFDB_"))) continue;
        if (stepName.contains(QRegExp("PARSE_BLAST_"))) continue;
        if (stepName.contains(QRegExp("SCAN_rRNA_"))) continue;
//        if (stepName.contains(QRegExp("STATS_"))) continue;

        QTableWidgetItem *item = new QTableWidgetItem();
        QMovie *loading = new QMovie(":/images/loading.gif");
        QLabel *imageLabel = new QLabel();
        loading->setParent(imageLabel);

        int _row = TABLE_MAPPING->key(stepName);

        //clear old cell
        this->summaryTable->removeCellWidget(_row,0);
        this->summaryTable->setItem(_row,0, NULL);

        QImage img;

        if (status.operator ==("FAILED")){
            img  = QImage(":/images/cross.png");
            item->setData(Qt::DecorationRole, QPixmap::fromImage(img).scaled(12,12));
            this->summaryTable->setItem(_row,0, item);
            _stepsCompleted++;

        }else if (status.operator ==("RUNNING")){
            imageLabel->setFixedSize(30,25);
            imageLabel->setMovie(loading);
            imageLabel->updateGeometry();
            loading->start();
            this->summaryTable->setCellWidget(_row,0,imageLabel);
        }else if (status.operator ==("SUCCESS") ||
                  status.operator ==("ALREADY_COMPUTED") ||
                  status.operator ==("SKIPPED")){
            img = QImage(":/images/check.png");
            item->setData(Qt::DecorationRole, QPixmap::fromImage(img).scaled(12,12));
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
        QMessageBox::warning(0,"Input/Output Not Set!","Missing input or output folders, step back to stages and make sure you've filled that in!",QMessageBox::Ok );
    }
    else{
        // otherwise start off the process, clear out a bunch of statuses in case there was a previous run
        cancelButton->setEnabled(true);
        runButton->setDisabled(true);
        standardOut->clear();
        logBrowser->clear();
        sampleSelect->clear();
        summaryTable->clearContents();
        progressBar->setValue(0);

        timer->start(1000); // refresh rate of 1 sec for the log

        _stepsCompleted = 0;
        _totalSteps = TABLE_MAPPING->size(); // used for progress bar

        initProcess();
    }

}

/*
 * A bunch of hard core setup leading up to the firing off of the python process.
 */
void ProgressDialog::initProcess(){

    //if(!this->checkInputOutPutLocations()) return;

    QFileInfo input(this->rundata->getParams()["fileInput"]);
    QFileInfo output(this->rundata->getParams()["folderOutput"]);

    if(input.exists()) this->checkFiles();

    QStringList files = this->rundata->getFileList();
   // qDebug() << files;
    foreach (QString f, files){
        sampleSelect->addItem(f); // sampleselect lets the user monitor the status of multiple samples
    }

    connect(sampleSelect, SIGNAL(activated(QString)), this, SLOT(selectedFileChanged(QString)));
    rundata->setCurrentSample(sampleSelect->currentText());

    QString program =  QDir::toNativeSeparators(rundata->getConfig()["PYTHON_EXECUTABLE"]);
    METAPATH = QDir::toNativeSeparators(this->rundata->getConfig()["METAPATHWAYS_PATH"]);

    QStringList arguments;
    // arguments to the python execution process call
    // recall : python code is run as python MetaPathways.py -i input -o output -p paramfile -c configfile -r writemode
    arguments <<  QDir::toNativeSeparators(METAPATH + QDir::separator() + "MetaPathways.py");
    if(runVerbose->isChecked()) arguments << "-v";
    arguments << "-i" << QDir::toNativeSeparators(this->rundata->getParams()["fileInput"]);
    arguments << "-o" << QDir::toNativeSeparators(this->rundata->getParams()["folderOutput"]);
    arguments << "-p" << QDir::toNativeSeparators(METAPATH + QDir::separator() + "template_param.txt");
    arguments << "-c" << QDir::toNativeSeparators(METAPATH + QDir::separator() + "template_config.txt");
    arguments << "-r" << (this->rundata->getParams()["overwrite"]);
    if(computeStats->isChecked())  arguments << "--stats";

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

    rundata->setProcess(myProcess); // let rundata know what's good

    // set up the timer so the logs will be updated
    connect(timer, SIGNAL(timeout()), this, SLOT(readStepsLog()));    
    connect(myProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
}


void ProgressDialog::selectedFileChanged(QString file){
    rundata->setCurrentSample(file);
    //progressLabel->setText("Progress - " + rundata->getCurrentSample());

    logBrowser->clear();
    summaryTable->clearContents();
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

    this->readStepsLog();
    timer->stop();
    rundata->setCurrentSample("");
    progressBar->setValue(0);
    myProcess = 0;
}


/*
 * Kill the current run. Clear everything.
 */
void ProgressDialog::terminateRun(){
    myProcess->kill();
    myProcess = 0;

    timer->stop();

    logBrowser->clear();
    standardOut->clear();
    sampleSelect->clear();
    summaryTable->clearContents();
    progressBar->setValue(0);

    rundata->setCurrentSample("");
    runButton->setEnabled(true);
}

ProgressDialog::~ProgressDialog()
{
    myProcess->kill();
    timer->stop();
    delete ui;
}
