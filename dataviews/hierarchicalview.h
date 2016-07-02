#ifndef HIERARCHICALVIEW_H
#define HIERARCHICALVIEW_H
#include <QTreeView>
#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QScrollArea>
#include <QWidget>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPair>
#include <QStack>

#include "tabs/resultwindow.h"

class ResultWindow;

class HierarchicalView : public QWidget
{
    Q_OBJECT
public:
    static HierarchicalView *getHierarchicalView();
    void createModel(QHash<QString, QStringList> &samples);
    void setFileList(QStringList &filenames);
    void showTreeView(QHash<QString, QStringList> &samples) ;
    void clearObjects();
    void setReceiver(ResultWindow *resultWindow  );

private:
    int addCounts(QStandardItem *head);

    void selectTree(QStandardItem *headitem, QList<QPair<QString, QString> > &selectSamples, bool checked, QString category="") ;

    void saveHistory();
    void restoreHistory();
    void insertIntoHistory(QString text);


    explicit HierarchicalView(QWidget *parent = 0);
    static HierarchicalView *hierarchicalview;
    static QHash<QString, QString> *history;

    QStringList filenames;
    QStandardItemModel * model;
    QStandardItem *rootitem;
    QTreeView *treeView ;
    QList<QStandardItem *> _createdItemList;
    ResultWindow *resultWindow;

    QStandardItem * getQStandardItem(const QString &name);
    QScrollArea *scroll;
signals:

public slots:
    void collapseTree();
    void expandTree();
    void defaultExpandTree();
    void selectAll();
    void unselectAll();
    void cancelChoice();
    void OKChoice();
};

#endif // HIERARCHICALVIEW_H
