#include "gridchoice.h"
#include "ui_gridchoice.h"

GridChoice::GridChoice(QWidget *parent, GridSetup const *g) :
    QWidget(parent),
    ui(new Ui::GridChoice)
{
    ui->setupUi(this);
    this->setWindowTitle("MetaPathways - Grid Type");

    EC2Instance = this->findChild<QPushButton *>("EC2Instance");
    NonEC2Instance = this->findChild<QPushButton *>("NonEC2Instance");

    connect(EC2Instance, SIGNAL(clicked()), g, SLOT(newEC2Form()));
    connect(NonEC2Instance, SIGNAL(clicked()), g, SLOT(newNonEC2Form()));
}

GridChoice::~GridChoice()
{
    delete ui;
}
