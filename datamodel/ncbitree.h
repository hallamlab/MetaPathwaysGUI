#ifndef NCBITREE_H
#define NCBITREE_H

#include <QString>
#include <QHash>
#include <QRegExp>

#include "datamodel/datamanager.h"
#include "displaywidgets/progressview.h"



class NCBITree
{

public:
    static NCBITree *getNCBITree();



    QRegExp begin_pattern;
    QHash<QString, QString> name_to_id; // a readable taxon name to ncbi id
    QHash<QString, QString> id_to_name; //  a readable taxon ncbi taxid to name
    QHash<QString, QStringList > taxid_to_ptaxid; // this is the tree structure in a id to parent map, you can traverse it to go to the root
    /* this is the tree structure in a parent to child id map, you can use it to traverse the tree downwards
     ptaxid_to_taxid[ptaxid] = [ cid1, cid2, ...cidk]
     */

    QHash<QString, QHash<QString, bool> > ptaxid_to_taxid;
     QString getLineage(QString taxon);
private:
    NCBITree();
    static NCBITree * ncbitree;
    void loadTree(QString treefile);

    void loadTrees();
    QString translateNameToID(const QString &name) ;
    QString translateIdToName(const QString &id) ;


};

#endif // NCBITREE_H
