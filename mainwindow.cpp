#include "mainwindow.h"
#include <QString>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <QSettings>


const QString MainWindow::TEMPLATE_PARAM = "template_param.txt";
const QString MainWindow::DEFAULT_TEMPLATE_PARAM = "default_template_param.txt";
const QString MainWindow::TEMPLATE_CONFIG = "template_config.txt";
const QString MainWindow::DEFAULT_TEMPLATE_CONFIG = "default_template_config.txt";
QHash<QString, QString> *MainWindow::CONFIG = NULL;
QHash<QString, QString> *MainWindow::PARAMS = NULL;
QHash<QString, QString> *MainWindow::CONFIG_MAPPING = NULL;

MainWindow::MainWindow(){
    //display startup screen
    Welcome *welcomeScreen = new Welcome();
    welcomeScreen->show();
    sleep(2);
    welcomeScreen->close();

    setupWindow = 0;
    parent = 0;
    warning = new QMessageBox();

    this->loadPathVariables();


    MainWindow::CONFIG_MAPPING = Utilities::createMapping();
    if (checkParams() && checkConfig()){
        MainWindow::CONFIG = Utilities::parseFile(this->pathMetaPathways + "/"+ TEMPLATE_CONFIG);
        MainWindow::PARAMS = Utilities::parseFile(this->pathMetaPathways + "/"+ TEMPLATE_PARAM);

        if(  !MainWindow::settingsAvailable() /*!Utilities::validateConfig(this->CONFIG)*/){
            warning->warning(0,"Configuration Invalid!","Ooops! \nYou appear to have a configuration file for me, but parameters may be missing! Please check that all required executables and folders are specified through setup.", QMessageBox::Ok);
            this->openSetup();
        }else{
            startRun();
        }
    }else{
        setupConfig();
        setupParams();
        openSetup();
    }
}

void MainWindow::startRun(){
    if (parent) parent->show();
    else{
        parent = new ParentSettingsMainWindow(this);
    }
}


void MainWindow::loadPathVariables() {
     QSettings settings("HallamLab", "MetaPathways");
     pathMetaPathways =  settings.value("METAPATHWAYS_PATH").toString();
}

void MainWindow::openSetup(){
    if (setupWindow) setupWindow->show();
    else {
        setupWindow = new Setup(0, this);
        setupWindow->show();
    }
}

MainWindow::~MainWindow(){
    delete this;
}

bool MainWindow::checkParams(){
    std::ifstream param_file( (this->pathMetaPathways + "/" + TEMPLATE_PARAM).toStdString().c_str());
    if (param_file) return true;
    else return false;
}

bool MainWindow::checkConfig(){
    std::ifstream config_file( (this->pathMetaPathways + "/" +TEMPLATE_CONFIG).toStdString().c_str());
    if (config_file) return true;
    else return false;
}

/*
 * Should setup settings.
 * Usually this lives in a template_config file. So if it doesn't exist, we'll have to create it.
 */
void MainWindow::setupConfig(){
    std::ifstream config_file((this->pathMetaPathways + "/" +TEMPLATE_CONFIG).toStdString().c_str());
    if (!config_file){
        qDebug() << "no config file, copying";
        std::ifstream defaultConfig((this->pathMetaPathways + "/" +DEFAULT_TEMPLATE_CONFIG).toStdString().c_str());
        std::ofstream configFile;

        configFile.open((this->pathMetaPathways + "/" +TEMPLATE_CONFIG).toStdString().c_str());
        configFile << defaultConfig.rdbuf();
        configFile.close();
    }
    qDebug() << "loading param file";
    MainWindow::CONFIG = Utilities::parseFile(this->pathMetaPathways + "/" +TEMPLATE_CONFIG);
}

/*
 * Should setup config file.
 * If it doesn't exist, it will copy over a default template.
 * In both instances, a default parameter settings list will be loaded.
 */

void MainWindow::setupParams(){
    std::ifstream ifile((this->pathMetaPathways + "/" + TEMPLATE_PARAM).toStdString().c_str());
    if (!ifile){
        qDebug() << "param file doesnt exist, copying template file to param";
        std::ofstream param_config;
        std::ifstream defaultConfig((this->pathMetaPathways + "/" +DEFAULT_TEMPLATE_PARAM).toStdString().c_str());

        param_config.open((this->pathMetaPathways + "/" +TEMPLATE_PARAM).toStdString().c_str());
        param_config << defaultConfig.rdbuf();
        param_config.close();
    }
    qDebug() << "loading param file";
    MainWindow::PARAMS = Utilities::parseFile(this->pathMetaPathways + "/" +TEMPLATE_PARAM);
}


bool MainWindow::settingsAvailable() {
    QSettings settings("HallamLab", "MetaPathways");
    if(
         settings.value("PYTHON_EXECUTABLE").toString().isEmpty()  || settings.value("PERL_EXECUTABLE").toString().isEmpty()\
        || settings.value("METAPATHWAYS_PATH").toString().isEmpty()  || settings.value("REFDBS").toString().isEmpty()
      ) {

        return false;

    }

    return true;

}
