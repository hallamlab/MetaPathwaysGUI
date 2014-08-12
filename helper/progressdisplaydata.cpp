#include "progressdisplaydata.h"
#include <QDebug>
ProgressDisplayData *ProgressDisplayData::progressdata = 0;
ProgressDisplayData::ProgressDisplayData()
{

}

void ProgressDisplayData::destroyWidgets() {
    QList<STATUS_SYMBOL> statuses;
    statuses << REDCROSS << GREENCHECK << LOADING;

    foreach( STATUS_SYMBOL status, statuses) {
        QStringList keys = _tablewidgetitem[status].keys();

        foreach( QString key, keys) {
            delete _tablewidgetitem[status][key];
            _tablewidgetitem[status].remove(key);
        }
    }
}



bool ProgressDisplayData::widgetsCreated() {

  if(this->_tablewidgetitem[REDCROSS].size() > 1  ) return true;
  return false;

}


void ProgressDisplayData::reCreateWidgets(const QString &stepName, const STATUS_SYMBOL status) {
   QImage img;

   switch(status) {
         case REDCROSS:
                img  = QImage(":/images/cross.png");
                break;
         case GREENCHECK:
                img  = QImage(":/images/check.png");
                break;
         case PARTIAL:
                img  = QImage(":/images/partial.png");
               break;
         case UNSURE:
                img  = QImage(":/images/unsure.png");
                break;
         case LOADING:
                img = QImage(":/images/running.png");
                break;
         default:
                break;
     }

     if( _tablewidgetitem[status][stepName] ==0) delete _tablewidgetitem[status][stepName];
     _tablewidgetitem[status][stepName] =0;


     _tablewidgetitem[status][stepName] = new QTableWidgetItem();
     _tablewidgetitem[status][stepName]->setData(Qt::DecorationRole, QPixmap::fromImage(img).scaled(12,12));

}

bool ProgressDisplayData::isValidKey(const QString &stepName) {
    return _tablewidgetitem[REDCROSS].contains(stepName);
}

void ProgressDisplayData::createWidgets(const QStringList &stepNames) {


    foreach(QString stepName, stepNames) {
        QImage rimg  = QImage(":/images/cross.png");
        _tablewidgetitem[REDCROSS][stepName] = new QTableWidgetItem();
        _tablewidgetitem[REDCROSS][stepName]->setData(Qt::DecorationRole, QPixmap::fromImage(rimg).scaled(12,12));

        QImage cimg  = QImage(":/images/check.png");
        _tablewidgetitem[GREENCHECK][stepName] = new QTableWidgetItem();
        _tablewidgetitem[GREENCHECK][stepName]->setData(Qt::DecorationRole, QPixmap::fromImage(cimg).scaled(12,12));

        QImage dimg  = QImage(":/images/partial.png");
        _tablewidgetitem[PARTIAL][stepName] = new QTableWidgetItem();
        _tablewidgetitem[PARTIAL][stepName]->setData(Qt::DecorationRole, QPixmap::fromImage(dimg).scaled(8,26));

        QImage eimg  = QImage(":/images/unsure.png");
        _tablewidgetitem[UNSURE][stepName] = new QTableWidgetItem();
        _tablewidgetitem[UNSURE][stepName]->setData(Qt::DecorationRole, QPixmap::fromImage(eimg).scaled(8,20));

        QImage loadmov  = QImage(":/images/running.png");
        _tablewidgetitem[LOADING][stepName] = new QTableWidgetItem();
        _tablewidgetitem[LOADING][stepName]->setData(Qt::DecorationRole, QPixmap::fromImage(loadmov).scaled(12,12));

/*
        _loadingmovie[stepName] = new QMovie(":/images/loading.gif");
        _imageLabel[stepName] = new QLabel();
        _loadingmovie[stepName]->setParent(_imageLabel[stepName]);

        _imageLabel[stepName]->setFixedSize(30,25);
        _imageLabel[stepName]->setMovie(_loadingmovie[stepName] );
        _imageLabel[stepName]->updateGeometry(); */
    }


}

ProgressDisplayData* ProgressDisplayData::getProgressDisplayData(){
    if (ProgressDisplayData::progressdata ==0){
        ProgressDisplayData::progressdata = new ProgressDisplayData();
    }
    return ProgressDisplayData::progressdata;
}


QTableWidgetItem *ProgressDisplayData::getTableWidgetItem(const QString &key, const STATUS_SYMBOL status) {
    if( _tablewidgetitem[status].contains(key) )
        return _tablewidgetitem[status][key];
    else
        return 0;
}

QMovie *ProgressDisplayData::getLoadingMovie(const QString &key) {
    if( _loadingmovie.contains(key))
        return _loadingmovie[key];
    else
        return 0;
}

QLabel *ProgressDisplayData::getImageLabel(const QString &key) {
    if( _imageLabel.contains(key) )
        return _imageLabel[key];
    else
       return 0;
}
