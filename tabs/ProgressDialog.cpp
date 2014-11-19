
#include "ui_ProgressDialog.h"

#include <QProcess>
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QMovie>
#include <QTextStream>
#include <QDir>
#include <QTime>

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
    showMessages = this->findChild<QTabWidget *>("showMessages");
    sampleSelect = this->findChild<QComboBox *>("sampleSelect");
    runVerbose = this->findChild<QCheckBox *>("runVerboseCheckBox");
    overwrite = this->findChild<QCheckBox *>("overwrite");


    summaryTable->setSortingEnabled(false);
    this->setExpectedSteps();

    initMapping();

    timer = new QTimer(this);


    this->myProcess =0;
    this->currentBLOCK = 0;

    cancelButton->setEnabled(false);
    runButton->setEnabled(true);

    overwrite->hide();
    this->showErrors();


    connect(timer, SIGNAL(timeout()), this, SLOT(timerTickResponse()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
    connect(runButton, SIGNAL(clicked()), this, SLOT(startRun()));
    connect(sampleSelect, SIGNAL(activated(QString)), this, SLOT(selectedFileChanged(QString)));

   // connect(sampleSelect, SIGNAL( currentIndexChanged(QString)  ), this, SLOT(readStepsLog() ) );

 // connect(sampleSelect,SIGNAL(currentIndexChanged(int)) , this, SLOT(readStepsLog()) );

    connect(this->rundata, SIGNAL(loadSampleList()), this, SLOT(loadSampleListToRun()));
  //  connect(this->showErrorsButton, SIGNAL(clicked()), this, SLOT(showErrors()));
    connect(this->overwrite, SIGNAL(clicked()), this, SLOT( updateOverwriteChoice() ) );
    connect(runButton, SIGNAL(clicked()), this, SLOT(readStepsLog()) );
}


/**
 * @brief ProgressDialog::showErrors, shows the error during the run
 */
void ProgressDialog::showErrors() {
  /* OLD WAY StatusView *statusview = StatusView::getStatusView();
   statusview->showTreeView();
   */

    TreeModel *model = new TreeModel;

    model->setSampleNames(this->rundata->getSamplesSubsetToRun());
    model->readFiles();

    QTreeView *view = new QTreeView;
    view->setModel(model);
    view->expandAll();
    view->show();
    view->resizeColumnToContents(0);
    view->resizeColumnToContents(1);
    view->resizeColumnToContents(2);
    this->showMessages->addTab(view,"Error Log");

}

/**
 * @brief ProgressDialog::loadSampleListToRun, loads the list of samples to the
 * drop down menu in the progress tab
 */
void ProgressDialog::loadSampleListToRun() {

    QStringList files =  this->rundata->getSamplesSubsetToRun();
    sampleSelect->clear();
    foreach (QString f, files){
        sampleSelect->addItem(f); // sampleselect lets the user monitor the status of multiple samples
    }
    rundata->setCurrentSample(sampleSelect->currentText());
}

/**
 * @brief ProgressDialog::checkInputOutPutLocations, checks if the input and output folder along with the
 * step logare a available
 * @return true or false
 */
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


/**
 * @brief ProgressDialog::setExpectedSteps setup the expected steps for the
 * widgets
 */
void ProgressDialog::setExpectedSteps() {
    this->expectedSteps["PREPROCESS_INPUT"] = true;
    this->expectedSteps["ORF_PREDICTION"] = true;
 //   this->expectedSteps["ORF_TO_AMINO"] = true;
    this->expectedSteps["FILTER_AMINOS"] = true;
  //  this->expectedSteps["COMPUTE_REFSCORES"] = true;
    this->expectedSteps["FUNC_SEARCH"] = true;
    this->expectedSteps["PARSE_FUNC_SEARCH"] = true;
    this->expectedSteps["SCAN_rRNA"] = true;
    this->expectedSteps["SCAN_tRNA"] = true;
    this->expectedSteps["ANNOTATE_ORFS"] = true;
    this->expectedSteps["PATHOLOGIC_INPUT"] = true;
//    this->expectedSteps["GENBANK_FILE"] = true;
    this->expectedSteps["CREATE_ANNOT_REPORTS"] = true;
 //  this->expectedSteps["MLTREEMAP_CALCULATION"] = true;
    this->expectedSteps["BUILD_PGDB"] = true;
    this->expectedSteps["COMPUTE_RPKM"] = true;
}


/**
 * @brief ProgressDialog::initMapping, There is no mapping from the
 * table to the groupboxes. Therefore, this function will declare
 * the mapping of a row index to its groupbox.
 */
void ProgressDialog::initMapping(){
  //  TABLE_MAPPING = new QHash<int,QString>();

    TABLE_MAPPING[0] = "PREPROCESS_INPUT";
    TABLE_MAPPING[1] = "ORF_PREDICTION";
 //   TABLE_MAPPING[2] = "ORF_TO_AMINO";
    TABLE_MAPPING[2] = "FILTER_AMINOS";
 //   TABLE_MAPPING[4] = "COMPUTE_REFSCORES";
    TABLE_MAPPING[3] = "FUNC_SEARCH";
    TABLE_MAPPING[4] = "PARSE_FUNC_SEARCH";
    TABLE_MAPPING[5] = "SCAN_rRNA";
    TABLE_MAPPING[6] = "SCAN_tRNA";
    TABLE_MAPPING[7] = "ANNOTATE_ORFS";
    TABLE_MAPPING[8] = "PATHOLOGIC_INPUT";
  //  TABLE_MAPPING[11] = "GENBANK_FILE";
    TABLE_MAPPING[9] = "CREATE_ANNOT_REPORTS";
  //  TABLE_MAPPING[13] = "MLTREEMAP_CALCULATION";
    TABLE_MAPPING[10] = "BUILD_PGDB";
    TABLE_MAPPING[11] = "COMPUTE_RPKM";

}


/**
 * @brief ProgressDialog::updateStatus, inserts with the right status;
 * @param line
 */

void ProgressDialog::updateStatus(const QString &line) {

    this->running.clear();

    if( line.contains(QRegExp("====")) && line.contains(QString("BLOCK0"))  ) {
        foreach(QString stepName, this->status.keys()) {
            if(QString("redo").compare(this->rundata->getPARAMS()[QString("metapaths_steps:") + stepName]) ==0 ||\
               QString("yes").compare(this->rundata->getPARAMS()[QString("metapaths_steps:") + stepName]) ==0 ) {
                  this->status[stepName].done.clear();
               }
        }
    }

    if( line.contains(QRegExp("SKIPPED"))) return;

    QStringList splitList = line.split(QRegExp("[\\s\\t]"),QString::SkipEmptyParts);

    if( splitList.size() < 2) return;

    QString step = splitList.at(0).trimmed();

    QString status = splitList.at(1).trimmed();

    QString key, subkey;
    QStringList keySubkey = step.split(QRegExp(":"));
    if( keySubkey.size()==2 && !keySubkey.isEmpty() && !keySubkey.isEmpty()) {
        key = keySubkey[0]; subkey = keySubkey[1];
    }
    else {
        key =  step.trimmed(); subkey = step.trimmed();
    }

    if( status.compare("RUNNING") ==0 ) this->running[QString("RUNNING")] = key;


    STATUS _status;
    if( this->status.contains(key)) _status = this->status[key];

    if( key.compare(QString("FUNC_SEARCH")) ==0 || key.compare(QString("PARSE_FUNC_SEARCH")) ==0   ) {
       QStringList dbs = this->rundata->getADBNames();
       foreach(QString db, dbs) _status.expected[db] = true;
    }

    if( key.compare(QString("SCAN_rRNA")) ==0 ) {
       QStringList dbs = this->rundata->getrNADBNames();
       foreach(QString db, dbs) _status.expected[db] = true;
    }

    if(key.compare(subkey)==0) _status.expected[key]= true;
    else _status.expected[key] = false;

    //only accept known steps
    if( !this->expectedSteps.contains(key)) return;

    _status.step = key;
    if( status.indexOf("ALREADY_COMPUTED") != -1 || status.indexOf("SUCCESS")!= -1 ) {
        if( _status.expected.contains(subkey) ) {
            if(key.compare(subkey)!=0)  { _status.done[subkey] = true; _status.done[key] = false; }
            else _status.done[subkey] = true;
        }
    }
    else {
        if( _status.expected.contains(subkey) ) _status.done[subkey] = false;
    }
    this->status[key] = _status;

}

unsigned int ProgressDialog::waitCounter =0;


/**
 * @brief ProgressDialog::delayReadingStepsLog, this function is called to to read the
 * steps logs and yet delay it for subsequenct reads that are triggered by the timer
 */
void ProgressDialog::delayReadingStepsLog() {
    ProgressDialog::waitCounter = 0;
    this->readStepsLog();
    ProgressDialog::waitCounter=2;
}

/**
 * @brief ProgressDialog::finalTimerTickResponse, respond to the tick of the second timer
 */
void ProgressDialog::finalTimerTickResponse() {
    this->readStepsLog();

    this->updateCurrentRunningProcessStatus();
    qDebug() << "responding to the second timer";
    //timer2->stop();

}


/**
 * @brief ProgressDialog::timerTickResponse, responds to the tick of the timer
 */
void ProgressDialog::timerTickResponse() {

    this->readStepsLog();

    this->updateCurrentRunningProcessStatus();

    if( !this->isProcessRunning() ) {
        this->timer->stop();
       /* timer2 = new QTimer;
        connect(timer2, SIGNAL(timeout()), this, SLOT(finalTimerTickResponse()));
        timer2->start(5000);
        qDebug() << "starting the second timer"; */
    }
}


/**
 * @brief ProgressDialog::readStepsLog
 *  Function called every time the timer expires. Intended to be what updates
 *  the table based off the log input for the current sample.
 */
void ProgressDialog::readStepsLog(){
   // static unsigned int i =0;


    if(  ProgressDialog::waitCounter > 0 ) {
        ProgressDialog::waitCounter--;
        return;
    }

    if( rundata->getCurrentSample().isEmpty()  ) return;

    QString sampleForLog = sampleSelect->currentText().trimmed();

    _stepsCompleted = 0;

    QString OUTPUTPATH = this->rundata->getParams()["folderOutput"];
    QString pathToLog = OUTPUTPATH + QDir::separator() + sampleForLog + QDir::separator() + "metapathways_steps_log.txt";

    QFile inputFile(pathToLog);
    QRegExp commentLine("#[^\"\\n\\r]*");

    logBrowser->clear();
    this->status.clear();
    bool process = false;

    if (inputFile.exists() && inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            if (!commentLine.exactMatch(line) && line.length()>0){   //if not a comment line
                logBrowser->append(line);
                if(line.contains(this->runid) ) process = true;
                if(process) this->updateStatus(line);
            }
       }
       logBrowser->update();
       inputFile.close();
    }else{
        logBrowser->append("The log file has not yet been generated. Please wait.");
    }


    // update icons for each step
    colorRunConfig();
    shadeActiveSteps();

    if( myProcess !=0 ) {
        updateProgressBar();
        // dump out output from stdout to the other log
        QByteArray read = myProcess->readAll();
        if (!read.isEmpty()) standardOut->append(QString(read));
        standardOut->update();
    }

}




