/*
 * This class represents the settings screen where the user sets up values for the run, including
 * the specification of dbs.
 */
#include "SettingsTab.h"
#include "ui_SettingsTab.h"
#include "mainframe.h"

QList<QWidget *> *SettingsTab::allWidgets = NULL;

SettingsTab::SettingsTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsTab)
{
    ui->setupUi(this);


    SettingsTab::allWidgets = new QList<QWidget *>();

    this->getAllWidgets();
  //  annotationDBS = this->findChild<QListWidget *>("annotationDBS");
  //  rrnaREFDBS = this->findChild<QListWidget *>("rrnaREFDBS");

    //showORFDBS();
    //showRRNADBS();

    //RunData *run = RunData::getRunData();
    //annotationFiles = new QStringList((run->getValueFromHash("annotation:dbs",_PARAMS)).split(",", QString::SkipEmptyParts));
    //rrnaFiles = new QStringList((run->getValueFromHash("rRNA:refdbs",_PARAMS)).split(",", QString::SkipEmptyParts));

    annotationFiles = new QStringList;
    rrnaFiles = new QStringList;

    connect(annotationDBS,SIGNAL(clicked(QModelIndex)), this, SLOT(annotationClicked(QModelIndex)));
    connect(rrnaREFDBS, SIGNAL(clicked(QModelIndex)), this, SLOT(rrnaClicked(QModelIndex)));

    this->initWidgetValues();
    setStyling();

    restoreDefaults = this->findChild<QPushButton *>("restoreDefaultsButton");
    connect(restoreDefaults, SIGNAL(clicked()), this, SLOT(restoreDefaultValues()));

    // setup ...
    Q_FOREACH( QSpinBox * sp, findChildren<QSpinBox*>() ) {
            sp->installEventFilter( this );
            sp->setFocusPolicy( Qt::StrongFocus );
    }

    Q_FOREACH( QDoubleSpinBox * sp, findChildren<QDoubleSpinBox*>() ) {
            sp->installEventFilter( this );
            sp->setFocusPolicy( Qt::StrongFocus );
    }


}

bool SettingsTab::eventFilter( QObject * o, QEvent * e ) {
   if ( e->type() == QEvent::Wheel &&
        qobject_cast<QAbstractSpinBox*>( o ) )
   {
       e->ignore();
       return true;
   }
   return QWidget::eventFilter( o, e );
}

void SettingsTab::restoreDefaultValues() {
     this->initWidgetValues(true);
}

void SettingsTab::loadParameters() {
    //SettingsTab::allWidgets = new QList<QWidget *>();

    //getAllWidgets();

    this->showORFDBS();
    this->showRRNADBS();


    RunData *run = RunData::getRunData();
//   qDebug() << run->getPARAMS();
   // *annotationFiles = QStringList((run->getValueFromHash("annotation:dbs",_PARAMS)).split(",", QString::SkipEmptyParts));
  //  *rrnaFiles = QStringList((run->getValueFromHash("rRNA:refdbs",_PARAMS)).split(",", QString::SkipEmptyParts));


  //  initWidgetValues();

}

void SettingsTab::annotationClicked(QModelIndex index){
    bool oneChecked = false;
    for (int i=0;i<annotationDBS->count();i++){
        if(annotationDBS->item(i)->checkState()==Qt::Checked){
            oneChecked = true;
          //  qDebug() <<  annotationDBS->item(i)->text() << " : " << "Checked";
        }
        else {
           //  qDebug() <<  annotationDBS->item(i)->text() << " : " << "UnChecked";
        }
    }

}

void SettingsTab::rrnaClicked(QModelIndex index){
    bool oneChecked = false;
    for (int i=0; i<rrnaREFDBS->count(); i++) {
        if(rrnaREFDBS->item(i)->checkState()==Qt::Checked){
            oneChecked = true;
        }
    }
}

bool SettingsTab::findFiles(QString path, QStringList &fileListing){
    QDir dir(path);

    if (!dir.exists()) return false;

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

    QFileInfoList files = dir.entryInfoList();

    foreach(QFileInfo file, files){
        fileListing.append(file.fileName());
    }
    return true;
}

