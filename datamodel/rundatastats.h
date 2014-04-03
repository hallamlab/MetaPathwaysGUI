#ifndef RUNDATASTATS_H
#define RUNDATASTATS_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include <QMap>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QStringList>
#include "rundata.h"
#include "samplercmanager.h"


class RunDataStats : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RunDataStats(QObject *parent = 0);
    void loadData();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setFileNames(const QStringList &filenames);
    void readStatFiles();
    void readDataFromFile(const QString &filename);
    
signals:
    
public slots:

private:
    QMap<QString, QList<QString> > dataVectorMap;
    QStringList filenames;
    int numcols;
    
};


#endif // RUNDATASTATS_H
