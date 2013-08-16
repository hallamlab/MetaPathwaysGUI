#include "resultpage.h"
#include "ui_resultpage.h"
#include <QFile>
#include <QMovie>
#include <QTextStream>
#include <QDebug>


ResultPage::ResultPage(RunData *run, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultPage)
{
    ui->setupUi(this);
    this->run = run;

    this->setWindowTitle("MetaPathways - Execution Status");
    this->setFixedWidth(522);
    this->setFixedHeight(627);

    groupBoxes = new QList<QGroupBox *>(this->findChildren<QGroupBox *>(QRegExp("^metapaths_steps*")));
    table = this->findChild<QTableWidget *>("tableWidget");
    fileSelectedPath = this->findChild<QLabel *>("fileBrowsePath");

    loadRunParams();
    initMapping();
    setStyling();

    timer = new QTimer(this);
    timer->start(1000);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateSteps()));
}

void ResultPage::setStyling(){
    QList<QGroupBox *>::iterator i;
    for (i = groupBoxes->begin(); i != groupBoxes->end(); ++i){
        QGroupBox *widget = *i;
        widget->setStyleSheet("border:0;");
        widget->setTitle("");
    }
    table->setColumnWidth(3,30);
    table->setHorizontalHeaderLabels(QString("run,skip,redo,").split(","));

}

void ResultPage::loadRunParams(){
    QList<QGroupBox *>::iterator i;
    for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){
        QGroupBox *temp = *i;
        QString stepName = temp->objectName();
        //get the name, look up in the hash the corresponding setting key value pair
        QString configKey = this->run->getConfigMapping()->key(stepName);
        QString configValue = this->run->getParams()->value(configKey);

        //get the name of the radiobutton on the form by isolating for caps from the step,
        //taking the step name, to lower
        QRegExp lowerCaseRegex("[a-z]+_?[a-z]+");
        QString radioButtonName = stepName.remove(lowerCaseRegex).toLower();

        QRadioButton *yesRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"YES");
        QRadioButton *redoRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"REDO");
        QRadioButton *skipRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"SKIP");

        if (configValue.operator ==("yes")) yesRadioButton->setChecked(true);
        else if (configValue.operator ==("skip")) skipRadioButton->setChecked(true);
        else redoRadioButton->setChecked(true);

        temp->setEnabled(false);
    }
}

/*
 * There is no mapping from the table to the groupboxes. Therefore, this function will declare
 * the mapping of a row index to its groupbox by augmenting TABLE_MAPPING.
 */
void ResultPage::initMapping(){
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

void ResultPage::updateSteps(){
    QFile inputFile("metapathways_steps_log.txt");

    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            colorRunConfig(line);
       }
    }
}

void ResultPage::colorRunConfig(QString line){
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
        QGroupBox *group = this->findChild<QGroupBox *>(stepName);
        stepName.remove(QRegExp("[a-z-]+_?")).toLower();


        if (!stepName.isEmpty()){
            QImage *img;
            QTableWidgetItem *item = new QTableWidgetItem();
            QMovie *loading = new QMovie(":/images/loading.gif");
            QLabel *imageLabel = new QLabel();

            //set to blank to start, in case it's changed since the last step
            this->table->removeCellWidget(TABLE_MAPPING->key("metapaths_steps"+stepName),3);
            this->table->setItem(TABLE_MAPPING->key("metapaths_steps"+stepName),3, NULL);

            if (status.operator ==("Failed")){
                img  = new QImage(":/images/cross.png");
                item->setData(Qt::DecorationRole, QPixmap::fromImage(*img).scaled(12,12));
                this->table->setItem(TABLE_MAPPING->key("metapaths_steps"+stepName),3, item);
            }else if (operation.operator ==("Running")){
                imageLabel->setFixedSize(30,25);
                imageLabel->setMovie(loading);
                imageLabel->updateGeometry();
                loading->start();
                this->table->setCellWidget(TABLE_MAPPING->key("metapaths_steps"+stepName),3,imageLabel);
            }else{
                img = new QImage(":/images/check.png");
                item->setData(Qt::DecorationRole, QPixmap::fromImage(*img).scaled(12,12));
                this->table->setItem(TABLE_MAPPING->key("metapaths_steps"+stepName),3, item);
            }
        }
    }
}

ResultPage::~ResultPage()
{
    delete ui;
}
