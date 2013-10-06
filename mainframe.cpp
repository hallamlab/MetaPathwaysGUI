#include "mainframe.h"
#include "ui_mainframe.h"
#include <fstream>
#include <iostream>
#include <QSettings>

#include <QStylePainter>

const QString MainFrame::TEMPLATE_PARAM = "template_param.txt";
const QString MainFrame::DEFAULT_TEMPLATE_PARAM = "default_template_param.txt";
const QString MainFrame::TEMPLATE_CONFIG = "template_config.txt";
const QString MainFrame::DEFAULT_TEMPLATE_CONFIG = "default_template_config.txt";
QHash<QString, QString> *MainFrame::CONFIG = NULL;
QHash<QString, QString> *MainFrame::PARAMS = NULL;
QHash<QString, QString> *MainFrame::CONFIG_MAPPING = NULL;
#include <QtGui>

class CustomTabStyle : public QProxyStyle
{
public:


    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab)
            s.transpose();
        return s;
    }

/*

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel)
        {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option))
            {
                QStyleOptionTab opt(*tab);
                opt.shape = QTabBar::RoundedNorth;
               // opt.text = "whatever";
                QProxyStyle::drawControl(element, &opt, painter, widget);
                return;
            }
        }
        QProxyStyle::drawControl(element, option, painter, widget);
    }
*/
    void drawItemText ( QPainter * painter, const QRect & rectangle, int alignment, const QPalette & palette, bool enabled, const QString & text, QPalette::ColorRole textRole) const{
      painter->save();
    //  painter->translate(160,50);
      painter->rotate(90);
      QCommonStyle::drawItemText(painter,rectangle, alignment , palette, enabled, text, textRole);

      qDebug() << "Draw item " << text;
      painter->restore();
    }
};



