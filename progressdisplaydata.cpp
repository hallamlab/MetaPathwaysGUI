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


bool ProgressDisplayData::isValidKey(const QString &stepName) {
    return _tablewidgetitem[REDCROSS].contains(stepName);
}

void ProgressDisplayData::createWidgets(QHash<QString,QString> &statusHash) {

    QHash<QString,QString>::iterator it;
    for(it=statusHash.begin();it!=statusHash.end();it++){
        QString stepName = it.key();

        QImage rimg  = QImage(":/images/cross.png");
        _tablewidgetitem[REDCROSS][stepName] = new QTableWidgetItem();
        _tablewidgetitem[REDCROSS][stepName]->setData(Qt::DecorationRole, QPixmap::fromImage(rimg).scaled(12,12));
        QImage cimg  = QImage(":/images/check.png");
        _tablewidgetitem[GREENCHECK][stepName] = new QTableWidgetItem();
        _tablewidgetitem[GREENCHECK][stepName]->setData(Qt::DecorationRole, QPixmap::fromImage(cimg).scaled(12,12));

        QImage loadmov  = QImage(":/images/logo.png");
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
