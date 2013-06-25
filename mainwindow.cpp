#include "mainwindow.h"
#include "ui_mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    setupWindow = NULL;

    setupButton = this->findChild<QPushButton *>("setupButton");
    startButton = this->findChild<QPushButton *>("startButton");
    remoteButton = this->findChild<QPushButton *>("remoteButton");
    logo = this->findChild<QGraphicsView *>("logo");
    warningLabel = this->findChild<QLabel *>("warningLabel");

    this->setFixedSize(786, 258);
    this->setWindowTitle("MetaPathways - Modular Pipeline for Metagenomic Analysis");

    QGraphicsScene *imageScene = new QGraphicsScene();
    QPixmap image("logo.png");
    imageScene->setForegroundBrush(image.scaled(QSize(200,160), Qt::KeepAspectRatio));
    imageScene->setSceneRect(10,10,180,150);
    logo->setScene(imageScene);

    connect(setupButton, SIGNAL(clicked()), this, SLOT(openSetup()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(openSettings()));
    connect(remoteButton, SIGNAL(clicked()), this, SLOT(openRemoteJob()));

    checkConfig();
    checkParams();
    createMapping();
}

void MainWindow::openRemoteJob(){
    remoteWindow = new RemoteForm();
    remoteWindow->show();
}

void MainWindow::openSettings(){
    parentWidget = new ParentWidget();
    parentWidget->show();
//    settingsWindow = new SettingsTab();
//    settingsWindow->show();
}

// Only one instance allowed of this window.
void MainWindow::openSetup(){
    if (setupWindow) setupWindow->show();
    else {
        setupWindow = new Setup();
        setupWindow->show();
    }
    startButton->setEnabled(true);
    warningLabel->hide();
}

MainWindow::~MainWindow(){
    delete ui;
}

/*
 * Should check that the settings file exists.
 * Usually this lives in a template_config file. So if it doesn't exist, we'll have to create it.
 */
bool MainWindow::checkConfig(){
    std::ifstream config_file(TEMPLATE_CONFIG.toStdString().c_str());
    if (!config_file){
        qDebug() << "no config file, copying";
        std::ifstream defaultConfig(DEFAULT_TEMPLATE_CONFIG.toStdString().c_str());
        std::ofstream configFile;
        configFile.open(TEMPLATE_CONFIG.toStdString().c_str());
        configFile << defaultConfig.rdbuf();
        configFile.close();

        warningLabel->show();
        startButton->setEnabled(false);
    }else{
        warningLabel->hide();
    }
    qDebug() << "loading param file";
    loadConfig(TEMPLATE_CONFIG);
}

/*
 * This should check if the config file exists already.
 * If it doesn't exist, it will copy over a default template.
 * In both instances, a default parameter settings list will be loaded.
 */

void MainWindow::checkParams(){
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
    loadParams(TEMPLATE_PARAM);
}

/*
 * Load config to CONFIG and mapping to CONFIG_MAPPING.
 */
void MainWindow::loadConfig(QString TEMPLATE_CONFIG){
    qDebug() << "calling utilities to parse config";
    MainWindow::CONFIG = Utilities::parseFile(TEMPLATE_CONFIG);
}

void MainWindow::loadParams(QString TEMPLATE_PARAM){
    MainWindow::PARAMS = Utilities::parseFile(TEMPLATE_PARAM);
}

void MainWindow::createMapping(){
    MainWindow::CONFIG_MAPPING = Utilities::createMapping();
}