MainFrame::MainFrame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainFrame)
{
    ui->setupUi(this);

    centralWidget = this->findChild<QWidget *>("centralWidget");
    actionSetup = this->findChild<QAction *>("actionSetup");
    actionRunSettings = this->findChild<QAction *>("actionRunSettings");
    actionProgress = this->findChild<QAction *>("actionProgress");
    actionGridProgress = this->findChild<QAction *>("actionGridProgress");
    actionResults = this->findChild<QAction *>("actionResults");
    toolBar = this->findChild<QToolBar *>("toolBar");
    leftToolBar = this->findChild<QToolBar *>("leftToolBar");
    stackedWidget = this->findChild<QStackedWidget *>("stackedWidget");
    actionSetupMenu = this->findChild<QAction *>("actionSetupMenu");
    actionAbout = this->findChild<QAction *>("actionAbout");

     leftToolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    leftToolBar->addWidget(new HTabWidget("htabwidget", ":images/cross.png"));
    leftToolBar->addWidget(new HTabWidget("htabwidget", ":images/cross.png"));
    leftToolBar->setStyleSheet("QToolBar{spacing: 2px;}");

    actionProgress->setDisabled(true);
    actionGridProgress->setDisabled(true);
    actionResults->setDisabled(true);



    setupWidget = 0;
    parentWidget = 0;
    resultWindow = 0;
    welcomeWindow = 0;

    this->setWindowTitle("MetaPathways 2.0");
    this->loadPathVariables();

    if (checkParams() && checkConfig()){
        MainFrame::CONFIG_MAPPING = Utilities::createMapping();
        MainFrame::CONFIG = Utilities::parseFile(this->pathMetaPathways + "/"+ TEMPLATE_CONFIG);
        MainFrame::PARAMS = Utilities::parseFile(this->pathMetaPathways + "/"+ TEMPLATE_PARAM);

        if(  !MainFrame::settingsAvailable() /*!Utilities::validateConfig(this->CONFIG)*/){
            warning->warning(0,"Configuration Invalid!","Ooops! \nYou appear to have a configuration file for me, but parameters may be missing! Please check that all required executables and folders are specified through setup.", QMessageBox::Ok);
            this->openSetup();
        }
    }else{

        this->setupConfig();
        this->setupParams();
        this->openSetup();
    }

    setupWidget = new Setup();
    parentWidget = new ParentWidget();
//    resultWindow = new ResultWindow();
    welcomeWindow = new Welcome();

    stackedWidget->addWidget(welcomeWindow);
    stackedWidget->addWidget(setupWidget);
    stackedWidget->addWidget(parentWidget);

    stackedWidget->setCurrentWidget(welcomeWindow);

    connect(actionSetup, SIGNAL(triggered()), this, SLOT(openSetup()));
    connect(actionRunSettings, SIGNAL(triggered()), this, SLOT(openParentWidget()));
    connect(actionProgress, SIGNAL(triggered()), this, SLOT(openResultWindow()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(displayWelcome()));
    connect(actionSetupMenu, SIGNAL(triggered()), this, SLOT(openSetup()));
    connect(actionProgress, SIGNAL(triggered()), this, SLOT(displayProgress()));
    connect(actionResults, SIGNAL(triggered()), this, SLOT(showResults()));

    connect(setupWidget, SIGNAL(continueFromSetup()), this, SLOT(continueFromSetupWidget()));
    connect(parentWidget, SIGNAL(continueFromParentSettings()), this, SLOT(continueFromParentSettingsWidget()));
    connect(parentWidget, SIGNAL(showResultsFromParentSettings()), this, SLOT(displayResults()));
}

void MainFrame::continueFromSetupWidget(){
    this->openParentWidget();
}

void MainFrame::continueFromParentSettingsWidget(){
    progress = new ProgressDialog(parentWidget);
    stackedWidget->addWidget(progress);
    stackedWidget->setCurrentWidget(progress);

    actionProgress->setDisabled(false);
    actionResults->setDisabled(false);
}

void MainFrame::displayResults(){
    resultWindow = new ResultWindow(progress);
    stackedWidget->addWidget(resultWindow);

   // QMdiArea *mdi = new QMdiArea(progress);
    //stackedWidget->addWidget(mdi);

}

void MainFrame::showResults(){
    stackedWidget->setCurrentWidget(resultWindow);
}

void MainFrame::displayProgress(){
    stackedWidget->setCurrentWidget(progress);
}

void MainFrame::displayWelcome(){
    stackedWidget->setCurrentWidget(welcomeWindow);
}

void MainFrame::openSetup(){
    stackedWidget->setCurrentWidget(setupWidget);
}

void MainFrame::openParentWidget(){
    stackedWidget->setCurrentWidget(parentWidget);
}

void MainFrame::openResultWindow(){
    stackedWidget->setCurrentWidget(resultWindow);
}

void MainFrame::loadPathVariables(){
    QSettings settings("HallamLab", "MetaPathways");
    pathMetaPathways =  settings.value("METAPATHWAYS_PATH").toString();
}

/*
 * Should setup settings.
 * Usually this lives in a template_config file. So if it doesn't exist, we'll have to create it.
 */
void MainFrame::setupConfig(){
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
    MainFrame::CONFIG = Utilities::parseFile(this->pathMetaPathways + "/" +TEMPLATE_CONFIG);
}

/*
 * Should setup config file.
 * If it doesn't exist, it will copy over a default template.
 * In both instances, a default parameter settings list will be loaded.
 */

void MainFrame::setupParams(){
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
    MainFrame::PARAMS = Utilities::parseFile(this->pathMetaPathways + "/" +TEMPLATE_PARAM);
}


bool MainFrame::settingsAvailable() {
    QSettings settings("HallamLab", "MetaPathways");
    if(
         settings.value("PYTHON_EXECUTABLE").toString().isEmpty()  || settings.value("PERL_EXECUTABLE").toString().isEmpty()\
        || settings.value("METAPATHWAYS_PATH").toString().isEmpty()  || settings.value("REFDBS").toString().isEmpty()
      ) {
        return false;
    }
    return true;

}

bool MainFrame::checkParams(){
    std::ifstream param_file( (this->pathMetaPathways + "/" + TEMPLATE_PARAM).toStdString().c_str());
    if (param_file) return true;
    else return false;
}

bool MainFrame::checkConfig(){
    std::ifstream config_file( (this->pathMetaPathways + "/" +TEMPLATE_CONFIG).toStdString().c_str());
    if (config_file) return true;
    else return false;
}

MainFrame::~MainFrame()
{
    delete ui;
}
