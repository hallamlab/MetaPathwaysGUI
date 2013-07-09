#include "mainwindow.h"
#include "qstring.h"
#include "qdebug.h"
#include <fstream>
#include <iostream>

const QString MainWindow::TEMPLATE_PARAM = "template_param.txt";
const QString MainWindow::DEFAULT_TEMPLATE_PARAM = "default_template_param.txt";
const QString MainWindow::TEMPLATE_CONFIG = "template_config.txt";
const QString MainWindow::DEFAULT_TEMPLATE_CONFIG = "default_template_config.txt";
QHash<QString, QString> *MainWindow::CONFIG = NULL;
QHash<QString, QString> *MainWindow::PARAMS = NULL;
QHash<QString, QString> *MainWindow::CONFIG_MAPPING = NULL;

MainWindow::MainWindow(){
    setupWindow = 0;
    parentWidget = 0;

    MainWindow::CONFIG_MAPPING = Utilities::createMapping();
    if (checkParams() && checkConfig()){
        MainWindow::CONFIG = Utilities::parseFile(TEMPLATE_CONFIG);
        MainWindow::PARAMS = Utilities::parseFile(TEMPLATE_PARAM);
        startRun();
    }else{
        setupConfig();
        setupParams();
        openSetup();
    }
}

void MainWindow::startRun(){
    parentWidget = new ParentWidget();
    parentWidget->show();
}

void MainWindow::openSetup(){
    setupWindow = new Setup(0, this);
    setupWindow->show();
}

MainWindow::~MainWindow(){
    delete this;
}

bool MainWindow::checkParams(){
    std::ifstream param_file(TEMPLATE_PARAM.toStdString().c_str());
    if (param_file) return true;
    else return false;
}

bool MainWindow::checkConfig(){
    std::ifstream config_file(TEMPLATE_CONFIG.toStdString().c_str());
    if (config_file) return true;
    else return false;
}

/*
 * Should setup settings.
 * Usually this lives in a template_config file. So if it doesn't exist, we'll have to create it.
 */
void MainWindow::setupConfig(){
    std::ifstream config_file(TEMPLATE_CONFIG.toStdString().c_str());
    if (!config_file){
        qDebug() << "no config file, copying";
        std::ifstream defaultConfig(DEFAULT_TEMPLATE_CONFIG.toStdString().c_str());
        std::ofstream configFile;

        configFile.open(TEMPLATE_CONFIG.toStdString().c_str());
        configFile << defaultConfig.rdbuf();
        configFile.close();
    }
    qDebug() << "loading param file";
    MainWindow::CONFIG = Utilities::parseFile(TEMPLATE_CONFIG);
}

/*
 * Should setup config file.
 * If it doesn't exist, it will copy over a default template.
 * In both instances, a default parameter settings list will be loaded.
 */

void MainWindow::setupParams(){
    std::ifstream ifile(TEMPLATE_PARAM.toStdString().c_str());
    if (!ifile){
        qDebug() << "param file doesnt exist, copying template file to param";
        std::ofstream param_config;
        std::ifstream defaultConfig(DEFAULT_TEMPLATE_PARAM.toStdString().c_str());

        param_config.open(TEMPLATE_PARAM.toStdString().c_str());
        param_config << defaultConfig.rdbuf();
        param_config.close();
    }
    qDebug() << "loading param file";
    MainWindow::PARAMS = Utilities::parseFile(TEMPLATE_PARAM);
}


