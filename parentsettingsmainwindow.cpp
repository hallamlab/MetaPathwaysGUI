#include "parentsettingsmainwindow.h"
#include "ui_parentsettingsmainwindow.h"

ParentSettingsMainWindow::ParentSettingsMainWindow(MainWindow *mw, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ParentSettingsMainWindow)
{
    ui->setupUi(this);
    parentWidget = new ParentWidget(this);
    this->setCentralWidget(parentWidget);

    menu = this->findChild<QMenuBar *>("menubar");
    openSetupAction = this->findChild<QAction *>("openSetup");
    openAboutAction = this->findChild<QAction *>("openAbout");

    connect(openSetupAction, SIGNAL(triggered()),mw,SLOT(openSetup()));

}

ParentSettingsMainWindow::~ParentSettingsMainWindow()
{
    delete ui;
}
