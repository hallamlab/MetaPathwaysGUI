#include "lcastar.h"

LCAStar *LCAStar::lcastar =0;

LCAStar *LCAStar::getLCAStar() {
    if( LCAStar::lcastar ==0 ) {
        LCAStar::lcastar = new LCAStar();
        LCAStar::lcastar->loadTree();
    }

    return LCAStar::lcastar;
}


void LCAStar::loadTree() {

    DataManager *datamanager = DataManager::getDataManager();
    QString ncbifile = datamanager->getResourceFile(NCBITREEFILE);

    qDebug() << ncbifile;

    ProgressView progressbar("Please wait while the NCBI tree file " +  ncbifile, 0, 0, 0);

    QString ncbiTree = ncbifile;
    QFile inputFile(ncbiTree);
    QChar delim = QChar('\t');
    QStringList fields;

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();
            if (this->begin_pattern.indexIn(line) != -1  ) continue;

            fields = line.split(QRegExp(delim));
            if(fields.size()  !=3 ) continue;

            this->name_to_id[fields[0]] = fields[1];
            this->id_to_name[fields[1]] = fields[0];

            this->taxid_to_ptaxid[fields[1]] << fields[2] << 0 << 0;
            if( !this->ptaxid_to_taxid.contains(fields[2]))
                this->ptaxid_to_taxid[fields[2]] = QHash<QString, bool>();

            if( !(  fields[2].compare(QString("1")) ==0  || fields[1].compare(QString("1")) ))
                this->ptaxid_to_taxid[fields[2]][fields[1]] = false;

        }
    }
    else return;

    /*
    qDebug() << " name to id " << this->name_to_id.size();
    qDebug() << " id to name " << this->id_to_name.size();
    qDebug() << " tax id to pid " << this->taxid_to_ptaxid.size();
    qDebug() << " pid to tax " << this->ptaxid_to_taxid.size();
*/

    inputFile.close();
    progressbar.hide();

}

QString LCAStar::lca_star(const QStringList &taxalist) {
    QStringList _taxalist = this->filter_taxa_list(taxalist);

    if( _taxalist.isEmpty() ) return QString("root");


    QString majority = this->__lca_majority(_taxalist);

    if( !majority.isEmpty() ) return majority;

    QHash<QString , unsigned int> read_counts;

    this->__read_counts(_taxalist, read_counts);


    this->__annotate_tree_counts( read_counts );

    this->__color_tree(read_counts);

    QString result_id = __create_majority("1", read_counts);

    this->__clear_lca_star_data_structure();

    QString results_taxon = this->translateIdToName(result_id);

    this->__decolor_tree();

    return results_taxon;
}


/**
 * @brief LCAStar::filter_taxa_list, removes the taxa from taxalist that
 * are at shallower levels of the NCBI tree
 * @param taxalist, list of taxa
 * @return filtered list
 */
QStringList LCAStar::filter_taxa_list(const QStringList &taxalist) {
     QStringList newlist;
     // filter based on depth
     foreach(QString taxon, taxalist) {
         unsigned int depth = this->taxon_depth(taxon);
         if( depth < this->lca_star_min_depth) continue;
         newlist.append(taxon);
     }

     // filter based on min_reads /decide if we should return 'root'
     // compute the taxon using lca_star

     if(newlist.size() < this->lca_star_min_reads)
         return QStringList();

     return newlist;
}


/**
 * @brief LCAStar::__lca_majority, computes the majority if it exists already
 * @param taxalist, list of taxa
 * @return
 */

QString LCAStar::__lca_majority(const QStringList &taxalist) {
    // create the read counts
    QHash<QString, unsigned int> read_counts;

    unsigned int  Total =  this->__read_counts(taxalist, read_counts);

    // find the taxon with the highest count but also has count higher than the
     // majority threshold
    QString majority = QString();
    unsigned int maxcount =0;

    foreach(QString taxon, read_counts.keys()) {
      if(maxcount < read_counts[taxon] && Total*this->lca_star_alpha < read_counts[taxon]) {
         maxcount = read_counts[taxon];
         majority = taxon;
       }
    }

   // majority exists
    if( !majority.isEmpty() )
      return majority;

   return QString();

}

