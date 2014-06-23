#include "gridsetup.h"
#include "ui_gridsetup.h"
#include "mainframe.h"
#include "tabs/Setup.h"
#include "gridchoice.h"
#include <QDebug>
#include <QMessageBox>

/**
 * @brief GridSetup::GridSetup, this sets up the grid configuration panel for adding and
 * deleting grid or to change their settings
 * @param parent
 */
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
    this->setDefaultParamValues();
    RunData *rundata = RunData::getRunData();
    rundata->setupDefaultParams();

    allWidgets = 0;
    Grids = 0;
    gc = 0;

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

    if (Grids->keys().count()>0) changeForm(Grids->keys().first());

    connect(gridSelection,SIGNAL(currentIndexChanged(QString)),this,SLOT(changeForm(QString)));
    connect(addGrid, SIGNAL(clicked()), this, SLOT(addNewGrid()));
    connect(deleteGrid, SIGNAL(clicked()), this, SLOT(deleteExistingGrid()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));


}


/**
 * @brief GridSetup::setDefaultParamValues
 * initialize the default values for some of the grid services parameters
 */
void GridSetup::setDefaultParamValues() {
    this->defaultParamValues.insert("bit", "bit64");
    this->defaultParamValues.insert("os","linux");
    this->defaultParamValues.insert("max_parallel_jobs","20");
    this->defaultParamValues.insert("batch_size","100");

    this->allowedParams["user"] = true;
    this->allowedParams["server"] = true;
    this->allowedParams["bit"] = true;
    this->allowedParams["sshKey"] = true;
    this->allowedParams["os"] = true;
    this->allowedParams["optional_string"] = true;

}

/**
 * @brief GridSetup::addNewGrid Add a regular grid (non-EC type)
 */
void GridSetup::addNewGrid(){
    //remove ability to specify amazon ec2 grids
    newNonEC2Form();
}

/**
 * @brief GridSetup::newNonEC2Form Add a non-ec type grid
 */
void GridSetup::newNonEC2Form(){
    unsigned int i =0;
    while( Grids->contains(QString("grid_engine%1").arg(i)) ) i++;
    QString name = QString("grid_engine%1").arg(i);

    Grid *g = Grids->value(name);
    if (g==0){
        g = new Grid();
        g->newGrid = true;
        g->deleted = false;
        Grids->insert(name,g);
    }
    initForm(name);
    gridSelection->addItem(name);
    gridSelection->setCurrentIndex(gridSelection->count()-1);
}


/**
 * @brief GridSetup::newEC2Form not in use anymore
 */
