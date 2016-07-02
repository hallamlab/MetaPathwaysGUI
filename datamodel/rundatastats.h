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

enum { FULL_REPL, PARTIAL_REPL};

class RunDataStats : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RunDataStats(QObject *parent = 0);
    void loadData();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    int rowCount();
    int columnCount();
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int col, Qt::Orientation orientation, int role) const;
    void setFileNames(const QStringList &filenames);
    void setCategories(const QStringList &categories);
    void initializeTags();
    bool readStatFiles();
    bool readDataFromFile(const QString &filename, const QString &sample);
    void insertIntoPartial( QString field, unsigned int baserange) ;
    QString getPartialCode(QString field);
    void clear();
    
signals:
    
public slots:

private:
    QMap<QString, QList<QString> > dataVectorMap;
    QStringList filenames;
    QStringList categories;

    QHash<QString, QHash<QString, QString> > statsData, partialstatsData;
    QHash<QString, unsigned int > partialOrder;

    QHash<QString, QString> codeToTagLine;
  //  QHash<QString, bool> statNames;
    QHash<QString, QString> replace[2];
    QStringList orderCodes;
};


#endif // RUNDATASTATS_H