void ProgressDialog::updateHighestBlock(QString sampleName) {

    QString OUTPUTPATH = this->rundata->getParams()["folderOutput"];
    QString pathToLog = OUTPUTPATH + QDir::separator() + sampleName + QDir::separator() + "metapathways_steps_log.txt";

    if( this->runid.isEmpty() ) return;

    QFile inputFile(pathToLog);
    QRegExp commentLine("#[^\"\\n\\r]*");


    if (inputFile.exists() && inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            if (!commentLine.exactMatch(line) && line.length()>0){   //if not a comment line
                if(line.contains(this->runid) ) {

                    if(line.contains(QString("BLOCK1"))) {
                        if(this->currentBLOCK == 0) {
                            this->currentBLOCK= 1;
                        }
                    }
                    if(line.contains(QString("BLOCK2"))) {
                        this->currentBLOCK = 2;
                    }

                }
            }
       }
       inputFile.close();
    }

}


/**
 * @brief ProgressDialog::isProcessingSample, checks if the sample has been processed by validating the current
 *        runid in the log, if present that means it was done in the current run.
 * @param sampleName
 * @return
 */
bool ProgressDialog::isProcessingSample(QString sampleName) {
    QString OUTPUTPATH = this->rundata->getParams()["folderOutput"];
    QString pathToLog = OUTPUTPATH + QDir::separator() + sampleName + QDir::separator() + "metapathways_steps_log.txt";


    if( this->runid.isEmpty() ) return false;

    QFile inputFile(pathToLog);
    QRegExp commentLine("#[^\"\\n\\r]*");

    if (inputFile.exists() && inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            if (!commentLine.exactMatch(line) && line.length()>0){   //if not a comment line
                if(line.contains(this->runid) ) {
                    if(line.contains(QString("BLOCK0")) && this->currentBLOCK==0) {
                              return true;
                    }
                    else if(line.contains(QString("BLOCK1")) && this->currentBLOCK==1) {
                            return true;
                    }
                    else if(line.contains(QString("BLOCK2")) &&    this->currentBLOCK == 2) {
                        return true;
                    }
                }
            }
       }
       inputFile.close();
    }else{
        logBrowser->append("The log file has not yet been generated. Please wait.");
    }

    return false;
}


