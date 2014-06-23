#ifndef RUNDATASTATS_H
#define RUNDATASTATS_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include <QMap>
#include <QFile>
#include <QHash>
#include <QFileInfo>
#include <QTextStream>
#include <QStringList>
#include <QPair>

#include "helper/rundata.h"
#include "datamodel/samplercmanager.h"


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
    bool readStatFiles();
    bool readDataFromFile(const QString &filename, const QString &sample);
    void clear();
    
signals:
    
public slots:

private:
    QMap<QString, QList<QString> > dataVectorMap;
    QStringList filenames;

    QHash<QString, QHash<QString, QString> > statsData;
    QHash<QString, unsigned int> ranking;
    QHash<QString, bool> statNames;
};


#endif // RUNDATASTATS_H