/**
 * @brief LCAStar::__read_counts, returns the frequency count of each taxa in a
 * hash
 * @param taxalist, taxalist
 * @param read_counts, the hash with the frequency count
 * @return total count
 */
unsigned int LCAStar::__read_counts(QStringList taxalist, QHash<QString, unsigned int> &read_counts) {
    read_counts.clear();
    unsigned int Total = 0;
    foreach(QString taxon, taxalist) {
        if( !read_counts.contains(taxon))
            read_counts[taxon] =0;
        read_counts[taxon]++;
        Total++;
    }
    return Total;
}


/**
 * @brief LCAStar::__annotate_tree_counts
 * @param read_counts
 */
void LCAStar::__annotate_tree_counts(const QHash<QString, unsigned int> &read_counts) {
    foreach(QString taxon, read_counts.keys() ) {
        QString id = this->translateNameToID(taxon);
        if( id.isEmpty() ) continue;
        this->id_to_R[id] = read_counts[taxon];
    }
}


/**
 * @brief LCAStar::__color_tree, colors the tree count by climbing to the root
 * starting from the taxon, one by one
 * @param read_counts
 */
void LCAStar::__color_tree(const QHash<QString, unsigned int> &read_counts) {
    foreach(QString taxon, read_counts.keys() ) {
        QString id = this->translateNameToID(taxon);

        QString tid = id;
        while( this->taxid_to_ptaxid.contains(tid) && tid.compare(QString("1")) !=0) {
            QString pid = this->taxid_to_ptaxid[tid][0];
            this->ptaxid_to_taxid[pid][tid] = true;
            tid = pid;
        }
    }

}


typedef struct _CANDIDATE {
    QString first;
    double second;
} CANDIDATE;
/**
 * @brief LCAStar::__create_majority, crates a majority by climbing up the NCBI tree until a majority
 * is found with least entropy distortion
 * @param root
 * @param read_name_counts
 * @return the majority of the new collapsed taxons
 */
QString LCAStar::__create_majority(const QString &root, const QHash<QString, unsigned int> &read_name_counts) {

    QHash<QString, unsigned int> read_counts;
    unsigned int Total=0;

    foreach(QString taxon, read_name_counts.keys() ) {
        QString id = this->translateNameToID(taxon);
        read_counts[id] = read_name_counts[taxon];
        Total += read_name_counts[taxon];
    }

    CANDIDATE candidate;
    candidate.first = QString("1");
    candidate.second = 100000000.0;
    QStack<QString> Stack;
    Stack.push(root);
/*
    foreach(QString key, read_name_counts.keys())
        qDebug() << " key -value " << key <<  read_name_counts[key];

    qDebug() << " id to H " << this->id_to_H.size();
    qDebug() << " id to V " << this->id_to_V.size();
    qDebug() << " ptaxid to taxid " << this->ptaxid_to_taxid.size();
*/


    while(! Stack.isEmpty()) {
        QString id = Stack.pop();
      //  qDebug() << "  id " << id;

        if( this->id_to_V.contains(id )) {
            QStringList C;

            if(this->ptaxid_to_taxid.contains(id))
                C = this->ptaxid_to_taxid[id].keys();

            this->id_to_H[id] = 0;

            if( read_counts.contains(id) ) {
                this->id_to_S[id] =  static_cast<double>(read_counts[id]);
                this->id_to_L[id] =  static_cast<double>(read_counts[id])*log( static_cast<double>(read_counts[id]));
            }
            else {
               this->id_to_S[id] = 0;
               this->id_to_L[id] = 0;
            }


            foreach(QString child, C) {
                if( this->ptaxid_to_taxid.contains(id)  )
                    if( this->ptaxid_to_taxid[id].contains(child) && this->ptaxid_to_taxid[id][child] ==true ) {
                        this->id_to_S[id] += this->id_to_S[child];
                        this->id_to_L[id] += this->id_to_L[child];
                    }
            }

            try {
                this->id_to_H[id] = -(this->id_to_L[id]/this->id_to_S[id] - log(this->id_to_S[id]) );
            }
            catch(...) {
                //QMessageBox::warning(this, "Error while LCA star computation", QString("Problem with ID :") + id, QMessageBox::Ok);
            }

            if( this->id_to_S[id] >= Total*this->lca_star_alpha) {
                if(candidate.second > this->id_to_H[id ]) {
                    candidate.first = id;
                    candidate.second = this->id_to_H[id];
                }
            }
        }
        else {
            QStringList C;
            this->id_to_V[id] = true;
            if( this->ptaxid_to_taxid.contains(id))
                C = this->ptaxid_to_taxid[id].keys();

            Stack.append(id);
            foreach(QString child, C) {
                if( this->ptaxid_to_taxid.contains(id)  )
                    if( this->ptaxid_to_taxid[id].contains(child) && this->ptaxid_to_taxid[id][child] ==true) {
                        Stack.append(child);
                    }
            }
        } //end else of if-else

    }//end while

    return candidate.first;
}


