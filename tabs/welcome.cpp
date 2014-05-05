/*
 * Just shows the logo without a frame on the application.
 */

#include "welcome.h"
#include "ui_welcome.h"


Welcome::Welcome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Welcome)
{

    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    QLabel *logo = this->findChild<QLabel *>("logo");
    logo->setPixmap(QPixmap(":/images/logo.png"));
}

Welcome::~Welcome()
{
    delete ui;
}
