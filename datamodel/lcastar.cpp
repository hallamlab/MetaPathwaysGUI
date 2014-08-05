#include "lcastar.h"

LCAStar *LCAStar::lcastar =0;
QMutex LCAStar::mutexLock;

LCAStar *LCAStar::getLCAStar() {
  //  if( LCAStar::lcastar ==0 ) {
        LCAStar::lcastar = new LCAStar();
  //  }

    return LCAStar::lcastar;
}


void LCAStar::setNCBITree(NCBITree *ncbitree) {
    this->ncbitree = ncbitree;
}


/**
 * @brief LCAStar::loadTrees, load the tree
 */
void LCAStar::loadTrees() {
    DataManager *datamanager = DataManager::getDataManager();
    QStringList taxfiles = datamanager->getResourceFiles(NCBITREEFILE);
    foreach(QString taxfile, taxfiles) {
        this->loadTree(taxfile);
    }

}


/**
 * @brief LCAStar::loadTree, loads the NCBI tree
 */
void LCAStar::loadTree(QString treefile) {

    if( !QFileInfo(treefile).exists() ) return;
    ProgressView progressbar("Reading NCBI tree file \n" +  QFileInfo(treefile).baseName(), 0, 0, 0);

    QFile inputFile(treefile);
    QChar delim = QChar('\t');
    QStringList fields;

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();
            if (this->begin_pattern.indexIn(line) != -1  ) continue;

            fields = line.split(QRegExp(delim));
            if(fields.size()  !=3 ) continue;

            ncbitree->name_to_id[fields[0]] = fields[1];
            ncbitree->id_to_name[fields[1]] = fields[0];

            ncbitree->taxid_to_ptaxid[fields[1]] << fields[2] << 0 << 0;
            if( !ncbitree->ptaxid_to_taxid.contains(fields[2]))
                ncbitree->ptaxid_to_taxid[fields[2]] = QHash<QString, bool>();

            if( !(  fields[2].compare(QString("1")) ==0  || fields[1].compare(QString("1")) ))
                ncbitree->ptaxid_to_taxid[fields[2]][fields[1]] = false;

        }
    }
    else return;

    inputFile.close();
    progressbar.hide();
}

void LCAStar::setData(LCA_THREAD_DATA data) {
   // qDebug() << data.data->size() << data.data->size();

    this->data = data;
}

void LCAStar::run() {
    this->lca_star(this->data.data, this->data.connectors, this->data.attrType);
}

void LCAStar::lca_star( QList<ROWDATA *> *data, const QList<Connector *> *connectors,  ATTRTYPE attrType) {

    QStringList taxonList;
    unsigned int Total = data->size();
    unsigned int step = this->data.numThreads;

    unsigned int i = this->data.threadid;
    for(QList<ROWDATA *>::iterator datum = data->begin() + this->data.threadid ; i < Total ; datum = datum + step, i+= step) {
        for(unsigned int i =0; i < (*datum)->taxons.size(); i++ ) {
            taxonList.clear();
            this->getTaxonList(*datum, i, connectors, taxonList, attrType);
            LCARESULT lcaresult = this->lca_star(taxonList);
            (*datum)->taxons[i] = lcaresult.taxon;
            (*datum)->counts[i] = lcaresult.count;
            (*datum)->lcaresults[i] = lcaresult;
        }
    }
}


void LCAStar::getTaxonList(ROWDATA *datum, unsigned int sampleNum, const QList<Connector *> *connectors,  QStringList &taxonList, ATTRTYPE attrType) {

    QString category = datum->name;

    DataManager *datamanager = DataManager::getDataManager();
    HTree *htree = datamanager->getHTree(attrType);
    HNODE *hnode = htree->getHNODE(category);
    if( htree==0 || hnode ==0 ) return;

    Connector *connector =   connectors->operator [](sampleNum);

    QList<ORF *> orfList = connector->getORFList(htree->getLeafAttributesOf(hnode));

    foreach(ORF *orf, orfList) {
      if( orf->attributes.contains(TAXON)  ) taxonList.append( orf->attributes[TAXON]->name);
    }
    return;

}


/**
 * @brief LCAStar::lca_star, computes the lca star value from a list of taxa
 * @param taxalist
 * @return
 */

