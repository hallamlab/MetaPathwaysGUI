#include "ProgressDialog.h"
#include "ui_ProgressDialog.h"
#include "qprocess.h"
#include <QPushButton>
#include <QDebug>
#include <QString>
#include <QFile>
#include "utilities.h"
#include "rundata.h"
#include "resultwindow.h"
#include <QMovie>

ProgressDialog::ProgressDialog(ParentWidget *pw, RunData *run, QWidget *parent) : QWidget(parent), ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    this->run = run;
    this->pw = pw;
    this->RUN_RESULT = run->getRunResults();
    this->RUN_RESULT = new QHash<QString,QString>();

    cancelButton = this->findChild<QPushButton *>("cancelButton");
    textBrowser = this->findChild<QTextBrowser *>("textBrowser");
    progressBar = this->findChild<QProgressBar *>("progressBar");

    initProgressBar();
    initMapping();

    timer = new QTimer(this);
    timer->start(1000);

//    QString program = "/usr/bin/python";
//    QStringList arguments;
//    arguments <<  "/Users/michaelwu/workspace/MetaPathways2-build-Desktop-Release/MetaPathways2.app/Contents/MacOS/test.py";
//    qDebug() << "declaring program";
//    myProcess = new QProcess();
//    qDebug() << "starting program";
//    myProcess->start(program, arguments);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(terminateRun()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateText()));
}

void ProgressDialog::initProgressBar(){
    progressBar->setMaximum(Utilities::countRunSteps(run->getParams()));
    progressBar->setMinimum(0);

}

void ProgressDialog::updateText(){
    textBrowser->clear();
    QFile inputFile("metapathways_steps_log.txt");

    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            textBrowser->append(line);

            colorRunConfig(line);
       }
    }
}

/*
 * There is no mapping from the table to the groupboxes. Therefore, this function will declare
 * the mapping of a row index to its groupbox by augmenting TABLE_MAPPING.
 */
void ProgressDialog::initMapping(){
    TABLE_MAPPING = new QHash<int,QString>();

    TABLE_MAPPING->operator [](0) = "metapaths_stepsPREPROCESS_FASTA";
    TABLE_MAPPING->operator [](1) = "metapaths_stepsORF_PREDICTION";
    TABLE_MAPPING->operator [](2) = "metapaths_stepsGFF_TO_AMINO";
    TABLE_MAPPING->operator [](3) = "metapaths_stepsFILTERED_FASTA";
    TABLE_MAPPING->operator [](4) = "metapaths_stepsCOMPUTE_REFSCORE";
    TABLE_MAPPING->operator [](5) = "metapaths_stepsBLAST_REFDB";
    TABLE_MAPPING->operator [](6) = "metapaths_stepsPARSE_BLAST";
    TABLE_MAPPING->operator [](7) = "metapaths_stepsSCAN_RRNA";
    TABLE_MAPPING->operator [](8) = "metapaths_stepsSCAN_TRNA";
    TABLE_MAPPING->operator [](9) = "metapaths_stepsSTATS_RRNA";
    TABLE_MAPPING->operator [](10) = "metapaths_stepsANNOTATE";
    TABLE_MAPPING->operator [](11) = "metapaths_stepsPATHOLOGIC_INPUT";
    TABLE_MAPPING->operator [](12) = "metapaths_stepsGENBANK_FILE";
    TABLE_MAPPING->operator [](13) = "metapaths_stepsCREATE_SEQUIN_FILE";
    TABLE_MAPPING->operator [](14) = "metapaths_stepsCREATE_REPORT_FILES";
    TABLE_MAPPING->operator [](15) = "metapaths_stepsMLTREEMAP_CALCULATION";
    TABLE_MAPPING->operator [](16) = "metapaths_stepsMLTREEMAP_IMAGEMAKER";
    TABLE_MAPPING->operator [](17) = "metapaths_stepsPATHOLOGIC";
}

void ProgressDialog::colorRunConfig(QString line){
    QRegExp whiteSpace("\\s");
    QRegExp commentLine("#[^\"\\n\\r]*");

    if (!commentLine.exactMatch(line) && line.length()>0){
        //if not a comment line
        QStringList splitList = line.split(whiteSpace);
        QString step = splitList.at(0).trimmed();
        QString operation = splitList.at(1).trimmed();        

        QString status;
        if (splitList.size()==3) status = splitList.at(2).trimmed();

        //find the step equivalent on the chart
        QString stepName = run->getConfigMapping()->operator [](step);
        QString untrimmed = stepName;
        QGroupBox *group = pw->findChild<QGroupBox *>(stepName);
        stepName.remove(QRegExp("[a-z-]+_?")).toLower();

        if (!stepName.isEmpty()){
            //save the run status
//            qDebug() << splitList << splitList.at(2).isNull();
//            this->RUN_RESULT->operator [](untrimmed) = splitList.at(1);
//            if (!(splitList.at(2).isEmpty())) this->RUN_RESULT->operator [](untrimmed) = splitList.at(1) + " " + splitList.at(2);

            QImage *img;
            QTableWidgetItem *item = new QTableWidgetItem();
            QMovie *loading = new QMovie("loading.gif");
            QLabel *imageLabel = new QLabel();

            //set to blank to start, in case it's changed since the last step
            pw->runConfigTab->table->removeCellWidget(TABLE_MAPPING->key("metapaths_steps"+stepName),3);
            pw->runConfigTab->table->setItem(TABLE_MAPPING->key("metapaths_steps"+stepName),3, NULL);

            if (status.operator ==("Failed")){
                img  = new QImage("cross.png");
                item->setData(Qt::DecorationRole, QPixmap::fromImage(*img).scaled(12,12));
                pw->runConfigTab->table->setItem(TABLE_MAPPING->key("metapaths_steps"+stepName),3, item);
            }else if (operation.operator ==("Running")){
                imageLabel->setFixedSize(30,25);
                imageLabel->setMovie(loading);
                imageLabel->updateGeometry();
                loading->start();
                pw->runConfigTab->table->setCellWidget(TABLE_MAPPING->key("metapaths_steps"+stepName),3,imageLabel);
            }else{
                img = new QImage("check.png");
                item->setData(Qt::DecorationRole, QPixmap::fromImage(*img).scaled(12,12));
                pw->runConfigTab->table->setItem(TABLE_MAPPING->key("metapaths_steps"+stepName),3, item);
            }
        }
    }
}

void ProgressDialog::terminateRun(){
    timer->stop();
    this->destroy();
    pw->close();

    ResultWindow *rw = new ResultWindow();
    rw->show();
}


ProgressDialog::~ProgressDialog()
{
    myProcess->kill();
    timer->stop();
    delete ui;
}
