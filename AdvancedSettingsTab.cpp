#include "AdvancedSettingsTab.h"
#include "ui_AdvancedSettingsTab.h"

AdvancedSettingsTab::AdvancedSettingsTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdvancedSettingsTab)
{
    ui->setupUi(this);
}

AdvancedSettingsTab::~AdvancedSettingsTab()
{
    delete ui;
}