/**
 * @brief ProgressDialog::updateCurrentRunningProcessStatus, updates the current running sample to
 * display the last running sample
 */
void ProgressDialog::updateCurrentRunningProcessStatus() {

    // compute the most upto date block no
    for(int i = 0; i < sampleSelect->count(); i++) {
        QString sampleName = sampleSelect->itemText(i);
        this->updateHighestBlock(sampleName);
    }

    for(int i = 0; i < sampleSelect->count(); i++) {
        QString sampleName = sampleSelect->itemText(i);

        if(!this->rundata->isAlreadyProcessedSample(sampleName) && this->isProcessingSample(sampleName) ) {
            this->rundata->addToProcessedSamples(sampleName);
            if( ProgressDialog::waitCounter ==0)
               if(this->sampleSelect->currentIndex() != i) {
                  this->sampleSelect->setCurrentIndex(i);

               }
            this->sampleSelect->update();

      //      qDebug() <<  this->rundata->numberOfProcessedSamples() << "  " << this->sampleSelect->count();
            if( this->rundata->numberOfProcessedSamples()==this->sampleSelect->count()) this->rundata->clearProcessedSamples();
            return;
        }
    }
}


/**
 * @brief ProgressDialog::updateProgressBar, computes the number steps completed against
 * the total number of step with redo or yes and updates the progress bar.
 */

