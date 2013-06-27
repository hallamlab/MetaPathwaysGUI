#include "RunConfig.h"
#include "ui_RunConfig.h"
#include "qregexp.h"
#include "qobject.h"
#include "QDebug"
#include "qfiledialog.h"
#include "mainwindow.h"

QComboBox *RunConfig::fileInputFormat = NULL;

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
    fileSelectedPath = this->findChild<QLabel *>("fileBrowsePath");
    runOptionsGroupBox = this->findChild<QGroupBox *>("runOptionsGroupBox");
    RunConfig::fileInputFormat = this->findChild<QComboBox *>("fileInputFormat");
    fileBrowseButton = this->findChild<QPushButton *>("fileBrowseButton");

    setStyling();
    loadRunParams();

    connect(skipAll, SIGNAL(clicked()), this, SLOT(toggleAllSkip()));
    connect(redoAll, SIGNAL(clicked()), this, SLOT(toggleAllRedo()));
    connect(runAll, SIGNAL(clicked()), this, SLOT(toggleAllRun()));
    connect(fileBrowseButton, SIGNAL(clicked()), this, SLOT(browseFile()));
}

void RunConfig::loadRunParams(){
    QString key = MainWindow::CONFIG_MAPPING->key(fileInputFormat->objectName());
    QString value = MainWindow::PARAMS->value(key);
    RunConfig::fileInputFormat->setCurrentIndex(RunConfig::fileInputFormat->findText(value));

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
    selectedFile = QFileDialog::getOpenFileName(this, tr("Select file run."),"/");
    fileSelectedPath->setText(selectedFile);
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
}

RunConfig::~RunConfig()
{
    delete ui;
}
