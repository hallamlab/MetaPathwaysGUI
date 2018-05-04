#include "dashboard.h"
#include "ui_DashBoard.h"

/*
DashBoard::DashBoard(QWidget *parent) : QWidget(parent)
{

}*/



DashBoard *DashBoard::dashboard=0;

DashBoard *DashBoard::getDashBoard() {
    if( DashBoard::dashboard == 0 ) {
        DashBoard::dashboard = new DashBoard();
    }

    return DashBoard::dashboard;
}


DashBoard::DashBoard(QWidget *parent):
    QWidget(parent)//, ui(new Ui::DashBoard)
{
  //this->setupUi(ui);
}


DashBoard::~DashBoard() {
    delete ui;
}

