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

    pythonBrowseButton = this->findChild<QPushButton *>("pythonBrowseButton");
    perlBrowseButton = this->findChild<QPushButton *>("perlBrowseButton");
    metapathwaysBrowseButton = this->findChild<QPushButton *>("metapathwaysBrowseButton");
    saveButton = this->findChild<QPushButton *>("saveButton");
    cancelButton = this->findChild<QPushButton *>("cancelButton");

    pythonLabel->setText(MainWindow::CONFIG->value("PYTHON_EXECUTABLE"));
    perlLabel->setText(MainWindow::CONFIG->value("PERL_EXECUTABLE"));
    metapathwaysLabel->setText(MainWindow::CONFIG->value("METAPATHWAYS_PATH"));

    connect(pythonBrowseButton, SIGNAL(clicked()), this, SLOT(pythonBrowse()));
    connect(perlBrowseButton, SIGNAL(clicked()), this, SLOT(perlBrowse()));
    connect(metapathwaysBrowseButton, SIGNAL(clicked()), this, SLOT(metapathwaysBrowse()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveSetup()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelSetup()));
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
    MainWindow::CONFIG->operator []("PYTHON_EXECUTABLE") = pythonPath;
    MainWindow::CONFIG->operator []("PERL_EXECUTABLE") = perlPath;
    MainWindow::CONFIG->operator []("METAPATHWAYS_PATH") = mpPath;

    //need to now write to file these settings
    Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "PYTHON_EXECUTABLE", pythonPath);
    Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "PERL_EXECUTABLE", perlPath);
    Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "METAPATHWAYS_PATH", mpPath);

    close();
}

void Setup::cancelSetup(){
    close();
}

Setup::~Setup()
{
    delete ui;
}
