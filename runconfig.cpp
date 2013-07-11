#include "RunConfig.h"
#include "ui_RunConfig.h"
#include "mainwindow.h"
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
    filesSelected = this->findChild<QLabel *>("filesSelected");
    runOptionsGroupBox = this->findChild<QGroupBox *>("runOptionsGroupBox");
    fileInputFormat = this->findChild<QComboBox *>("fileInputFormat");
    fileBrowseButton = this->findChild<QPushButton *>("fileBrowseButton");
    selectedFiles = 0;

    setStyling();
    loadRunParams();

    connect(skipAll, SIGNAL(clicked()), this, SLOT(toggleAllSkip()));
    connect(redoAll, SIGNAL(clicked()), this, SLOT(toggleAllRedo()));
    connect(runAll, SIGNAL(clicked()), this, SLOT(toggleAllRun()));
    connect(fileBrowseButton, SIGNAL(clicked()), this, SLOT(browseFile()));
    connect(this, SIGNAL(fileSet()), this->parent(), SLOT(enableContinueButton()));
}

void RunConfig::loadRunParams(){
    QString key = MainWindow::CONFIG_MAPPING->key(fileInputFormat->objectName());
    QString value = MainWindow::PARAMS->value(key);
    fileInputFormat->setCurrentIndex(fileInputFormat->findText(value));

    QList<QGroupBox *>::iterator i;
    for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){
        QGroupBox *temp = *i;
        QString stepName = temp->objectName();
        //get the name, look up in the hash the corresponding setting key value pair
        QString configKey = MainWindow::CONFIG_MAPPING->key(stepName);
        QString configValue = MainWindow::PARAMS->value(configKey);

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

void RunConfig::browseFile(){
    QString selectedFileList = "";
    selectedFiles = new QStringList(QFileDialog::getOpenFileNames(this, tr("Select file run.")));
    for (int i=0;i < selectedFiles->size(); i++){
        QString file = selectedFiles->at(i).split("/").last();
        selectedFileList.append(file);
        selectedFileList.append(",");
    }
    filesSelected->setText(selectedFileList);
    //send a signal to the parent to enable the continue button
    if (!selectedFiles->empty()) emit fileSet();
}

void RunConfig::toggleAllRun(){
    QList<QGroupBox *>::iterator i;
    for (i = groupBoxes->begin(); i!= groupBoxes->end(); ++i){
        QGroupBox *temp = *i;
        QString stepName = temp->objectName();
        //get the name, look up in the hash the corresponding setting key value pair
        QString configKey = MainWindow::CONFIG_MAPPING->key(stepName);
        QString configValue = MainWindow::CONFIG->value(configKey);

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
        QString configKey = MainWindow::CONFIG_MAPPING->key(stepName);
        QString configValue = MainWindow::CONFIG->value(configKey);

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
        QString configKey = MainWindow::CONFIG_MAPPING->key(stepName);
        QString configValue = MainWindow::CONFIG->value(configKey);

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

}

RunConfig::~RunConfig()
{
    delete ui;
}
