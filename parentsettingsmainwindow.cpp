#include "parentsettingsmainwindow.h"
#include "ui_parentsettingsmainwindow.h"
#include <QDebug>

ParentSettingsMainWindow::ParentSettingsMainWindow(MainWindow *mw, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ParentSettingsMainWindow)
{
    this->mw = mw;

    ui->setupUi(this);

    parentWidget = new ParentWidget(this);
    this->setCentralWidget(parentWidget);
    this->setWindowTitle("MetaPathways - Configure Run Settings");

    menu = this->findChild<QMenuBar *>("menubar");
    openSetupAction = this->findChild<QAction *>("openSetupWindow");
    openAboutAction = this->findChild<QAction *>("openAboutAction");

    this->show();
    connect(openSetupAction, SIGNAL(triggered()),this->mw,SLOT(openSetup()));

}

ParentSettingsMainWindow::~ParentSettingsMainWindow()
{
    delete ui;
}
