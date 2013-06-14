#include "ParentSettings.h"
#include "ui_ParentSettings.h"

ParentSettings::ParentSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParentSettings)
{
    ui->setupUi(this);

    QPushButton *cancelButton = this->findChild<QPushButton *>("Cancel");
    QPushButton *continueButton = this->findChild<QPushButton *>("Continue");

    QTabWidget *tabWidget = this->findChild<QTabWidget *>("tabWidget");
    AdvancedSettings = new AdvancedSettingsTab();
    basicSettingsTab = new SettingsTab();

    tabWidget->removeTab(0);
    tabWidget->removeTab(0);

    tabWidget->addTab(basicSettingsTab,"Basic Settings");
    tabWidget->addTab(AdvancedSettings, "Grid Settings (Optional)");

    tabWidget->setTabEnabled(0, true);

    tabWidget->show();

}

void ParentSettings::closeWindow(){

}

void ParentSettings::openParameterSetup(){

}

ParentSettings::~ParentSettings()
{
    delete ui;
}