void SettingsTab::showRRNADBS(){

    RunData *run = RunData::getRunData();
    QString dbPath =  run->getConfig().operator []("REFDBS") + QDir::separator() + "taxonomic";
    QStringList filesInDir;

    QRegExp reg("^[.]");
    rrnaREFDBS->clear();
    if (this->findFiles(dbPath, filesInDir)){
        if (filesInDir.length()>0){
            foreach (QString db, filesInDir){
                if( db.indexOf(reg, 0) >  -1) continue;
                QListWidgetItem *item = new QListWidgetItem(db);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable );
                item->setCheckState(Qt::Unchecked);
                rrnaREFDBS->addItem(item);
            }
        }else{
//            rrnaREFDBSWarning->setText("No taxonomic databases found under your specified database path!");
       }
    }
    else{
        QMessageBox msg;
        //msg.warning(0,"Invalid Path!", "Could not find " + dbPath + ", does this folder actually exist? \nPlease check your setup to make sure your databases path is set up correctly.", QMessageBox::Ok);
    }

}

void SettingsTab::showORFDBS(){

    RunData *run = RunData::getRunData();
    QString dbPath =  run->getConfig().operator []("REFDBS") + QDir::separator() + QString("functional");
    QStringList filesInDir;

    QRegExp reg("^[.]"); // removes stray file created by the OS

    annotationDBS->clear();
    if (this->findFiles(dbPath, filesInDir)){
        if (filesInDir.length() >0 ){
            foreach (QString db, filesInDir){
                if( db.indexOf(reg, 0) >  -1) continue;
                QListWidgetItem *item = new QListWidgetItem(db);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable );
                item->setCheckState(Qt::Unchecked);
                annotationDBS->addItem(item);
            }
        }
    }
    else{
        QMessageBox msg;
      //  msg.warning(0,"Invalid Path!", "Could not find " + dbPath + ", does this folder actually exist? \nPlease check your setup to make sure your databases path is set up correctly.", QMessageBox::Ok);
    }
}

void SettingsTab::setStyling(){
//    rrnaREFDBSWarning->setStyleSheet("color:red");
//    annotationDBSWarning->setStyleSheet("color:red");
}


void SettingsTab::closeWindow(){
    close();
}


/**
 * @brief SettingsTab::initWidgetValues, Similiar to the code in MainFrame,
 * where we save values, we are instead initializing them here
 * for each field widget.
 */
void SettingsTab::initWidgetValues(bool defaultValue){
    QList<QWidget *>::iterator i;
    SETTING_TYPE settingType;

    settingType = defaultValue ? _PARAMS_DEFAULT : _PARAMS;

    RunData *rundata = RunData::getRunData();

    for (i = SettingsTab::allWidgets->begin();i != SettingsTab::allWidgets->end(); ++i){
        QWidget *widget = *i;
        QString objectName = widget->objectName();

        QString configName = rundata->getConfigMapping().key(objectName);

        QString value = rundata->getValueFromHash(configName, settingType);
        if (qobject_cast<QComboBox *>(widget)!=NULL){
            QComboBox *temp = qobject_cast<QComboBox *>(widget);
            temp->setCurrentIndex(temp->findText(value));
            if (temp->currentIndex()==-1){
                temp->addItem(value);
            }
        }
        else if (qobject_cast<QDoubleSpinBox *>(widget)!=NULL){
            QDoubleSpinBox *temp = qobject_cast<QDoubleSpinBox *>(widget);
            temp->setValue(value.toDouble());
        }
        else if (qobject_cast<QSpinBox *>(widget)!=NULL){
            QSpinBox *temp = qobject_cast<QSpinBox *>(widget);
            temp->setValue(value.toInt());
        }
        else if (qobject_cast<QTextEdit *>(widget)!=NULL){
            QTextEdit *temp = qobject_cast<QTextEdit *>(widget);
            temp->setText(value);
        }
    }

    //update the annotation dbs
    this->checkAnnotationDBS();
}