void ProgressDialog::updateProgressBar(){
    progressBar->setMinimum(0);

    _stepsCompleted = this->getNumStepsCompleted();

    _totalSteps = this->countTotalNumberOfSteps();

    if( this->rundata->getProcess()==0) {
        progressBar->setValue(1);
    }else {
          progressBar->setValue( _stepsCompleted < _totalSteps ? 2*_stepsCompleted + 1 : 2*_totalSteps   );
    }
    _totalSteps = this->countTotalNumberOfSteps();

 //   qDebug() << _stepsCompleted << "/" << _totalSteps;

    progressBar->setMaximum( _totalSteps==0 ? 2 : 2*_totalSteps );

}


/**
 * @brief ProgressDialog::getActiveSteps, retuns the step names of the states
 * that have redo or yes
 * @return
 */

QStringList ProgressDialog::getActiveSteps() {

    QStringList activeSteps;
    foreach(QString stepName, this->TABLE_MAPPING.values() ) {
        if(QString("redo").compare(this->rundata->getPARAMS()[QString("metapaths_steps:") + stepName]) ==0 ||\
           QString("yes").compare(this->rundata->getPARAMS()[QString("metapaths_steps:") + stepName]) ==0 ) {
               activeSteps.append(stepName);
           }
    }
    return activeSteps;

}



/**
 * @brief ProgressDialog::getNumStepsCompleted, computes the number of completed steps in the
 *  current run that has redo or yes settings
 * @return number of completed steps
 */
