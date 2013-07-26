#include "gridsetup.h"
#include "ui_gridsetup.h"
#include "mainwindow.h"
#include <QDebug>

GridSetup::GridSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GridSetup)
{
    ui->setupUi(this);
    this->setWindowTitle("MetaPathways - Grid Setup");

    gridSelection = this->findChild<QComboBox *>("gridSelection");
    startMessage = this->findChild<QLabel *>("startMessage");
    wid = this->findChild<QWidget *>("widget");

    initGridValues();
    initSelectChoices();
    initForm(gridSelection->currentText());

    connect(gridSelection,SIGNAL(currentIndexChanged(QString)),this,SLOT(initForm(QString)));

}

void GridSetup::initForm(QString selected){
    Grid *g = MainWindow::GRIDS->value(selected);

    if (!g->values->value("type").compare("AWS")){
        //AWS Config is shown
        EC2Grid *EC2Form = new EC2Grid();
        wid = EC2Form;
        qDebug() << "making AWS config";
    }
    else{
        //normal config is shown
        NonEC2 *normalForm = new NonEC2();
        wid = normalForm;
        qDebug() << "making normal config";
    }
}

void GridSetup::initSelectChoices(){
    QStringList grids = MainWindow::GRIDS->keys();
    gridSelection->addItems(grids);
    if (gridSelection->currentIndex()!=-1){
        startMessage->hide();
    }
}

void GridSetup::initGridValues(){
    QHashIterator<QString,QString> it(*MainWindow::PARAMS);
    MainWindow::GRIDS = new QHash<QString, Grid*>();

    while(it.hasNext()){
        it.next();

        QRegExp gridRegExp("grid_engine\\d+:");
        if (it.key().contains(gridRegExp)){
            QRegExp splitReg(":");
            QStringList split = it.key().split(splitReg);

            //check if this grid exists in our hash, if not, add it
            QString gridName = split[0];
            QString param = split[1];

            Grid *g = MainWindow::GRIDS->value(gridName);
            if (!g){
                g = new Grid();
                MainWindow::GRIDS->operator [](gridName) = g;
            }
            g->values->insert(param,it.value());
        }
    }
}

GridSetup::~GridSetup()
{
    delete ui;
}