void GridSetup::newEC2Form(){

    unsigned int i =0;
    while( Grids->contains(QString("grid_engine%1").arg(i)) ) i++;


    QString name = QString("grid_engine%1").arg(i);

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


/**
 * @brief GridSetup::getWidgetValues read the parameters of a grid as provided by the user
 * from the GUI
 * @param g, the grid
 */
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

/**
 * @brief GridSetup::saveAndClose, save the grid setting if they are validated
 */
void GridSetup::saveAndClose(){

    RunData *rundata = RunData::getRunData();
    QString errMsg ;
    if( ! this->validateGridSettings(errMsg) ) {
        QMessageBox::warning(this, "Errors in Grid Setup", QString("Please either remove the erroneously set grids, or correct their settings ") + errMsg, QMessageBox::Ok )  ;
        return;
    }
    this->writeGridSettingToFile(rundata->getConfig()["METAPATHWAYS_PATH"] +  QDir::separator() + "config" + QDir::separator() + RunData::TEMPLATE_PARAM);


    rundata->setupDefaultParams();
    this->close();
    // since we've added a new grid, need to update the hash
}


/**
 * @brief GridSetup::validateGridSettings, validate the Grid settings
 */
bool GridSetup::validateGridSettings(QString &errMsg) {

       errMsg = "Error : \n";
       bool error = true;
       QHashIterator<QString,  Grid *> grids(*Grids);
       while (grids.hasNext()){


           //get next grid in hash
           grids.next();

           //grids.key() gives us the name of the grid
           //grids.value()->values gives us the hash with the key being the setting, value

           getWidgetValues(*grids.value());
           //refresh the values stored for each grid, they may have changed
           QHashIterator<QString,QString> gridValues(*grids.value()->values);

           while( gridValues.hasNext() ) {
               gridValues.next();

               if( gridValues.value().trimmed().size() ==0 && gridValues.key().size()!=0 && ( gridValues.key().compare("user")==0 || gridValues.key().compare("server")==0  ) ) {
                   errMsg += "Grid : " + grids.key() + " missing " +  gridValues.key() + "\n";
                   error = false;
               }
           }

       }

    return error;
}



/**
 * @brief GridSetup::writeGridSettingToFile
 *
 * Creates a new file with filename TEMPLATE_FILE. Copies over old lines and the new specified KEY, VALUE
 * pairing for settings. TYPE specifies if it is a config or param file, as the first will require all values
 * to be wrapped in single quotations, and params do not.
 *
 * @param TEMPLATE_FILE
 */
void GridSetup::writeGridSettingToFile(const QString &TEMPLATE_FILE){
    QFile inputFile(TEMPLATE_FILE);
    QFile newFile( TEMPLATE_FILE + "_new" );
    bool prevLineNonEmpty = true;
    QRegExp gridLine("^grid_");


    if (inputFile.open(QIODevice::ReadOnly) && newFile.open(QIODevice::ReadWrite))
    {
       QTextStream in(&inputFile);
       QTextStream out(&newFile);

       while ( !in.atEnd() )
       {
            QString line = in.readLine();

            if (gridLine.indexIn(line)==-1 ){
                //if the line doesn't begin with a comment hash
                if( !line.trimmed().isEmpty()  ||  prevLineNonEmpty == false) {
                    out << line << "\n";
                }
                if(line.trimmed().isEmpty()) prevLineNonEmpty = true;
                else prevLineNonEmpty = false;
            }
       }

       QHashIterator<QString,  Grid *> grids(*Grids);

       out << "\n";
       while (grids.hasNext()){
           out << "\n";
           //get next grid in hash
           grids.next();

           //grids.key() gives us the name of the grid
           //grids.value()->values gives us the hash with the key being the setting, value

           getWidgetValues(*grids.value());
           //refresh the values stored for each grid, they may have changed
           QHashIterator<QString,QString> gridValues(*grids.value()->values);
           while( gridValues.hasNext() ) {
               gridValues.next();
               out <<  grids.key() + ":" + gridValues.key() + " " +  gridValues.value() << "\n";
           }
       }


       inputFile.close();
       inputFile.remove(TEMPLATE_FILE);
       //delete old file

       newFile.rename(TEMPLATE_FILE + "_new", TEMPLATE_FILE);
       newFile.close();
       //set new file with the proper name and close

    }else{
        QMessageBox msg;
        msg.warning(0,"Error!\n",QString("Could not write the configuration file to " + TEMPLATE_FILE + ", do you have permissions to write there?"), QMessageBox::Ok);
    }

}

void GridSetup::changeForm(QString selected){
    Grid *g = Grids->value(selected);

    foreach(QString param, g->values->keys()) {
      QString value = g->values->value(param);

      if( value.isEmpty()) {
          if( this->defaultParamValues.contains(param) )
             value = this->defaultParamValues[param];
          if( param=="name") value = g->name;
          g->values->insert(param, value);
      }
    }

    if (!g) return;
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
        delete g;
        Grids->remove(gridSelection->currentText());
        gridSelection->removeItem(gridSelection->currentIndex());
    }
}

void GridSetup::initForm(const QString &selected){
    Grid *g = Grids->value(selected);

//    if (!g->values->value("type").isEmpty()){
//        //AWS Config is shown
//        EC2Grid *EC2Form = new EC2Grid();
//        g->ec2 = EC2Form;
//        wid->addWidget(g->ec2);
//        EC2Form->populateValues(g, selected);
//    }
//    else{
    //normal config is shown
    NonEC2 *normalForm = new NonEC2();
    g->nonec2 = normalForm;
    wid->addWidget(g->nonec2);
    normalForm->populateValues(g, selected);
//    }
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


/**
 * Parse the parameters file and find any settings we can for any given grid.
 * Initialize grids if they don't exist, adding in values into the values hash as we go.
 * @brief GridSetup::initGridValues
 */
void GridSetup::initGridValues(){
    RunData *rundata = RunData::getRunData();
    QHashIterator<QString,QString> it(rundata->getParams());



   // qDebug() << rundata->getParams();
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
            if (g==0){
                g = new Grid();
                Grids->operator [](gridName) = g;
                g->name = gridName;
                g->enabled = false;
                g->newGrid = false;
                g->deleted = false;
            }


            if( !this->allowedParams.contains(param)) continue;

            QString value;
            if( it.value().isEmpty() ) {
                if( this->defaultParamValues.contains(param) )
                    value = this->defaultParamValues[param];
                if( param=="name") value = g->name;
            } else {
                value = it.value();
            }

            g->values->insert(param, value);
            Grids->insert(gridName,g); //update our hash of grids
        }
    }
}

/**
 * @brief GridSetup::~GridSetup destroy the grid, widgets, and grid configurations
 */
GridSetup::~GridSetup()
{
    delete Grids;
    delete allWidgets;
    delete ui;
    delete gc;

}
