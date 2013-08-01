#include "gridsetup.h"
#include "ui_gridsetup.h"
#include "mainwindow.h"
#include "Setup.h"
#include "gridchoice.h"
#include <QDebug>

GridSetup::GridSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GridSetup)
{
    ui->setupUi(this);

    this->setWindowTitle("MetaPathways - Grid Setup");

    gridSelection = this->findChild<QComboBox *>("gridSelection");
    startMessage = this->findChild<QLabel *>("startMessage");
    wid = this->findChild<QStackedWidget *>("stackedWidget");
    addGrid = this->findChild<QToolButton *>("addGrid");
    deleteGrid = this->findChild<QToolButton *>("deleteGrid");
    cancelButton = this->findChild<QPushButton *>("cancelButton");
    saveButton = this->findChild<QPushButton *>("saveButton");


    initGridValues();
    initSelectChoices();

    EC2Grids = new QHash<QString, EC2Grid*>();
    NonEC2Grids = new QHash<QString, NonEC2*>();

    if (gridSelection->count() > 0) {
        initForm(gridSelection->currentText());
    }

    connect(gridSelection,SIGNAL(currentIndexChanged(QString)),this,SLOT(initForm(QString)));
    connect(addGrid, SIGNAL(clicked()), this, SLOT(addNewGrid()));
    connect(deleteGrid, SIGNAL(clicked()), this, SLOT(deleteExistingGrid()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
}

void GridSetup::newEC2Form(){
    qDebug() << "new ec2 form";
    gc->close();

    QString name = QString("grid_engine%1").arg(gridSelection->count());

    Grid *g = MainWindow::GRIDS->value(name);
    if (!g){
        g = new Grid();
        MainWindow::GRIDS->insert(name,g);
    }
    g->values->insert("type","AWS");
    initForm(name);
    gridSelection->addItem(name);
}

void GridSetup::newNonEC2Form(){
    qDebug() << "new non-ec2 form";
    gc->close();

    QString name = QString("grid_engine%1").arg(gridSelection->count());

    Grid *g = MainWindow::GRIDS->value(name);
    if (!g){
        g = new Grid();
        MainWindow::GRIDS->insert(name,g);
    }
    initForm(name);
    gridSelection->addItem(name);
}

void GridSetup::saveAndClose(){
    //write everything out to the configuration file
    QHashIterator<QString,Grid *> grids(*MainWindow::GRIDS);

    while (grids.hasNext()){
        //try to write out the current settings to the file
        grids.next();
        qDebug() << grids.value()->values;
    }

    this->close();
}

void GridSetup::closeWindow(){
    this->close();
}

void GridSetup::addNewGrid(){
    gc = new GridChoice(0, this);
    gc->show();
}

void GridSetup::deleteExistingGrid(){

}

void GridSetup::initForm(QString selected){
    Grid *g = MainWindow::GRIDS->value(selected);
    if (!g->values->value("type").compare("AWS")){
        //AWS Config is shown
        EC2Grid *EC2Form = new EC2Grid();
        EC2Grids->insert(selected, EC2Form);

        wid->addWidget(EC2Form);
        wid->setCurrentWidget(EC2Form);

        EC2Form->populateValues(g->values, selected);
    }
    else{
        //normal config is shown
        NonEC2 *normalForm = new NonEC2();
        NonEC2Grids->insert(selected, normalForm);

        wid->addWidget(normalForm);
        wid->setCurrentWidget(normalForm);

        normalForm->populateValues(g->values, selected);
    }
}

void GridSetup::initSelectChoices(){
    QStringList grids = MainWindow::GRIDS->keys();
    if (grids.length()>0) {
        gridSelection->addItems(grids);
        if (gridSelection->currentIndex()!=-1){
            startMessage->hide();
        }
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
