#include "RemoteForm.h"
#include "ui_RemoteForm.h"

RemoteForm::RemoteForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RemoteForm)
{
    ui->setupUi(this);

    QPushButton *connectButton = this->findChild<QPushButton *>("connectButton");
    QPushButton *cancelButton = this->findChild<QPushButton *>("cancelButton");

    connect(connectButton, SIGNAL(clicked()), this, SLOT(openContinue()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
}

void RemoteForm::openContinue(){

}

void RemoteForm::closeWindow(){
    close();
}

RemoteForm::~RemoteForm()
{
    delete ui;
}
