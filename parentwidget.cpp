#include "parentwidget.h"
#include "ui_parentwidget.h"

ParentWidget::ParentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParentWidget)
{
    ui->setupUi(this);
}

ParentWidget::~ParentWidget()
{
    delete ui;
}
