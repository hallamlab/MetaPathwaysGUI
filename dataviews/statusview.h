#ifndef STATUSVIEW_H
#define STATUSVIEW_H
#include <QTreeView>
#include <QStringList>
#include <QTreeView>
#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTextStream>
#include <QFileInfo>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QList>

#include "datamodel/samplercmanager.h"
#include "rundata.h"

class StatusView : public QObject
{
    Q_OBJECT
public:
    static StatusView *getStatusView();
    void createModel();
    void setFileList(QStringList &filenames);
    void showTreeView() ;
    void clearObjects();

private:
    explicit StatusView(QObject *parent = 0);
    static StatusView *statusview;
    QStringList filenames;
    QStandardItemModel * model;
    QStandardItem *rootitem;
    QTreeView *treeView ;
    QList<QStandardItem *> _createdItemList;
    void readDataFromFile(const QString &absFileName, const QString &filename);
    QStandardItem * getQStandardItem(const QString &name);
signals:
    
public slots:
    
};

#endif // STATUSVIEW_H