unsigned int ProgressDialog::getNumStepsCompleted() {
    unsigned int num = 0;

    foreach(QString stepName, this->status.keys()) {
        if(QString("redo").compare(this->rundata->getPARAMS()[QString("metapaths_steps:") + stepName]) ==0 ||\
           QString("yes").compare(this->rundata->getPARAMS()[QString("metapaths_steps:") + stepName]) ==0 ) {

              foreach(bool yesno, this->status[stepName].done.values() ) {
                  if(yesno)  { num++;
                  //    qDebug() << stepName;
                  }
              }

           }
    }
    return num;

}


/**
 * @brief ProgressDialog::countTotalNumberOfSteps, function calculates the number of stages
 * to computed based on the settings of the metapaths_steps
 * @return number of steps to do
 */
unsigned int ProgressDialog::countTotalNumberOfSteps() {
    RunData *rundata = RunData::getRunData();

    QStringList candidate_keys = rundata->getPARAMS().keys();
    QRegExp metapathway_steps("^metapaths_steps*");

    QRegExp metapathway_steps_FUNC_SEARCH("^metapaths_steps:FUNC_SEARCH");
    QRegExp metapathway_steps_SCAN_rRNA("^metapaths_steps:SCAN_rRNA");

    unsigned int count=0;
    /* count the steps that are set to redo or yes in the config */
    foreach( QString candidate_step, candidate_keys ) {
        if( metapathway_steps.indexIn(candidate_step,0) != -1 ) {
            if(QString("redo").compare(rundata->getPARAMS()[candidate_step]) ==0 ||\
               QString("yes").compare(rundata->getPARAMS()[candidate_step]) ==0 )  {
                   if( metapathway_steps_FUNC_SEARCH.indexIn(candidate_step,0) != -1 ) {
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
    return count;
}



/**
 * @brief ProgressDialog::getState, gets the running state of a step
 * -2 if unknown, 1 means success, 0 is partial -1 is failed
 * @param stepName
 * @return the code for the state
 */
short int ProgressDialog::getState(const QString &stepName) {

    if( !this->status.contains(stepName) ) return -2;

    STATUS _status = this->status[stepName];
    unsigned int successCount = 0;


    foreach(QString subKey, _status.expected.keys()) {
        if( _status.done.contains(subKey)  && _status.done[subKey]) successCount++;
    }

    unsigned int expectedSuccess = 0;

    foreach(QString subKey, _status.expected.keys()) {
         if( _status.expected.contains(subKey)  && _status.expected[subKey])  expectedSuccess++;
    }

    if( successCount==expectedSuccess ) return 1;

    if( this->running.contains(QString("RUNNING"))  && this->running["RUNNING"].compare(stepName)==0 && this->myProcess!=0 ) {
        try {
            if( this->myProcess->Running !=0 ) return 2;
        }
        catch(...) {
            return -2;
        }

    }

    if( successCount > 0 &&  _status.expected.size() > 0  ) return 0;

    if( successCount ==0 &&  _status.expected.size() > 0  ) return -1;

    return -1;
}

/**
 * @brief ProgressDialog::isProcessRunning, is the process running
 * @return
 */
bool ProgressDialog::isProcessRunning() {
    if( this->myProcess!=0 ) {
        try {
            if( this->myProcess->Running !=0 ) return true;
        }
        catch(...) {
            return false;
        }
    }
    return false;
}


/**
 * @brief ProgressDialog::isRunningAStep, is any step running
 * @return
 */
bool ProgressDialog::isRunningAStep() {
    if( this->isProcessRunning() ) {
        if(this->running.contains("RUNNING")) return true;
    }

    return false;
}


/**
 * @brief ProgressDialog::shadeActiveSteps, shade the active steps of the
 * pipeline
 */

void ProgressDialog::shadeActiveSteps() {

    QHash<QString,STATUS> ::iterator it;
    _stepsCompleted = 0;
   // this->summaryTable->clearContents();
    QTableWidgetItem *item;

    QHash<QString, bool> activeSteps;
    foreach(QString stepName, this->getActiveSteps()) {
        activeSteps[stepName] =true;
    }

    for(unsigned int i =0; i < this->summaryTable->rowCount(); i++){
        item= this->summaryTable->item(i,0);

        if( activeSteps.contains( this->TABLE_MAPPING[i]) && this->isProcessRunning() ) {
            item->setBackground(Qt::lightGray);
        }
        else
            item->setBackground(Qt::white);
    }
}

/**
 * @brief ProgressDialog::isStepActive, is step active
 * @param stepName
 * @return true/false
 */
bool ProgressDialog::isStepActive(QString stepName) {

    QHash<QString, bool> activeSteps;
    foreach(QString stepName, this->getActiveSteps()) {
        activeSteps[stepName] =true;
    }

    if( activeSteps.contains(stepName) ) return true;
    return false;
}

bool ProgressDialog::updateItem(const QString stepName, STATUS_SYMBOL status, ProgressDisplayData *progressdisplaydata ) {
  int _row = TABLE_MAPPING.key(stepName);
 // qDebug() << this->_stepstatus.contains(stepName) << "   " << this->_stepstatus[stepName] << "  " << status;
  if( !this->_stepstatus.contains(stepName)|| this->_stepstatus[stepName]!=status) {
       this->summaryTable->setItem(_row, 0, NULL);
       progressdisplaydata->reCreateWidgets(stepName,status);
       QTableWidgetItem *item = progressdisplaydata->getTableWidgetItem(stepName, status);
       this->summaryTable->setItem(_row, 0, item);
    //   qDebug() << "item " << item;
       this->_stepstatus[stepName] = status;
       return true;
  }


  return false;

}

/**
 * @brief ProgressDialog::colorRunConfig,
 * Update the table with the appropriate widget depending on the status for that step
 */
void ProgressDialog::colorRunConfig(){
    _stepsCompleted = 0;
   // this->summaryTable->clearContents();

    ProgressDisplayData *progressdisplaydata = ProgressDisplayData::getProgressDisplayData();
    //if( !progressdisplaydata->widgetsCreated() )
   // progressdisplaydata->destroyWidgets();
    if( !progressdisplaydata->widgetsCreated() )
    progressdisplaydata->createWidgets(this->expectedSteps.keys());

    bool isAStepRunning = this->isRunningAStep();
    unsigned int runningStepNo = TABLE_MAPPING.key(this->running["RUNNING"]);

   // qDebug() << " runnint step no " << runningStepNo;
    foreach(QString stepName, this->expectedSteps.keys()){      
        int _row = TABLE_MAPPING.key(stepName);
    //    qDebug() << stepName << this->getState(stepName);
        if(isAStepRunning && runningStepNo < _row &&  this->isStepActive(stepName) ) {
             this->updateItem(stepName, UNSURE,progressdisplaydata);
        }
        else {
            if ( this->getState(stepName) == -1 ){
                if( this->updateItem(stepName, REDCROSS ,progressdisplaydata)) _stepsCompleted++;

            }else if (this->getState(stepName) == 0){
                  this->updateItem(stepName, PARTIAL ,progressdisplaydata);
            }
            else if (this->getState(stepName) == 1)  {
                 if(this->updateItem(stepName, GREENCHECK,progressdisplaydata)) _stepsCompleted++;
            }
            else if (this->getState(stepName) == 2)  {
                  this->updateItem(stepName, LOADING,progressdisplaydata);
            }
            else if (this->getState(stepName) == -2)  {
                  this->updateItem(stepName, UNSURE ,progressdisplaydata);
            }

        }
    }
}

bool ProgressDialog::isReadyToRun() {
    // this->checkBinaries();
     QString message;
     QString _message;
     bool valid = true;
     QString executablesDir = rundata->getValueFromHash("METAPATHWAYS_PATH", _CONFIG) +\
                              QDir::separator() + rundata->getValueFromHash("EXECUTABLES_DIR", _CONFIG);
     if( !rundata->checkBinaries(_message, executablesDir)) {
         message += QString("\n")  + _message;
         valid = false;
     }
     QString pythonExec = rundata->getValueFromHash("PYTHON_EXECUTABLE", _CONFIG);
     if( !rundata->checkPythonBinary(message, pythonExec)) {
         message += QString("\n\n")  + _message;
         valid = false;
     }
     if( !valid ) {
         QMessageBox report;
         report.setText(message);
         report.exec();
     }
     return valid;
}


/**
 * @brief ProgressDialog::startRun, When the user presses the run button, this function is called
 */
void ProgressDialog::startRun(){
    QString rRNArefdbs = this->rundata->getParams()["rRNA:refdbs"];
    QString annotationDBS = this->rundata->getParams()["annotation:dbs"];


    this->rundata->emitloadSampleList();
    if(rRNArefdbs.isEmpty()){
        Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] +  QDir::separator() + "config" + QDir::separator()  + RunData::TEMPLATE_PARAM, "PARAMS", "rRNA:refdbs","",false,false);
    }
    if(annotationDBS.isEmpty()){
        Utilities::writeSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] +  QDir::separator() + "config" + QDir::separator()  + RunData::TEMPLATE_PARAM,"PARAMS", "annotation:dbs","",false,false);
    }

    if (this->rundata->getParams()["fileInput"].isEmpty()
        || this->rundata->getParams()["folderOutput"].isEmpty()){
        // check to see if they're jonesing us
        QMessageBox::warning(0,"Input/Output Not Set!","Missing input or output folders!\nPlease set input and output folders in Stages tab.",QMessageBox::Ok );
    }
    else{

        if( !this->isReadyToRun() ) {
            Utilities::showInfo(this, "Cannot run from GUI. Incorrect or insufficient settings or input!\n Consider running manually!");
            return;
        }

        // otherwise start off the process, clear out a bunch of statuses in case there was a previous run
        this->resetRunTab(false, false);
        initProcess();
    }

}

