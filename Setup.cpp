#include "Setup.h"
#include "ui_Setup.h"
#include "utilities.h"
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QSettings>

Setup::Setup(QWidget *parent, MainWindow *mw) : QWidget(parent), ui(new Ui::Setup)
{
    ui->setupUi(this);
    this->mw = mw;

    this->setWindowTitle("MetaPathways - Setup");

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

    pythonExecTxt = this->findChild<QLineEdit *>("pythonExecTxt");
    perlExecTxt = this->findChild<QLineEdit *>("perlExecTxt");
    dbDirectoryTxt = this->findChild<QLineEdit *>("dbDirectoryTxt");
    pathMetaPathwaysTxt = this->findChild<QLineEdit *>("pathMetaPathwaysTxt");

    pythonPath = MainWindow::CONFIG->value("PYTHON_EXECUTABLE");
    perlPath = MainWindow::CONFIG->value("PERL_EXECUTABLE");
    mpPath = MainWindow::CONFIG->value("METAPATHWAYS_PATH");
    databasePath = MainWindow::CONFIG->value("REFDBS");


    if(!MainWindow::settingsAvailable()) this->canSave();

    if (pythonPath.isEmpty()) { pythonLabel->setText("Please select the path to your Python executable."); }
    else pythonLabel->setText(pythonPath);
    if (perlPath.isEmpty()) perlLabel->setText("Please select the path to your Perl executable.");
    else perlLabel->setText(perlPath);
    if (mpPath.isEmpty()) metapathwaysLabel->setText("Please select the path to your MetaPathways directory.");
    else metapathwaysLabel->setText(mpPath);
    if (databasePath.isEmpty()) databaseLabel->setText("Please select the path to your database directory.");
    else databaseLabel->setText(databasePath);

    connect(pythonBrowseButton, SIGNAL(clicked()), this, SLOT(pythonBrowse()));
    connect(perlBrowseButton, SIGNAL(clicked()), this, SLOT(perlBrowse()));
    connect(metapathwaysBrowseButton, SIGNAL(clicked()), this, SLOT(metapathwaysBrowse()));
    connect(databaseButton, SIGNAL(clicked()), this, SLOT(databaseBrowse()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveSetup()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelSetup()));
    connect(pythonExecTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave(QString )) );
    connect(perlExecTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave(QString )) );
    connect(dbDirectoryTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave(QString )) );
    connect(pathMetaPathwaysTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave(QString )) );


     QSettings settings("HallamLab", "MetaPathways");
     pythonExecTxt->setText(settings.value("PYTHON_EXECUTABLE").toString());
     perlExecTxt->setText(settings.value("PERL_EXECUTABLE").toString());
     pathMetaPathwaysTxt->setText(settings.value("METAPATHWAYS_PATH").toString());
     dbDirectoryTxt->setText(settings.value("REFDBS").toString());
     canSave();

}

void Setup::canSave(QString a){
    if (
          !(pythonPath.isEmpty() && pythonExecTxt->text().isEmpty()) \
       && !(perlPath.isEmpty() && perlExecTxt->text().isEmpty()) \
       && !(mpPath.isEmpty() && pathMetaPathwaysTxt->text().isEmpty()) \
       && !(databasePath.isEmpty() && dbDirectoryTxt->text().isEmpty() )
    )
    {
        saveButton->setEnabled(true);
        if(pythonPath.isEmpty())
           pythonPath = pythonExecTxt->text();
        if(perlPath.isEmpty())
           perlPath = perlExecTxt->text();
        if(mpPath.isEmpty())
           mpPath = pathMetaPathwaysTxt->text();
        if(databasePath.isEmpty())
            databasePath = dbDirectoryTxt->text();

    }
    else {
        saveButton->setEnabled(false);
    }
}

void Setup::databaseBrowse(){

    databasePath = QFileDialog::getExistingDirectory(this, tr("Select the directory where your databases are defined."));
    dbDirectoryTxt->setText(databasePath);
    databaseLabel->setText(databasePath);
    canSave();

}

void Setup::pythonBrowse(){

    pythonPath = QFileDialog::getOpenFileName(this,tr("Select Python Executable"));
    pythonExecTxt->setText(pythonPath);
    pythonLabel->setText(pythonPath);
    canSave();

}

void Setup::perlBrowse(){

    perlPath = QFileDialog::getOpenFileName(this,tr("Select Perl Executable"));
    perlExecTxt->setText(perlPath);
    perlLabel->setText(perlPath);
    canSave();

}

void Setup::metapathwaysBrowse(){

    mpPath = QFileDialog::getExistingDirectory(this,tr("Select MetaPathways Directory"));
    pathMetaPathwaysTxt->setText(mpPath);
    metapathwaysLabel->setText(mpPath);
    canSave();

}

void Setup::saveSetup(){

    QSettings settings("HallamLab", "MetaPathways");

    //write to file only if the user has provided input
    if (!pythonPath.isEmpty()) {
        MainWindow::CONFIG->operator []("PYTHON_EXECUTABLE") = pythonPath;
        settings.setValue("PYTHON_EXECUTABLE", pythonPath);
        Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "PYTHON_EXECUTABLE", pythonPath, false, false);
    }
    if (!perlPath.isEmpty()) {
        MainWindow::CONFIG->operator []("PERL_EXECUTABLE") = perlPath;
        settings.setValue("PERL_EXECUTABLE", perlPath);
        Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "PERL_EXECUTABLE", perlPath, false, false);
    }
    if (!mpPath.isEmpty()) {
        MainWindow::CONFIG->operator []("METAPATHWAYS_PATH") = mpPath;
        settings.setValue("METAPATHWAYS_PATH", mpPath);
        Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "METAPATHWAYS_PATH", mpPath, false, false);
    }
    if (!databasePath.isEmpty()) {
        MainWindow::CONFIG->operator []("REFDBS") = databasePath;
        settings.setValue("REFDBS", databasePath);
        Utilities::writeSettingToFile(MainWindow::TEMPLATE_CONFIG, "REFDBS", databasePath, false, false);
    }
    close();
    this->mw->startRun();

}

void Setup::cancelSetup(){
    close();
}

Setup::~Setup()
{
    delete ui;
}
