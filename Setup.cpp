#include "Setup.h"
#include "ui_Setup.h"
#include "utilities.h"
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QSettings>

Setup::Setup(QWidget *parent) : QWidget(parent), ui(new Ui::Setup)
{
    ui->setupUi(this);

    pythonBrowseButton = this->findChild<QPushButton *>("pythonBrowseButton");
    perlBrowseButton = this->findChild<QPushButton *>("perlBrowseButton");
    metapathwaysBrowseButton = this->findChild<QPushButton *>("metapathwaysBrowseButton");
    saveButton = this->findChild<QPushButton *>("saveButton");
    databaseButton = this->findChild<QPushButton *>("databaseButton");

    pythonExecTxt = this->findChild<QLineEdit *>("pythonExecTxt");
    perlExecTxt = this->findChild<QLineEdit *>("perlExecTxt");
    dbDirectoryTxt = this->findChild<QLineEdit *>("dbDirectoryTxt");
    pathMetaPathwaysTxt = this->findChild<QLineEdit *>("pathMetaPathwaysTxt");

  //  if(!MainFrame::settingsAvailable()) this->canSave();

    RunData *rundata = RunData::getRunData();
    this->loadPathVariables();



    if( !rundata->getConfig().isEmpty() ){
        pythonExecTxt->setText(rundata->getValueFromHash("PYTHON_EXECUTABLE",_CONFIG));
        perlExecTxt->setText(rundata->getValueFromHash("PERL_EXECUTABLE",_CONFIG));
        dbDirectoryTxt->setText(rundata->getValueFromHash("REFDBS",_CONFIG));
        pathMetaPathwaysTxt->setText(rundata->getValueFromHash("METAPATHWAYS_PATH",_CONFIG));
    }

    connect(pythonBrowseButton, SIGNAL(clicked()), this, SLOT(pythonBrowse()));
    connect(perlBrowseButton, SIGNAL(clicked()), this, SLOT(perlBrowse()));
    connect(metapathwaysBrowseButton, SIGNAL(clicked()), this, SLOT(metapathwaysBrowse()));
    connect(databaseButton, SIGNAL(clicked()), this, SLOT(databaseBrowse()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveSetup()));
    connect(pythonExecTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave(QString )) );
    connect(perlExecTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave(QString )) );
    connect(dbDirectoryTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave(QString )) );
    connect(pathMetaPathwaysTxt, SIGNAL( textEdited(QString)), this, SLOT(canSave(QString )) );

     canSave();

}

void Setup::canSave(QString a){
    if (
          !(pythonExecTxt->text().isEmpty()) \
       && !(perlExecTxt->text().isEmpty()) \
       && !(pathMetaPathwaysTxt->text().isEmpty()) \
       && !(dbDirectoryTxt->text().isEmpty() )
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
    canSave();

}

void Setup::pythonBrowse(){

    pythonPath = QFileDialog::getOpenFileName(this,tr("Select Python Executable"));
    pythonExecTxt->setText(pythonPath);
    canSave();

}

void Setup::perlBrowse(){

    perlPath = QFileDialog::getOpenFileName(this,tr("Select Perl Executable"));
    perlExecTxt->setText(perlPath);
    canSave();

}

void Setup::metapathwaysBrowse(){

    mpPath = QFileDialog::getExistingDirectory(this,tr("Select MetaPathways Directory"));
    pathMetaPathwaysTxt->setText(mpPath);
    canSave();

}

void Setup::saveSetup(){

    RunData *rundata = RunData::getRunData();
    QHash<QString,QString> temp = rundata->getConfig();

    //write to file only if the user has provided input
    if (!pythonExecTxt->text().isEmpty()) {
        temp["PYTHON_EXECUTABLE"] = pythonExecTxt->text();
        Utilities::writeSettingToFile(RunData::TEMPLATE_CONFIG, "PYTHON_EXECUTABLE", pythonExecTxt->text(), false, false);
    }
    if (!perlExecTxt->text().isEmpty()) {
        temp["PERL_EXECUTABLE"] = perlExecTxt->text();
        Utilities::writeSettingToFile(RunData::TEMPLATE_CONFIG, "PERL_EXECUTABLE", perlExecTxt->text(), false, false);
    }
    if (!pathMetaPathwaysTxt->text().isEmpty()) {
        temp["METAPATHWAYS_PATH"] = pathMetaPathwaysTxt->text();
        Utilities::writeSettingToFile(RunData::TEMPLATE_CONFIG, "METAPATHWAYS_PATH", pathMetaPathwaysTxt->text(), false, false);
    }
    if (!dbDirectoryTxt->text().isEmpty()) {
        temp["REFDBS"] = dbDirectoryTxt->text();
        Utilities::writeSettingToFile(RunData::TEMPLATE_CONFIG, "REFDBS", dbDirectoryTxt->text(), false, false);
    }

    rundata->setConfig(temp);
    this->savePathVariables();
    emit continueFromSetup();
}


void Setup::loadPathVariables(){
    RunData *rundata = RunData::getRunData();
    QSettings settings("HallamLab", "MetaPathways");
    if( settings.allKeys().contains("METAPATHWAYS_PATH") ) {
        pathMetaPathwaysTxt->setText(settings.value("METAPATHWAYS_PATH").toString());
        rundata->setValue("METAPATHWAYS_PATH", pathMetaPathwaysTxt->text(), _CONFIG);
    }
    if( settings.allKeys().contains("PYTHON_EXECUTABLE") ) {
        pythonExecTxt->setText(settings.value("PYTHON_EXECUTABLE").toString());
        rundata->setValue("PYTHON_EXECUTABLE", pythonExecTxt->text(), _CONFIG);
    }
    if( settings.allKeys().contains("PERL_EXECUTABLE") ) {
        perlExecTxt->setText(settings.value("PERL_EXECUTABLE").toString());
        rundata->setValue("PERL_EXECUTABLE", perlExecTxt->text(), _CONFIG);
    }
    if( settings.allKeys().contains("REFDBS") ) {
        dbDirectoryTxt->setText(settings.value("REFDBS").toString());
        rundata->setValue("REFDBS", dbDirectoryTxt->text(), _CONFIG);
    }

    //rundata->setConfig(Utilities::parseFile(rundata->getValueFromHash("METAPATHWAYS_PATH", _CONFIG) +  "/"+ MainFrame::TEMPLATE_CONFIG));
  //  qDebug() << "param path " << rundata->getValueFromHash("METAPATHWAYS_PATH", _CONFIG)  + "/"+ MainFrame::TEMPLATE_PARAM;
    rundata->setParams(Utilities::parseFile(rundata->getValueFromHash("METAPATHWAYS_PATH", _CONFIG)  + "/"+ MainFrame::TEMPLATE_PARAM));
    rundata->setConfigMapping(Utilities::createMapping());


}

void Setup::savePathVariables(){
    QSettings settings("HallamLab", "MetaPathways");
    if( !pathMetaPathwaysTxt->text().isEmpty())
    settings.setValue("METAPATHWAYS_PATH", pathMetaPathwaysTxt->text());

    if( !pythonExecTxt->text().isEmpty())
    settings.setValue("PYTHON_EXECUTABLE", pythonExecTxt->text());

    if( !perlExecTxt->text().isEmpty())
    settings.setValue("PERL_EXECUTABLE", perlExecTxt->text());

    if( !dbDirectoryTxt->text().isEmpty()) {
       settings.setValue("REFDBS",  dbDirectoryTxt->text());
       qDebug() << " saving " << dbDirectoryTxt->text();
    }
}


void Setup::cancelSetup(){
    close();
}

Setup::~Setup()
{
    delete ui;
}