/**
 * @brief ProgressDialog::resetRunTab, resets the run tab by enabling
 * the cancel button, diabling the run button and clearing the output of the
 * log and steps and then setting the progress bar to 0
 */
void ProgressDialog::resetRunTab(bool status, bool clearlog ) {
    cancelButton->setEnabled(!status);
    runButton->setEnabled(status);

    if( clearlog ) {
     //   standardOut->clear();
      //  logBrowser->clear();
    }

    progressBar->setValue(0);

 //   timer->start(1000); // refresh rate of 1 sec for the log
    _stepsCompleted = 0;
    _totalSteps = TABLE_MAPPING.size(); // used for progress bar
}



void ProgressDialog::checkBinaries() {

    qDebug() << " Checking the binaries";
    qDebug() << this->rundata->CONFIG;


    QString executablesFolder = this->rundata->getValueFromHash("METAPATHWAYS_PATH", _CONFIG) +\
                                QDir::separator() + this->rundata->getValueFromHash("EXECUTABLES_DIR", _CONFIG);

    QFile execfolder;

    //qDebug() << this->rundata->getConfig();


    bool success = false;
    if(!success ) {
        QMessageBox::warning(this, "Invalid binaries in the executables folder", "Detailed message");

    }


}



/**
 * @brief ProgressDialog::initProcess
 * A bunch of hard core setup leading up to the firing off of the python process
 */
