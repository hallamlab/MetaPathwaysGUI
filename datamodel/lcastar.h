#ifndef LCASTAR_H
#define LCASTAR_H

#include <QWidget>
#include <QString>
#include <QRegExp>
#include <QHash>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QStack>
#include <QMessageBox>
#include <QDebug>

#include "displaywidgets/progressview.h"
#include "datamodel/datamanager.h"

#include <math.h>

class LCAStar
{

public:
    static LCAStar *getLCAStar();
    void loadTree();
    QString lca_star(const QStringList &taxalist);
      void setParameters(unsigned int min_reads = 1, unsigned int min_depth = 1, double alpha = 0.53);
private:
    QStringList filter_taxa_list(const QStringList &taxalist);
    QString __lca_majority(const QStringList &taxalist);
    unsigned int __read_counts(QStringList taxalist, QHash<QString, unsigned int> &read_counts);
    void __annotate_tree_counts(const  QHash<QString , unsigned int> &read_counts);
    void __color_tree(const  QHash<QString , unsigned int> &read_counts);
    QString __create_majority(const QString &root,  const  QHash<QString , unsigned int> &read_name_counts );
    void __clear_lca_star_data_structure();
    QString  translateIdToName(const QString &id);
    void __decolor_tree();

    unsigned int taxon_depth(const QString &taxon);
    QString translateNameToID(const QString &taxon);


private:
    LCAStar();
    static LCAStar *lcastar;


    QRegExp begin_pattern;

    QHash<QString, QString> name_to_id; // a readable taxon name to ncbi id
    QHash<QString, QString> id_to_name; //  a readable taxon ncbi taxid to name
    QHash<QString, QStringList > taxid_to_ptaxid; // this is the tree structure in a id to parent map, you can traverse it to go to the root
    /* this is the tree structure in a parent to child id map, you can use it to traverse the tree downwards
     ptaxid_to_taxid[ptaxid] = [ cid1, cid2, ...cidk]
     */

    QHash<QString, QHash<QString, bool> > ptaxid_to_taxid;
    //a map from id to value, which has the S = sum n,  value for each id

    QHash<QString, QString> id_to_R;
    // a map from id to value, which has the S = sum n,  value for each id

    QHash<QString, double> id_to_S;
    // a map from id to value, which has the L = sum n log n,  value for each id

    QHash<QString, double> id_to_L;
    //a map from id to value, which has the entropy H value for each id

    QHash<QString, double> id_to_H;
    // a map to keep track of visited nodes

    QHash<QString, QString> id_to_V;

    unsigned int lca_star_min_reads;
    unsigned int  lca_star_min_depth;
    double lca_star_alpha;
    QHash<QString, QString> results_dictionary;


};

#endif // LCASTAR_H
