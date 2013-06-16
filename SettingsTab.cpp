#include "SettingsTab.h"
#include "ui_SettingsTab.h"

SettingsTab::SettingsTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsTab)
{
    ui->setupUi(this);
}

SettingsTab::~SettingsTab()
{
    delete ui;
}
