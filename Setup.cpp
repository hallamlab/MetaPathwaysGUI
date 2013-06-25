#include "Setup.h"
#include "ui_Setup.h"
#include "string.h"
#include "mainwindow.h"
#include "qfiledialog.h"
#include "qdebug.h"
#include "utilities.h"

Setup::Setup(QWidget *parent) : QWidget(parent), ui(new Ui::Setup)
{
    ui->setupUi(this);

    pythonLabel = this->findChild<QLabel *>("PYTHON_EXECUTABLE");
    perlLabel = this->findChild<QLabel *>("PERL_EXECUTABLE");
    metapathwaysLabel = this->findChild<QLabel *>("METAPATHWAYS_PATH");
    databaseLabel = this->findChild<QLabel *>("REFDBS");

    pythonBrowseButton = this->findChild<QPushButton *>("pythonBrowseButton");
    perlBrowseButton = this->findChild<QPushButton *>("perlBrowseButton");
    metapathwaysBrowseButton = this->findChild<QPushButton *>("metapathwaysBrowseButton");
    saveButton = this->findChild<QPushButton *>("saveButton");
    cancelButton = this->findChild<QPushButton *>("cancelButton");
    databaseButton = this->findChild<QPushButton *>("databaseButton");

    pythonPath = MainWindow::CONFIG->value("PYTHON_EXECUTABLE");
    perlPath = MainWindow::CONFIG->value("PERL_EXECUTABLE");
    mpPath = MainWindow::CONFIG->value("METAPATHWAYS_PATH");
    databasePath = MainWindow::CONFIG->value("REFDBS");

    if (pythonPath.isNull() || pythonPath.isEmpty()) pythonLabel->setText("Please select the path to your Python directory.");
    if (perlPath.isNull() || perlPath.isEmpty()) perlLabel->setText("Please select the path to your Perl directory.");
    if (mpPath.isNull() || mpPath.isEmpty()) metapathwaysLabel->setText("Please select the path to your MetaPathways directory.");
    if (databasePath.isNull() || databasePath.isEmpty()) databaseLabel->setText("Please select the path to your database directory.");

    connect(pythonBrowseButton, SIGNAL(clicked()), this, SLOT(pythonBrowse()));
    connect(perlBrowseButton, SIGNAL(clicked()), this, SLOT(perlBrowse()));
    connect(metapathwaysBrowseButton, SIGNAL(clicked()), this, SLOT(metapathwaysBrowse()));
    connect(databaseButton, SIGNAL(clicked()), this, SLOT(databaseBrowse()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveSetup()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelSetup()));
}

void Setup::databaseBrowse(){
    databasePath = QFileDialog::getExistingDirectory(this, tr("Select the directory where your databases are defined."),"/");
    databaseLabel->setText(databasePath);
}

void Setup::pythonBrowse(){
    pythonPath = QFileDialog::getOpenFileName(this,tr("Select Python Executable"),"/");
    pythonLabel->setText(pythonPath);
}

void Setup::perlBrowse(){
    perlPath = QFileDialog::getOpenFileName(this,tr("Select Perl Executable"),"/");
    perlLabel->setText(perlPath);
}

void Setup::metapathwaysBrowse(){
    mpPath = QFileDialog::getExistingDirectory(this,tr("Select MetaPathways Directory"),"/");
    metapathwaysLabel->setText(mpPath);
}

void Setup::saveSetup(){

    //write to file only if the user has provided input
    if (!pythonPath.isNull() && !pythonPath.isEmpty()) {
        MainWindow::CONFIG->operator []("PYTHON_EXECUTABLE") = pythonPath;
        Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "PYTHON_EXECUTABLE", pythonPath);
    }
    if (!perlPath.isNull() && !perlPath.isEmpty()) {
        MainWindow::CONFIG->operator []("PERL_EXECUTABLE") = perlPath;
        Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "PERL_EXECUTABLE", perlPath);
    }
    if (!mpPath.isNull() && !mpPath.isEmpty()) {
        MainWindow::CONFIG->operator []("METAPATHWAYS_PATH") = mpPath;
        Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "METAPATHWAYS_PATH", mpPath);
    }
    if (!databasePath.isNull() && !databasePath.isEmpty()) {
        MainWindow::CONFIG->operator []("REFDBS") = databasePath;
        Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "REFDBS", databasePath);
    }

    close();
}

void Setup::cancelSetup(){
    close();
}

Setup::~Setup()
{
    delete ui;
}