void ProgressDialog::initProcess(){

    //if(!this->checkInputOutPutLocations()) return;
    QFileInfo input(this->rundata->getParams()["fileInput"]);

    if(input.exists()) this->checkFiles();

    QString program =  QDir::toNativeSeparators(rundata->getConfig()["PYTHON_EXECUTABLE"]);
    METAPATH = QDir::toNativeSeparators(this->rundata->getConfig()["METAPATHWAYS_PATH"]);


    QStringList arguments;

    if(this->rundata->getSamplesSubsetToRun().size()==0) {
       QMessageBox::warning(0,"ERROR", "At least one sample must be selected to process!\nPlease select samples in the Stages tab.", QMessageBox::Ok);
       this->processFinished(0,QProcess::NormalExit);
       this->resetRunTab(true);
       return;
    }

    if(this->rundata->getNumADB()==0 && this->rundata->getParams().contains(PIPELINE_STEP_BLAST)\
            && this->rundata->getParams()[PIPELINE_STEP_BLAST].indexOf(QString("skip")) == -1 )  {
        QMessageBox::warning(0,QString("Error!\n") ,QString("No reference Database selected! \nPlease make sure you selected at least one in the Parameters tab."), QMessageBox::Ok);
        this->resetRunTab(true);
        return;
    }

    // arguments to the python execution process call
    // recall : python code is run as python MetaPathways.py -i input -o output -p paramfile -c configfile -r writemode
    arguments <<  QDir::toNativeSeparators(METAPATH  + QDir::separator() +  "MetaPathways.py");
    if(runVerbose->isChecked()) arguments << "-v";
    arguments << "-i" << QDir::toNativeSeparators(this->rundata->getParams()["fileInput"]);
    arguments << "-o" << QDir::toNativeSeparators(this->rundata->getParams()["folderOutput"]);
    arguments << "-p" << QDir::toNativeSeparators(METAPATH + QDir::separator() + "config" + QDir::separator() + "template_param.txt");
    arguments << "-c" << QDir::toNativeSeparators(METAPATH + QDir::separator()  + "config" + QDir::separator() + "template_config.txt");
    this->updateOverwriteChoice();
    arguments << "-r" << (this->rundata->getParams()["overwrite"]);


    //add the specific samples
    foreach(QString samplename, this->rundata->getSamplesSubsetToRun()) {
        arguments << "-s" << samplename;
    }
    QTime time;

    runid = time.currentTime().toString();

    this->currentBLOCK = 0;

    arguments << "--runid" << runid;

    // set up paths and environment - essential for the python code to run
    QProcessEnvironment env = this->rundata->getProcessEnvironment(METAPATH);

    // the actual process setup

    myProcess = new QProcess();
    myProcess->setProcessEnvironment(env);
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(program, arguments); // start the python script

    standardOut->clear();
    this->timer->start(5000);

 //   standardOut->append( arguments.join(" ") );

    this->resetRunTab(false);
    rundata->setProcess(myProcess); // let rundata know what's good

    // set up the timer so the logs will be updated

    this->readStepsLog();
    connect(myProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
}

/**
 * @brief ProgressDialog::selectedFileChanged, the user selected a different sample
 * @param sampleName
 */
void ProgressDialog::selectedFileChanged(const QString &sampleName){
    rundata->setCurrentSample(sampleName);
    //progressLabel->setText("Progress - " + rundata->getCurrentSample());
    logBrowser->clear();
    this->delayReadingStepsLog();
}

/**
 * @brief ProgressDialog::checkFiles, checks for new input files and reloads them
 */
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


    }

    this->rundata->setFileList(filesDetected);
}