LCARESULT LCAStar::lca_star(const QStringList &taxalist) {
    QStringList _taxalist = this->filter_taxa_list(taxalist);

    LCARESULT lcaresult;
    lcaresult.taxon = QString("-");
    lcaresult.count = 0;

    if( _taxalist.isEmpty() ) return lcaresult;


    LCARESULT majority = this->__lca_majority(_taxalist);

    if( !majority.taxon.isEmpty() ) return majority;

    QHash<QString , unsigned int> read_counts;

    this->__read_counts(_taxalist, read_counts);

    this->__annotate_tree_counts( read_counts );

    this->__color_tree(read_counts);


    LCARESULT result = __create_majority("1", read_counts);


    this->__clear_lca_star_data_structure();

    result.taxon = this->translateIdToName(result.taxon);
    result.total = taxalist.size();
    this->__decolor_tree();

    return result;
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

LCARESULT LCAStar::__lca_majority(const QStringList &taxalist) {
    // create the read counts
    QHash<QString, unsigned int> read_counts;
    LCARESULT majority;
    majority.taxon = QString();
    majority.count = 0;

    unsigned int  Total =  this->__read_counts(taxalist, read_counts);

    // find the taxon with the highest count but also has count higher than the
     // majority threshold

    unsigned int maxcount =0;

    foreach(QString taxon, read_counts.keys()) {
      if(maxcount < read_counts[taxon] && Total*this->lca_star_alpha < read_counts[taxon]) {
         majority.taxon = taxon;
         maxcount = read_counts[taxon];
         majority.count = maxcount;
       }
    }

    return majority;

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
        while( ncbitree->taxid_to_ptaxid.contains(tid) && tid.compare(QString("1")) !=0) {
            QString pid = ncbitree->taxid_to_ptaxid[tid][0];
            ncbitree->ptaxid_to_taxid[pid][tid] = true;
            tid = pid;
        }
    }
}



/**
 * @brief LCAStar::__create_majority, crates a majority by climbing up the NCBI tree until a majority
 * is found with least entropy distortion
 * @param root
 * @param read_name_counts
 * @return the majority of the new collapsed taxons
 */

LCARESULT LCAStar::__create_majority(const QString &root, const QHash<QString, unsigned int> &read_name_counts) {

    QHash<QString, unsigned int> read_counts;
    unsigned int Total=0;

    foreach(QString taxon, read_name_counts.keys() ) {
        QString id = this->translateNameToID(taxon);
        read_counts[id] = read_name_counts[taxon];
        Total += read_name_counts[taxon];
    }

    CANDIDATE candidate;
    candidate.first = QString("1");
    candidate.second = 1e20;
    candidate.count = 7;
    QStack<QString> Stack;
    Stack.push(root);


    while(! Stack.isEmpty()) {
        QString id = Stack.pop();
      //  qDebug() << "  id " << id;

        if( !this->id_to_V.contains(id )) {  // way down to the tree
            QStringList C;
            this->id_to_V[id] = true;   //have visited now

            this->id_to_F[id] = false;

            if( ncbitree->ptaxid_to_taxid.contains(id))
                foreach(QString child, ncbitree->ptaxid_to_taxid[id].keys())
                   if( ncbitree->ptaxid_to_taxid[id][child] ==true )
                       C.append(child);

            Stack.append(id);
            foreach(QString child, C) {
               Stack.append(child);
            }

        } //end else of if-else
        else { //way up to the tree
            QStringList C;

            this->id_to_H[id] = 0;
            if(ncbitree->ptaxid_to_taxid.contains(id) ) {
                C = ncbitree->ptaxid_to_taxid[id].keys();
                foreach(QString child, C)
                    if( this->id_to_F[child] ) {
                        this->id_to_F[id] = true; break;
                    }
            }
           // if(this->id_to_F[id] ) continue;

            if(this->id_to_F[id] ==false) { // child without a majority
                if( read_counts.contains(id)) {
                    this->id_to_S[id] =  static_cast<double>(read_counts[id]);
                    this->id_to_L[id] =  static_cast<double>(read_counts[id])*log( static_cast<double>(read_counts[id]));
                }
                else {
                   this->id_to_S[id] = 0;
                   this->id_to_L[id] = 0;
                }
             }

             // also add the childres values
            foreach(QString child, C) {
                 this->id_to_S[id] += this->id_to_S[child];
                 this->id_to_L[id] += this->id_to_L[child];
            }


        //    qDebug() <<  "+" << this->id_to_S[id] <<  Total <<  Total*this->lca_star_alpha;
            if( this->id_to_S[id] >= Total*this->lca_star_alpha && this->id_to_F[id] == false ) {
                try {
                    double  spart;
                    if( this->id_to_S[id]==0) spart = 1;

                    this->id_to_H[id] = -(this->id_to_L[id]/spart - log(spart) );
                }
                catch(...) {
                    QMessageBox::warning(0, "Error while LCA star computation", QString("Problem with ID :") + id, QMessageBox::Ok);
                }
               // qDebug() <<  this->id_to_S[id] <<  Total <<  Total*this->lca_star_alpha << candidate.second << this->id_to_H[id];

                this->id_to_F[id] = true; // found a majority

                if(candidate.second > this->id_to_H[id ]) {
                 //    qDebug() <<  "--" << this->id_to_S[id] <<  Total <<  Total*this->lca_star_alpha << candidate.second << this->id_to_H[id];
                    candidate.first = id;
                    candidate.second = this->id_to_H[id];
                    candidate.count = this->id_to_S[id];
                }
            }  //if
        }
    }//end while



    LCARESULT lcaresult;
    lcaresult.taxon = candidate.first;
    lcaresult.count = candidate.count;
    lcaresult.valid = Total;
  //  qDebug() << "====" << lcaresult.taxon <<  lcaresult.count << candidate.count << Total;

    return lcaresult;
}

