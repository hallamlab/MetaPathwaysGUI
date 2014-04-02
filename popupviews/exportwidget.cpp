#include "exportwidget.h"
#include "ui_exportwidget.h"

ExportWidget::ExportWidget(TableData* td, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExportWidget)
{
    ui->setupUi(this);

    this->td = td;

    QGroupBox *groupBox = new QGroupBox(tr("Exclusive Radio Buttons"));

    QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
    QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
    QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));

    radio1->setChecked(true);
}

ExportWidget::~ExportWidget()
{
    delete ui;
}