/**
 * @brief SettingsTab::checkAnnotationDBS, puts a checks in the settings panel
 * for the database names for each db in the template_params.txt
 */
void SettingsTab::checkAnnotationDBS() {
    QHash<QString, bool> DBNames;

    RunData *rundata = RunData::getRunData();

    // deal with the refdbs
    foreach(QString dbname, Utilities::getSplitNames(rundata->getParams() ["rRNA:refdbs"]) ) {
         DBNames[dbname] = true;
    }
    for(unsigned int i =0; i < this->rrnaREFDBS->count(); i++) {

        if( DBNames.contains( this->rrnaREFDBS->item(i)->text() )) {
           this->rrnaREFDBS->item(i)->setCheckState(Qt::Checked);
        }
        else{
            this->rrnaREFDBS->item(i)->setCheckState(Qt::Unchecked);
        }
    }

    // deal with the refdbs
    DBNames.clear();
    foreach(QString dbname, Utilities::getSplitNames(rundata->getParams() ["annotation:dbs"]) ) {
         DBNames[dbname] = true;
    }

    for(unsigned int i =0; i < this->annotationDBS->count(); i++) {
        if( DBNames.contains( this->annotationDBS->item(i)->text() )) {
           this->annotationDBS->item(i)->setCheckState(Qt::Checked);
        }
        else{
            this->annotationDBS->item(i)->setCheckState(Qt::Unchecked);
        }
    }
}

/**
 * @brief SettingsTab::getADBNames, gets the annotation db names in the
 *  folder in randata
 * @return
 */
QStringList SettingsTab::getADBNames() {

        QStringList names;
        /*
    RunData *run = RunData::getRunData();
    QString dbPath =  run->getConfig().operator []("REFDBS") + "/functional";
    QStringList filesInDir;
    QStringList names;
    QRegExp reg("^[.]");

    if (this->findFiles(dbPath, filesInDir)){
        if (filesInDir.length() >0 ){
            foreach (QString db, filesInDir){
                if( db.indexOf(reg, 0) != -1) continue;
                names.append(db);
            }
        }
    }*/
    return names;
}




/*
 * AllWidgets is used by the mainframe to update values across activities. We simply cast
 * all the widgets of this class to QWidget, and save it in allWidgets.
 */
void SettingsTab::getAllWidgets(){
    qcWidgets = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^quality_control*")));
    orfWidgets = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^orf_prediction*")));
    annotationWidgets = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^annotation*")));
    rrnaWidgets = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^rrna*")));
    pathwaysWidgets = new QList<QWidget *>(this->findChildren<QWidget *>(QRegExp("^ptools_settings*")));


    annotationDBS = this->findChild<QListWidget *>("annotationDBS");
    rrnaREFDBS = this->findChild<QListWidget *>("rrnaREFDBS");

    QList<QWidget *>::iterator i;


    for (i = qcWidgets->begin();i != qcWidgets->end(); ++i){
        QWidget *widget = *i;
        SettingsTab::allWidgets->append(widget);
    }
    for (i = orfWidgets->begin();i != orfWidgets->end(); ++i){
        QWidget *widget = *i;
        SettingsTab::allWidgets->append(widget);
    }
    for (i = annotationWidgets->begin();i != annotationWidgets->end(); ++i){
        QWidget *widget = *i;
        SettingsTab::allWidgets->append(widget);
    }
    for (i = rrnaWidgets->begin();i != rrnaWidgets->end(); ++i){
        QWidget *widget = *i;
        SettingsTab::allWidgets->append(widget);
    }
    for (i = pathwaysWidgets->begin(); i != pathwaysWidgets->end(); ++i){
        QWidget *widget = *i;
        SettingsTab::allWidgets->append(widget);
    }
}

SettingsTab::~SettingsTab()
{
    delete ui;

    delete annotationFiles;
    delete rrnaFiles;
    delete qcWidgets;
    delete orfWidgets;
    delete annotationWidgets;
    delete rrnaWidgets;
    delete pathwaysWidgets;
}
