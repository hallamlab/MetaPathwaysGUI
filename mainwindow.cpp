#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fstream.h"
#include "qstring.h"
#include "iostream.h"
#include "qdebug.h"

const QString MainWindow::TEMPLATE_PARAM = "template_param.txt";
const QString MainWindow::DEFAULT_TEMPLATE_PARAM = "default_template_param.txt";
const QString MainWindow::TEMPLATE_CONFIG = "template_config.txt";
const QString MainWindow::DEFAULT_TEMPLATE_CONFIG = "default_template_config.txt";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    qDebug() << "initializing config file";

    setupButton = this->findChild<QPushButton *>("setupButton");
    startButton = this->findChild<QPushButton *>("startButton");
    remoteButton = this->findChild<QPushButton *>("remoteButton");
    logo = this->findChild<QGraphicsView *>("logo");
    warningLabel = this->findChild<QLabel *>("warningLabel");

    QGraphicsScene *imageScene = new QGraphicsScene();
    QPixmap image("logo.png");
    imageScene->setForegroundBrush(image.scaled(QSize(200,160), Qt::KeepAspectRatio));
    imageScene->setSceneRect(10,10,180,150);
    logo->setScene(imageScene);

    connect(setupButton, SIGNAL(clicked()), this, SLOT(openSetup()));
    connect(startButton, SIGNAL(clicked()), this, SLOT(openSettings()));
    connect(remoteButton, SIGNAL(clicked()), this, SLOT(openRemoteJob()));

    checkParams();
    checkConfig();
}

void MainWindow::openRemoteJob(){
    remoteWindow = new RemoteForm();
    remoteWindow->show();
}

void MainWindow::openSettings(){
    settingsWindow = new ParentSettings();
    settingsWindow->show();
}

void MainWindow::openSetup(){
    if (setupWindow) setupWindow->show();
    else {
        setupWindow = new Setup();
        setupWindow->show();
    }
}

MainWindow::~MainWindow(){
    delete ui;
}

/*
 * Should check that the settings file exists.
 * Usually this lives in a template_config file. So if it doesn't exist, we'll have to create it.
 */
bool MainWindow::checkConfig(){
    ifstream config_file(TEMPLATE_CONFIG.toStdString().c_str());

    if (config_file.good()){
        warningLabel->hide();
        loadConfig(TEMPLATE_CONFIG);
    }else{
        std::ifstream defaultConfig(DEFAULT_TEMPLATE_CONFIG.toStdString().c_str());
        std::ofstream configFile;
        configFile.open(TEMPLATE_CONFIG.toStdString().c_str());
        configFile << defaultConfig.rdbuf();
        configFile.close();

        warningLabel->show();
        startButton->setEnabled(false);
    }
}

/*
 * This should check if the config file exists already.
 * If it doesn't exist, it will create an empty shell config to be populated with settings.
 * If it exists, should call loadConfig.
 */

void MainWindow::checkParams(){
    ifstream ifile(TEMPLATE_PARAM.toStdString().c_str());
    if (ifile.good()){
        qDebug() << "config file exists, now loading";
        loadParams(TEMPLATE_PARAM);
    }else{
        qDebug() << "config file doesnt exist, copying template file to config";
        std::ofstream param_config;
        std::ifstream defaultConfig(DEFAULT_TEMPLATE_PARAM.toStdString().c_str());

        param_config.open(TEMPLATE_PARAM.toStdString().c_str());
        param_config << defaultConfig.rdbuf();
        param_config.close();
    }
}

void MainWindow::loadConfig(QString TEMPLATE_CONFIG){
    CONFIG = Utilities::parseConfig(TEMPLATE_CONFIG);
}

void MainWindow::loadParams(QString TEMPLATE_PARAM){
    PARAMS = Utilities::parseParams(TEMPLATE_PARAM);
}
