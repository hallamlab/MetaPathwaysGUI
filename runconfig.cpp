#include "RunConfig.h"
#include "ui_RunConfig.h"
#include "mainframe.h"
#include <QRegExp>
#include <QObject>
#include "QDebug"
#include <QFileDialog>

RunConfig::RunConfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunConfig)
{
    ui->setupUi(this);

    groupBoxes = new QList<QGroupBox *>(this->findChildren<QGroupBox *>(QRegExp("^metapaths_steps*")));
    table = this->findChild<QTableWidget *>("tableWidget");
    runAll = this->findChild<QRadioButton *>("runAllRadioButton");
    skipAll = this->findChild<QRadioButton *>("skipAllRadioButton");
    redoAll = this->findChild<QRadioButton *>("redoAllRadioButton");
    filesSelected = this->findChild<QLabel *>("fileInput");
    runOptionsGroupBox = this->findChild<QGroupBox *>("runOptionsGroupBox");

    fileInputFormat = this->findChild<QComboBox *>("fileInputFormat");
    fileBrowseButton = this->findChild<QPushButton *>("fileBrowseButton");

    folderSelected= this->findChild<QLabel *>("outputFolderName");
    folderBrowseButton = this->findChild<QPushButton *>("outputFolderBrowser");

    sampleWarning = this->findChild<QLabel *>("sampleWarning");
    gridBlastChoice = this->findChild<QCheckBox *>("blastWithGrid");
    setupGrids = this->findChild<QPushButton *>("setupGrids");
    overwrite = this->findChild<QCheckBox *>("overwrite");

    gridSetup = 0;
    selectedFiles = 0;
    selectedFolder =0;

    rundata = RunData::getRunData();


    setStyling();
    loadRunParams();


    connect(skipAll, SIGNAL(clicked()), this, SLOT(toggleAllSkip()));
    connect(redoAll, SIGNAL(clicked()), this, SLOT(toggleAllRedo()));
    connect(runAll, SIGNAL(clicked()), this, SLOT(toggleAllRun()));
    connect(fileBrowseButton, SIGNAL(clicked()), this, SLOT(browseFile()));
    connect(folderBrowseButton, SIGNAL(clicked()), this, SLOT(browseFolder()));

    connect(setupGrids, SIGNAL(clicked()), this, SLOT(specifyGrid()));
}

void RunConfig::loadRunParams(){
    RunData *run = RunData::getRunData();
    QString key = run->getConfigMapping().key(fileInputFormat->objectName());
    QString value =run->getValueFromHash(key, _PARAMS);
    fileInputFormat->setCurrentIndex(fileInputFormat->findText(value));

    QList<QGroupBox *>::iterator i;
    for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){
        QGroupBox *temp = *i;
        QString stepName = temp->objectName();
        //get the name, look up in the hash the corresponding setting key value pair
        QString configKey = run->getConfigMapping().key(stepName);
        QString configValue = run->getValueFromHash(configKey,_PARAMS);

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
    }
}

void RunConfig::specifyGrid(){
    if (gridSetup){
        qDebug() << "making a new grid";
        delete gridSetup;
        gridSetup = new GridSetup();
        gridSetup->show();
    }else{
        qDebug() << "create new grid setup";
        gridSetup = new GridSetup();
        gridSetup->show();
    }
}

void RunConfig::browseFile(){
    selectedFiles = new QString(QFileDialog::getExistingDirectory(this, tr("Select a directory with your files.")));
    filesSelected->setText(*selectedFiles);

    this->rundata->setCurrentSample(QString());

    this->rundata->setValue("fileInput", *selectedFiles, _PARAMS);
    //send a signal to the parent to enable the continue button
    if (selectedFiles){
        if( selectedFolder != 0 ) emit fileSet();
        sampleWarning->hide();
    }
}

void RunConfig::browseFolder(){
    selectedFolder = new QString(QFileDialog::getExistingDirectory(this, tr("Select a directory for your output.")));
    folderSelected->setText(*selectedFolder);

    this->rundata->setValue("folderOutput", *selectedFolder, _PARAMS);
    //send a signal to the parent to enable the continue button
    if (selectedFolder){
        if(selectedFiles!=0) emit fileSet();
        sampleWarning->hide();
    }
}


void RunConfig::toggleAllRun(){
    QList<QGroupBox *>::iterator i;
    for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){
        QGroupBox *temp = *i;
        QString stepName = temp->objectName();
        //get the name of the radiobutton on the form by isolating for caps from the step,
        //taking the step name, to lower
        QRegExp lowerCaseRegex("[a-z]+_?[a-z]+");
        QString radioButtonName = stepName.remove(lowerCaseRegex).toLower();

        QRadioButton *yesRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"YES");

        yesRadioButton->setChecked(true);
    }
}

void RunConfig::toggleAllSkip(){

    QList<QGroupBox *>::iterator i;
    for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){
        QGroupBox *temp = *i;
        QString stepName = temp->objectName();
        //get the name, look up in the hash the corresponding setting key value pair


        //get the name of the radiobutton on the form by isolating for caps from the step,
        //taking the step name, to lower
        QRegExp lowerCaseRegex("[a-z]+_?[a-z]+");
        QString radioButtonName = stepName.remove(lowerCaseRegex).toLower();

        QRadioButton *skipRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"SKIP");

        skipRadioButton->setChecked(true);
    }
}

void RunConfig::toggleAllRedo(){
    QList<QGroupBox *>::iterator i;
    for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){
        QGroupBox *temp = *i;
        QString stepName = temp->objectName();
        //get the name, look up in the hash the corresponding setting key value pair


        //get the name of the radiobutton on the form by isolating for caps from the step,
        //taking the step name, to lower
        QRegExp lowerCaseRegex("[a-z]+_?[a-z]+");
        QString radioButtonName = stepName.remove(lowerCaseRegex).toLower();

        QRadioButton *redoRadioButton = temp->findChild<QRadioButton *>(radioButtonName+"REDO");

        redoRadioButton->setChecked(true);
    }
}

void RunConfig::closeWindow(){
    close();
}

void RunConfig::setStyling(){
    QList<QGroupBox *>::iterator i;
    for (i = groupBoxes->begin(); i != groupBoxes->end(); ++i){
        QGroupBox *widget = *i;
        widget->setStyleSheet("border:0;");
        widget->setTitle("");
    }
    runOptionsGroupBox->setStyleSheet("border:0");
    runOptionsGroupBox->setTitle("");

    table->horizontalHeader()->setStretchLastSection(true);

    table->setColumnWidth(3,30);
    table->setHorizontalHeaderLabels(QString("run,skip,redo,").split(","));

    //set grid checkbox
    RunData *rundata = RunData::getRunData();
    if (rundata->getValueFromHash("metapaths_steps:BLAST_REFDB",_PARAMS)=="grid"){
        gridBlastChoice->setChecked(true);
    }

}

RunConfig::~RunConfig()
{
    delete ui;
}
