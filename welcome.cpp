#include "welcome.h"
#include "ui_welcome.h"
#include <QPixmap>
#include <QLabel>

Welcome::Welcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Welcome)
{
    this->setWindowFlags( Qt::CustomizeWindowHint );

    ui->setupUi(this);
    QLabel *logo = this->findChild<QLabel *>("logo");
    logo->setPixmap(QPixmap(":/images/logo.png"));
}

Welcome::~Welcome()
{
    delete ui;
}