/**
 * @brief LCAStar::__clear_lca_star_data_structure, clear the structure
 */
void LCAStar::__clear_lca_star_data_structure() {
    this->id_to_R.clear();
    this->id_to_S.clear();
    this->id_to_L.clear();
    this->id_to_H.clear();
    this->id_to_V.clear();
}


/**
 * @brief LCAStar::translateIdToName, translated a valid name from the ID, if not
 * found then return empty string
 * @param id
 * @return
 */
QString LCAStar::translateIdToName(const QString &id) {
    if( !this->id_to_name.contains(id)) return QString();
    return  this->id_to_name[id];
}

/**
 * @brief LCAStar::translateNameToID, translate a valid name to its corresponding id, if
 * not found return empty string
 * @param name
 * @return
 */
QString LCAStar::translateNameToID(const QString &name) {
    if( !this->name_to_id.contains(name)) QString();
    return this->name_to_id[name];
}

/**
 * @brief LCAStar::__decolor_tree, decolors the tree for next round of calculation of
 *  lca star
 */
void LCAStar::__decolor_tree() {
    QStack<QString> S;
    S.push(QString("1"));

    while( !S.isEmpty()) {
        QString id = S.pop();

        QStringList C;
        if(this->ptaxid_to_taxid.contains(id)) {
            C = this->ptaxid_to_taxid[id].keys();
        }

        foreach(QString child, C) {
            if( this->ptaxid_to_taxid.contains(id)  )
                if( this->ptaxid_to_taxid[id].contains(child)) {
                    this->ptaxid_to_taxid[id][child] =false;
                    S.push(child);
                }
        }
    }

}

/**
 * @brief LCAStar::setParameters, sets the three parameters for the LCA star
 * algorithm
 * @param min_reads
 * @param min_depth
 * @param alpha
 */
void LCAStar::setParameters(unsigned int min_reads, unsigned int min_depth, double alpha) {
    this->lca_star_min_reads = min_reads;
    this->lca_star_min_depth = min_depth;
    this->lca_star_alpha = alpha;
}


/**
 * @brief LCAStar::taxon_depth, returns the depth of the taxon starting with 0
 * for the root
 * @param taxon
 * @return
 */
unsigned int LCAStar::taxon_depth(const QString &taxon) {
    QString id = this->translateNameToID(taxon);

    if( id.isEmpty() ) return 0;
    QString tid =  id;
    unsigned int depth = 0;
    //climb up the tree from the taxon to the root
    // the number of climbing steps is the depth
    while( this->taxid_to_ptaxid.contains(tid) && tid.compare(QString("1"))!=0  ) {
        tid = this->taxid_to_ptaxid[tid][0];
        depth += 1;
    }

    return depth;
}



LCAStar::LCAStar()
{

    this->begin_pattern.setPattern("^#");
    this->lca_star_min_reads = 1;
    this->lca_star_min_depth = 1;
    this->lca_star_alpha = 0.30;

}
