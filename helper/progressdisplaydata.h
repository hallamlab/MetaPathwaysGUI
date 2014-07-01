#ifndef PROGRESSDISPLAYDATA_H
#define PROGRESSDISPLAYDATA_H
#include <QMovie>
#include <QLabel>
#include <QTableWidgetItem>
#include <QString>
#include <QHash>
#include <QImage>

#include "types.h"


class ProgressDisplayData
{
public:
    static ProgressDisplayData *getProgressDisplayData();
    void createWidgets(const QStringList &stepNames);
    void destroyWidgets();
    QTableWidgetItem *getTableWidgetItem(const QString &key, const STATUS_SYMBOL);
    QMovie *getLoadingMovie(const QString &key);
    QLabel *getImageLabel(const QString &key) ;
    bool isValidKey(const QString &stepName);
private:
    ProgressDisplayData();
    static ProgressDisplayData *progressdata;

    QHash<QString, QTableWidgetItem *> _tablewidgetitem[5] ;  //= new QTableWidgetItem();
    QHash<QString, QMovie *>_loadingmovie; // = new QMovie(":/images/loading.gif");
    QHash<QString, QLabel *> _imageLabel; // = new QLabel();
};

#endif // PROGRESSDISPLAYDATA_H
