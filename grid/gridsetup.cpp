#include "gridsetup.h"
#include "ui_gridsetup.h"
#include "mainframe.h"
#include "Setup.h"
#include "gridchoice.h"
#include <QDebug>
#include <QMessageBox>

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


    gc = 0;

    MainFrame::PARAMS = Utilities::parseFile("template_param.txt");

    Grids = new QHash<QString, Grid*>();
    //keeps track of the names of our grids and our grids
    //an internal representation of a grid is held by a Grid object,
    //regardless if its a ec2 or nonec2 grid specification

    initGridValues();
    initSelectChoices();

    //create individual forms and populate their values
    QHashIterator<QString, Grid*> it(*Grids);
    while(it.hasNext()){
        it.next();
        initForm(it.key());
    }

    changeForm(Grids->keys().first());

    connect(gridSelection,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeForm(QString)));
    connect(addGrid, SIGNAL(clicked()), this, SLOT(addNewGrid()));
    connect(deleteGrid, SIGNAL(clicked()), this, SLOT(deleteExistingGrid()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
}

void GridSetup::addNewGrid(){
    if (gc){
        delete gc;
    }
    gc = new GridChoice(0, this);
    gc->show();
}

void GridSetup::newNonEC2Form(){
    qDebug() << "new non-ec2 form";
    gc->close();

    QString name = QString("grid_engine%1").arg(gridSelection->count());

    Grid *g = Grids->value(name);
    if (!g){
        g = new Grid();
        g->newGrid = true;
        g->deleted = false;
        Grids->insert(name,g);
    }
    initForm(name);
    gridSelection->addItem(name);
    gridSelection->setCurrentIndex(gridSelection->count()-1);
}

void GridSetup::newEC2Form(){
    qDebug() << "new ec2 form";
    gc->close();

    QString name = QString("grid_engine%1").arg(gridSelection->count());

    Grid *g = Grids->value(name);
    if (!g){
        g = new Grid();
        g->newGrid = true;
        g->deleted = false;
        Grids->insert(name,g);
    }
    g->values->insert("type","AWS");
    initForm(name);
    gridSelection->addItem(name);
}

void GridSetup::getWidgetValues(const Grid &g){
    //get all the widgets on a form
    QList<QWidget *>::iterator i;

    if (g.ec2) allWidgets = new QList<QWidget *>(g.ec2->findChildren<QWidget *>());
    else allWidgets = new QList<QWidget *>(g.nonec2->findChildren<QWidget *>());

    for (i = allWidgets->begin(); i != allWidgets->end(); ++i){
        QWidget *widget = *i;

        QString objectName = widget->objectName();
        QString value;

        if (qobject_cast<QComboBox *>(widget)){
            QComboBox *temp = qobject_cast<QComboBox *>(widget);
            value = temp->currentText();
            g.values->operator [](objectName) = value;
        }
        else if (qobject_cast<QSpinBox *>(widget)){
            QSpinBox *temp = qobject_cast<QSpinBox *>(widget);
            value = QString::number(temp->value());
            g.values->operator [](objectName) = value;

        }
        else if (qobject_cast<QLineEdit *>(widget)){
            QLineEdit *temp = qobject_cast<QLineEdit *>(widget);
            if (temp->objectName().compare("qt_spinbox_lineedit") < 0 ||
                    temp->objectName().compare("qt_spinbox_lineedit") > 0){
                value = temp->text();
                g.values->operator [](objectName) = value;
            }
        }
    }
}

void GridSetup::saveAndClose(){
    QHashIterator<QString,Grid *> grids(*Grids);

    while (grids.hasNext()){
        //get next grid in hash
        grids.next();

        //grids.key() gives us the name of the grid
        //grids.value()->values gives us the hash with the key being the setting, value

        getWidgetValues(*grids.value());
        //refresh the values stored for each grid, they may have changed

        QHashIterator<QString,QString> gridValues(*grids.value()->values);

        while (gridValues.hasNext()){
            gridValues.next();
            qDebug() << grids.key() << grids.value()->deleted;

            if (grids.value()->deleted){
                //if it is set to be scheduled, skip writing out the params
                //need to implement deletion from file using utilities
                qDebug() << "deleting grid" <<  grids.key() + ":" + gridValues.key() << gridValues.value();
                Utilities::writeSettingToFile("template_param.txt", grids.key() + ":" + gridValues.key(), gridValues.value(), false, true);
            }else if(grids.value()->newGrid){
                qDebug() << "new grid writing " <<  grids.key() + ":" + gridValues.key() << gridValues.value();
                Utilities::writeSettingToFile("template_param.txt", grids.key() + ":" + gridValues.key(), gridValues.value(), true,false);
            }else{
                qDebug() << "old grid writing" << grids.key() + ":" + gridValues.key() << gridValues.value();
                Utilities::writeSettingToFile("template_param.txt", grids.key() + ":" + gridValues.key(), gridValues.value(), false, false);
            }
        }
    }
    this->close();
}


void GridSetup::changeForm(QString selected){
    Grid *g = Grids->value(selected);
    if (!g->values->value("type").isEmpty()){
        wid->setCurrentWidget(g->ec2);
    }else wid->setCurrentWidget(g->nonec2);
}

void GridSetup::closeWindow(){
    this->close();
}

void GridSetup::deleteExistingGrid(){
    if (QMessageBox::Yes == QMessageBox::question(0,"Delete grid?","Do you really want to remove this grid?", QMessageBox::Yes|QMessageBox::No)){
        Grid *g = Grids->value(gridSelection->currentText());
        g->deleted = true;
        qDebug() << g->values << g->deleted;
        gridSelection->removeItem(gridSelection->currentIndex());

    }
}

void GridSetup::initForm(const QString &selected){
    Grid *g = Grids->value(selected);

    if (!g->values->value("type").isEmpty()){
        //AWS Config is shown
        EC2Grid *EC2Form = new EC2Grid();
        g->ec2 = EC2Form;
        wid->addWidget(g->ec2);
        EC2Form->populateValues(g, selected);
    }
    else{
        //normal config is shown
        NonEC2 *normalForm = new NonEC2();
        g->nonec2 = normalForm;
        wid->addWidget(g->nonec2);
        normalForm->populateValues(g, selected);
    }
}

void GridSetup::initSelectChoices(){
    QHashIterator<QString,Grid *> it(*Grids);

    if (it.hasNext()){
        startMessage->hide();
    }

    while (it.hasNext()){
        it.next();
        gridSelection->addItem(it.value()->name);
    }
}

/*
 * Parse the parameters file and find any settings we can for any given grid.
 * Initialize grids if they don't exist, adding in values into the values hash as we go.
 */
void GridSetup::initGridValues(){
    QHashIterator<QString,QString> it(*MainFrame::PARAMS);

    while(it.hasNext()){
        it.next();

        QRegExp gridRegExp("grid_engine\\d+:");
        if (it.key().contains(gridRegExp)){
            QRegExp splitReg(":");
            QStringList split = it.key().split(splitReg);

            //check if this grid exists in our hash, if not, add it
            QString gridName = split[0];
            QString param = split[1]; //setting name (eg type)

            Grid *g = Grids->value(gridName);
            if (!g){
                g = new Grid();
                Grids->operator [](gridName) = g;
                g->name = gridName;
                g->enabled = true;
                g->newGrid = false;
                g->deleted = false;
            }

            g->values->insert(param,it.value());
            Grids->insert(gridName,g); //update our hash of grids
        }
    }
}

GridSetup::~GridSetup()
{
    delete ui;
}