/**
 * @brief ProgressDialog::processFinished, Reset the state after its finished, but don't clear everything
 * @param exitCode, the exit code,
 * @param exitStatus, the exit status
 */
void ProgressDialog::processFinished(int exitCode, QProcess::ExitStatus exitStatus){
    this->resetRunTab(true, false);
    this->setProcessToZero();
}


/*
 * Kill the current run. Clear everything.
 */
/**
 * @brief ProgressDialog::terminateRun
 * Kill the current run. Clear everything.
 */
void ProgressDialog::terminateRun(){
    if(myProcess !=0 && myProcess->Running == QProcess::Running)  {
        myProcess->kill();
        // dump out output from stdout to the other log
        QByteArray read = myProcess->readAll();
        standardOut->append(QString("Received a TERMINATION/KILL signal"));
    }

    this->resetRunTab(true);
}

/**
 * @brief ProgressDialog::setProcessTozero, this function sets the myProcess and process in rundata to
 * 0 to mean that no process is running
 */
void ProgressDialog::setProcessToZero() {
    myProcess = 0;
    this->rundata->setProcess(myProcess);
}


/**
 * @brief ProgressDialog::updateOverwriteChoice, updates the overwrite choice
 * once the checkbox is changed
 *
 */
void ProgressDialog::updateOverwriteChoice() {
   if (this->shouldOverwrite())
      this->rundata->setValue("overwrite", "overwrite", _PARAMS);
   else
      this->rundata->setValue("overwrite", "overlay", _PARAMS);
}


/**
 * @brief ProgressDialog::shouldOverwrite, check if the
 * should overwrite previous results flag is on
 * @return
 */
bool ProgressDialog::shouldOverwrite() {

    return this->overwrite->isChecked();
}

/**
 * @brief ProgressDialog::~ProgressDialog, destructor for the ProgressDialog
 * at the end
 */
ProgressDialog::~ProgressDialog()
{
    myProcess->kill();
    timer->stop();
    delete ui;
}
