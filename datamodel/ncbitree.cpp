#include "ncbitree.h"


NCBITree *NCBITree::ncbitree = 0;

NCBITree::NCBITree()
{

    this->begin_pattern.setPattern("^#");
}


NCBITree *NCBITree::getNCBITree() {
    if(  NCBITree::ncbitree ==0 ) {
         NCBITree::ncbitree = new NCBITree;
         NCBITree::ncbitree->loadTrees();
    }

    return NCBITree::ncbitree;
}

void NCBITree::loadTrees() {
    DataManager *datamanager = DataManager::getDataManager();
    QStringList treefiles = datamanager->getResourceFiles(NCBITREEFILE);


    foreach(QString treefile, treefiles) {
        this->loadTree(treefile);
    }

    QStringList mapfiles = datamanager->getResourceFiles(NCBINAMEMAPFILE);
    qDebug() << mapfiles;

    foreach(QString mapfile, mapfiles) {
        this->loadMeganMap(mapfile);
    }


}


/**
 * @brief LCAStar::loadTree, loads the NCBI tree
 */
void NCBITree::loadTree(QString treefile) {

    if(!QFileInfo(treefile).exists() ) return;

   // ProgressView progressbar("Reading NCBI tree file \n" +  QFileInfo(treefile).baseName(), 0, 0, 0);


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

    inputFile.close();
   // progressbar.hide();
}

/**
 * @brief LCAStar::loadMeganMap, loads the NCBI taxon name map
 */
void NCBITree::loadMeganMap(QString mapfile) {

    if(!QFileInfo(mapfile).exists() ) return;

    ProgressView progressbar("Reading NCBI taxon map file \n" +  QFileInfo(mapfile).baseName(), 0, 0, 0);


    QFile inputFile(mapfile);
    QChar delim = QChar('\t');
    QStringList fields;

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();
            if (this->begin_pattern.indexIn(line) != -1  ) continue;

            fields = line.split(QRegExp(delim));
            if(fields.size()  !=3 ) continue;
            this->id_to_namemap[fields[0]] = fields[1];
        }
    }
    else return;

    inputFile.close();
    progressbar.hide();
}


QString NCBITree::translateNameToID(const QString &name) {
    if( !ncbitree->name_to_id.contains(name)) QString();
    return ncbitree->name_to_id[name];
}


QString NCBITree::translateIdToName(const QString &id, bool meganfirst) {

  if(meganfirst && this->id_to_namemap.contains(id)) return this->id_to_namemap[id];

    if( !this->id_to_name.contains(id)) return QString();
    return  this->id_to_name[id];
}


QString NCBITree::grabNameinQuotes(const QString &name) {
    QRegExp pattern1, pattern2;
    QString newName;
    pattern1.setPattern("[\"](.*)[\"]");
    pattern2.setPattern("['](.*)[']");

    newName = name;

    int pos  = pattern1.indexIn(newName);
    if( pos > -1 ) {
        newName = pattern1.cap(1);
    }

    pos  = pattern2.indexIn(newName);
    if( pos > -1 ) {
        newName = pattern2.cap(1);
    }

    return newName;
}

QString NCBITree::getLineage(QString taxon) {
  QString id = this->translateNameToID(taxon);

  QString tid = id;
  QString lineage = this->grabNameinQuotes( taxon);
  while( this->taxid_to_ptaxid.contains(tid) && tid.compare(QString("1")) !=0) {
      QString pid = this->taxid_to_ptaxid[tid][0];
      lineage = this->grabNameinQuotes(this->translateIdToName(pid,true)) + ";" + lineage;
      this->ptaxid_to_taxid[pid][tid] = true;
      tid = pid;
  }

  return lineage;

}