/*
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

//    foreach(QString key, read_name_counts.keys())
  //      qDebug() << " key -value " << key <<  read_name_counts[key];

   // qDebug() << " id to H " << this->id_to_H.size();
   // qDebug() << " id to V " << this->id_to_V.size();
   // qDebug() << " ptaxid to taxid " << this->ptaxid_to_taxid.size();



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
*/

/**
 * @brief LCAStar::__clear_lca_star_data_structure, clear the structure
 */
void LCAStar::__clear_lca_star_data_structure() {


    /*
    for(QHash<QString, double>::iterator it = this->id_to_S.begin(); it!= this->id_to_S.end(); it++ ) {
        it.value() = 0;
    }

    for(QHash<QString, double>::iterator it = this->id_to_L.begin(); it!= this->id_to_L.end(); it++ ) {
        it.value() = 0;
    }

    for(QHash<QString, double>::iterator it = this->id_to_H.begin(); it!= this->id_to_H.end(); it++ ) {
        it.value() = 0;
    }
*/

/*
    for(QHash<QString, bool>::iterator it = this->id_to_V.begin(); it!= this->id_to_V.end(); it++ ) {
        it.value() = false;
    }


    for(QHash<QString, bool>::iterator it = this->id_to_F.begin(); it!= this->id_to_F.end(); it++ ) {
        it.value() = false;
    }
*/
    this->id_to_R.clear();


    this->id_to_S.clear();
    this->id_to_L.clear();

    this->id_to_H.clear();

    this->id_to_V.clear();
    this->id_to_F.clear();
}


/**
 * @brief LCAStar::translateIdToName, translated a valid name from the ID, if not
 * found then return empty string
 * @param id
 * @return
 */
QString LCAStar::translateIdToName(const QString &id) {
    if( !ncbitree->id_to_name.contains(id)) return QString();
    return  ncbitree->id_to_name[id];
}

/**
 * @brief LCAStar::translateNameToID, translate a valid name to its corresponding id, if
 * not found return empty string
 * @param name
 * @return
 */
QString LCAStar::translateNameToID(const QString &name) {
    if( !ncbitree->name_to_id.contains(name)) QString();
    return ncbitree->name_to_id[name];
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
        if(ncbitree->ptaxid_to_taxid.contains(id)) {
            C = ncbitree->ptaxid_to_taxid[id].keys();
        }

        foreach(QString child, C) {
            if( ncbitree->ptaxid_to_taxid.contains(id)  )
                if( ncbitree->ptaxid_to_taxid[id].contains(child)) {
                    ncbitree->ptaxid_to_taxid[id][child] =false;
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
  * @brief LCAStar::setLCAStarAlpha, sets the lca start alpha
  * @param alpha
  */
void LCAStar::setLCAStarAlpha(double alpha) {
     this->lca_star_alpha = alpha;
}


/**
  * @brief LCAStar::setLCAStarDepth, sets the lca min depth
  * @param depth
  */
void LCAStar::setLCAStarDepth(unsigned int depth) {
     this->lca_star_min_depth =depth;
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
    while( ncbitree->taxid_to_ptaxid.contains(tid) && tid.compare(QString("1"))!=0  ) {
        tid = ncbitree->taxid_to_ptaxid[tid][0];
        depth += 1;
    }

    return depth;
}



/**
 * @brief LCAStar::getToolTipText, creates the tool tip for the list of taxons
 * @param taxons
 * @return
 */
QString LCAStar::getToolTipText(const QStringList &taxons, const FREQUENCY &__lca_star ) {
    QList<FREQUENCY> freq;
    unsigned int maxCount = this->getTaxonsDistributions(taxons, freq);

    QString tooltip = this->getTaxonsDistribtionTooltipTxt(freq, maxCount, __lca_star);
    return tooltip;
}


/**
 * @brief LCAStar::getTaxonsDistributions, creates a list of taxon distribution from a
 * list of taxons
 * @param taxons
 * @param freq
 * @return
 */
unsigned int LCAStar::getTaxonsDistributions(const QStringList &taxons, QList<FREQUENCY> &freq) {
    QHash<QString, unsigned int>  hashCount;
    foreach(QString taxon, taxons) {
        if( !hashCount.contains( taxon) ) hashCount[taxon] =0;
         hashCount[taxon]++;
    }


    QList<TaxonFreqQPair > taxonFreqList;
    double totalCount =0;
    foreach(QString key, hashCount.keys()) {
        taxonFreqList.append(TaxonFreqQPair(key, hashCount[key]) );
        totalCount += static_cast<double>(hashCount[key]);
    }
    if( totalCount ==0) totalCount = 1;

    qSort(taxonFreqList.begin(), taxonFreqList.end(), Utilities::compareFreq);

    unsigned int maxCount =0;
    for(QList<TaxonFreqQPair>::iterator it = taxonFreqList.begin(); it!= taxonFreqList.end(); ++it) {
        FREQUENCY item;
        item.name = it->first;
        item.count = it->second;
        item.percent = (it->second/totalCount)*100;
        freq.append(item);
        if( item.count > maxCount) maxCount = item.count;
    }

    return maxCount;

}

/**
 * @brief LCAStar::getTaxonsDistribtionTooltipTxt, creates the distribution for the tooltip
 * display, maxCount is use for the scaling
 * @param freq
 * @param maxCount, used for the scaling  of the tooltip image
 * @return
 */
QString LCAStar::getTaxonsDistribtionTooltipTxt(const QList<FREQUENCY> &freq, const unsigned int maxCount, const FREQUENCY &__lca_star__) {
    QString header = QString(" <table style  width='100%' height='100%'> <caption align='center'> <h3> Taxonomic Distribution (At most top 5) </h3> <br>");

    header += QString("LCA* Taxon :  ") + __lca_star__.name  + QString(" : ") +  QString::number(__lca_star__.count)
            + QString(" (") + QString::number(__lca_star__.percent, 'f', 2)  + QString("%)") + QString("<br>");


    header += QString("# ORFs Total :  ")  + QString::number(__lca_star__.total) + QString("<br>");
    header += QString("# ORFs used in LCA*  :  ")  + QString::number(__lca_star__.valid) + QString("<br> </caption>");
    QString tail =    QString("</table> <br> </div>");

    QString string = header;
    unsigned int i =0;
    foreach( FREQUENCY item, freq) {
       if( i > 5) {
           string += this->getTaxonDistributionToolTipTxt(QString("......."), 0, 0, 0, true);
           break;
       }
       i++;
       string += this->getTaxonDistributionToolTipTxt(item.name, item.count, item.percent, maxCount);
    }
    string += tail;
    return string;
}


/**
 * @brief LCAStar::getTaxonDistributionToolTipTxt, compute the bar for one taxon representing count percent and maxCount
 * @param name
 * @param count
 * @param percent
 * @param maxCount
 * @param shorten
 * @return
 */
QString LCAStar::getTaxonDistributionToolTipTxt(const QString &name, const unsigned int count, const float percent, const unsigned int maxCount, bool shorten) {
    unsigned int width = static_cast<unsigned int>(static_cast<double>(count)*200/static_cast<double>(maxCount));
    QString row = QString("<tr> <td style= 'white-space:nowrap;' > ") + name +
                  QString(" </td>  <td> <img src=':/images/blue.png' height='12' width='") +
                  QString::number(width) +
                  QString("' align='left' />  </td> <td style= 'white-space:nowrap;'>");
    if( !shorten)
        row += QString::number(count) + QString(" (") + QString::number(percent, 'f', 2)  + QString("%)");

    row +=  QString("</td> </tr>");

    return row;
}

LCAStar::LCAStar()
{

    this->begin_pattern.setPattern("^#");
    this->lca_star_min_reads = 1;
    this->lca_star_min_depth = 1;
    this->lca_star_alpha = 0.30;

    mutexLock.lock();
    this->ncbitree = NCBITree::getNCBITree();
    mutexLock.unlock();

  //  qDebug() << "tax id " << this->ncbitree->taxid_to_ptaxid.size();
}
