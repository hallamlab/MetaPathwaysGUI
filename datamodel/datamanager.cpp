#include "datamanager.h"


DataManager *DataManager::datamanager = 0;

DataManager *DataManager::getDataManager() {
    if( datamanager==0)
        datamanager = new DataManager;

    return datamanager;

}

DataManager::DataManager()
{
    ORFList = new QHash<QString, QList<ORF *>*>();
    this->setDataModelCreated(false);
            //    dataModelCreated = false;
}

void DataManager::setDataModelCreated(bool flag) {
    this->dataModelCreated = flag;
}

bool DataManager::getDataModelCreated() {
    return this->dataModelCreated;
}


void DataManager::setORFsUptoDateAll(bool flag) {
    foreach( QString sample, this->ORFsUptoDateList.keys())
       this->ORFsUptoDateList[sample] = flag;
}


bool DataManager::AreORFsUptoDate(QString sample) {
    if( !this->ORFsUptoDateList.contains(sample)) return false;
    return this->ORFsUptoDateList[sample];
}

void DataManager::setORFsUptoDate(QString sample, bool flag){
    this->ORFsUptoDateList[sample] = flag;
}

HTree *DataManager::getHTree(ATTRTYPE atrType) {

    if(htrees.contains(atrType) )
        return htrees[atrType];

    return 0;
}

QString DataManager::getORFFaa(QString sampleName) {


    return QString();
}

QString DataManager::getORFFna(QString sampleName) {



}

QString DataManager::getORFFasta(QString sampleName) {



}

void DataManager::setIndexFileFaa(QString sampleName, QString fileName) {



}

void DataManager::setIndexFileFna(QString sampleName, QString fileName) {



}

void DataManager::setIndexFileFasta(QString sampleName, QString fileName) {



}

QStringList DataManager::getConnectorSamples() {

    return this->connectors.keys();
}


QList<ATTRTYPE> DataManager::getConnectorSampleAttributes(QString sampleName) {
    if( this->connectors.contains(sampleName) ) {
        return this->connectors[sampleName].keys();
    }
    return QList<ATTRTYPE>();

}

Connector *DataManager::getConnector( QString sampleName, ATTRTYPE type) {
    if(this->connectors.contains(sampleName) && this->connectors[sampleName].contains(type) ) {
        return this->connectors[sampleName][type];
    }
    return 0;
}


/**
 * @brief DataManager::createCategoryNode, creates a node for CATEGORYNODE
 *  for the hierarchical tree for the line
 * @param line
 * @return
 */
CATEGORYNODE DataManager::createCategoryNode(QString line) {
    QRegExp d1("^[\\t]");
    QRegExp d2("^[\\t][\\t]");
    QRegExp d3("^[\\t][\\t][\\t]");
    QRegExp d4("^[\\t][\\t][\\t][\\t]");
    QRegExp d5("^[\\t][\\t][\\t][\\t][\\t]");
    QRegExp d6("^[\\t][\\t][\\t][\\t][\\t][\\t]");
    QRegExp d7("^[\\t][\\t][\\t][\\t][\\t][\\t][\\t]");
    QRegExp d8("^[\\t][\\t][\\t][\\t][\\t][\\t][\\t][\\t]");
    QRegExp d9("^[\\t][\\t][\\t][\\t][\\t][\\t][\\t][\\t][\\t]");
    QRegExp d10("^[\\t][\\t][\\t][\\t][\\t][\\t][\\t][\\t][\\t][\\t]");

    QString name;
    unsigned int depth;

    if( line.indexOf(d10) > -1 ) {
        depth = 10;
        name = line.replace(d10,"");
    }
    else if( line.indexOf(d9) > -1 ) {
        depth = 9;
        name = line.replace(d9,"");
    }
    else if( line.indexOf(d8) > -1 ) {
        depth = 8;
        name = line.replace(d8,"");
    }
    else if( line.indexOf(d7) > -1 ) {
        depth = 7;
        name = line.replace(d7,"");
    }
    else if( line.indexOf(d6) > -1 ) {
        depth = 6;
        name = line.replace(d6,"");
    }
    else if( line.indexOf(d5) > -1 ) {
        depth = 5;
        name = line.replace(d5,"");
    }
    else if( line.indexOf(d4) > -1 ) {
        depth = 4;
        name = line.replace(d4,"");
    }
    else if( line.indexOf(d3) > -1) {
        depth = 3;
        name = line.replace(d3,"");
    }
    else if( line.indexOf(d2) > -1) {
        depth = 2;
        name = line.replace(d2,"");
    }
    else if( line.indexOf(d1) > -1) {
        depth = 1;
        name = line.replace(d1,"");
    }
    else {
        depth = 0;
        name = line;
    }

    QStringList fields = line.trimmed().split("\t");

    CATEGORYNODE node;
    node.depth  = depth;
    node.name = fields[0];
    if( fields.size() > 1)
        node.alias = fields[1];
    else
        node.alias = "";
    return node;
}


/**
 * @brief DataManager::getResourceFile, gets the type of resource file for the
 * run
 * @param resType
 * @return
 */
QStringList DataManager::getResourceFiles(const RESOURCETYPE &resType) {

    QString NCBI_TREE_FILE  = QString("ncbi_taxonomy_tree.txt");
    RunData *rundata = RunData::getRunData();

    QStringList filePaths;
    QString dirPath = rundata->getValueFromHash("REFDBS", _CONFIG ) + QDir::separator() + QString("ncbi_tree") + QDir::separator();
    QDir dir(dirPath);

    QDir mapDir(dirPath + QDir::separator() + "mapping");
    if(!dir.exists() ) return filePaths;

    switch(resType ) {
         case NCBITREEFILE:
             if(QFileInfo(dirPath + QDir::separator()+NCBI_TREE_FILE).exists()) filePaths.append(dirPath + QDir::separator() + NCBI_TREE_FILE);

             foreach(QString fileName, dir.entryList()) {
                 if( fileName.compare(QString("."))==0  || fileName.compare(QString(".."))==0  || fileName.compare(NCBI_TREE_FILE)==0) continue;
                 filePaths.append(dirPath + QDir::separator() + fileName);
             }
             break;

      case NCBINAMEMAPFILE:

          if(mapDir.exists())
            foreach(QString fileName, dir.entryList()) {
                if( fileName.compare(QString("."))==0  || fileName.compare(QString(".."))==0) continue;
                filePaths.append(dirPath + QDir::separator() + fileName);
            }
          break;

         default:
              filePaths.clear();
              break;
    }

    return filePaths;

}

/**
 * @brief DataManager::createDataModel, creates the different functional trees
 */
void DataManager::createDataModel() {

    if( dataModelCreated ) return;

  //  QString refDBFolder = rundata->getValueFromHash("REFDBS", _CONFIG);
    //QString COG_categories = refDBFolder + "/functional_categories/" + "COG_categories.txt";
    QString COG_categories = QString(":/functional_categories/") + "COG_categories.txt";
    HTree *htree = createHTree(COG_categories, COG);

    htree->hashNodes(htree->root);
    this->htrees[COG] = htree;
  //  htree->printTree(htree->getRootHNODE());


    //QString KEGG_categories = refDBFolder + "/functional_categories/" + "KO_classification.txt";
    QString KEGG_categories =  QString(":/functional_categories/") + "KO_classification.txt";
    htree = createHTree(KEGG_categories, KEGG);
    htree->hashNodes(htree->root);
    this->htrees[KEGG] = htree;

    //QString MetaCyc_hierarchy = refDBFolder + "/functional_categories/" + "metacyc_hierarchy.txt";
    QString MetaCyc_hierarchy = QString(":/functional_categories/") + "metacyc_hierarchy.txt";
    htree = createHTree(MetaCyc_hierarchy, METACYC);
    htree->hashNodes(htree->root);
    this->htrees[METACYC] = htree;

    htree = this->createBaseTree(this->htrees[METACYC]);
    htree->hashNodes(htree->root);
    htree->setAttrType(METACYCBASE);
    this->htrees[METACYCBASE] = htree;


    //QString Seed_subsystems = refDBFolder + "/functional_categories/" + "SEED_subsystems.txt";
    QString Seed_subsystems =  QString(":/functional_categories/") + "SEED_subsystems.txt";
    htree = createHTree(Seed_subsystems, SEED);
    htree->hashNodes(htree->root);
    this->htrees[SEED] = htree;

    QString CAZY_hierarchy =  QString(":/functional_categories/") + "CAZY_hierarchy.txt";
    htree = createHTree(CAZY_hierarchy, CAZY);
    htree->hashNodes(htree->root);
    this->htrees[CAZY] = htree;

    this->setDataModelCreated(true);

}

/**
 * @brief DataManager::createHTree, creates a hierarchical functional tree from the
 * file that is tab indented at the front to suggest relative level from the preceeding line
 * @param refDBFileName
 * @return
 */
HTree *DataManager::createHTree(QString refDBFileName, ATTRTYPE type) {
    HTree *htree = new HTree();

    htree->setAttrType(type);
    QStack<HNODE *> S;
    QHash<QString, ATTRIBUTE *> created;


    HNODE *hnode = new HNODE;
    htree->root = hnode;
    hnode->attribute = new ATTRIBUTE;
    hnode->attribute->name = "root";
    hnode->attribute->alias = "root";
    hnode->depth = -1;

    created["root"] = hnode->attribute;
    S.push(hnode);

    QFile inputFile(refDBFileName);
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            CATEGORYNODE cnode = createCategoryNode(in.readLine());
            while(S.top()->depth >= cnode.depth)
                S.pop();

            hnode = new HNODE;
            if(!created.contains(cnode.name)) {
                hnode->attribute = new ATTRIBUTE;
                hnode->attribute->name = cnode.name;
                hnode->attribute->alias = cnode.alias;
                created[cnode.name] = hnode->attribute;
            }
            else {
                hnode->attribute = created[cnode.name];
            }
            hnode->depth  = cnode.depth;

            S.top()->children.append(hnode);
            S.push(hnode);
        }
        inputFile.close();
    }
    return htree;
}

/**
 * @brief DataManager::createBaseTree, takes a tree and creates a tree only with the base pathways, i.e., the node with the
 * nodes before the leave nodes
 * @param shtree, the source tree
 * @return
 */
HTree *DataManager::createBaseTree(HTree *shtree) {
    HTree *htree = new HTree();
    QHash<QString, bool> created;

    HNODE *hnode = new HNODE;
    htree->root = hnode;
    hnode->attribute = new ATTRIBUTE;
    hnode->attribute->name = "root";
    hnode->attribute->alias = "root";
    hnode->children.clear();
    hnode->depth = -1;
    created["root"] = hnode->attribute;
    _createBaseTree(shtree->root, htree->root, created);
    return htree;
}

/**
 * @brief DataManager::_createBaseTree, it recursively creates the  base pathway tree
 * @param hnode, the recursively travelled node
 * @param newnode, the root of the new tree
 * @param created, hash to keep track of which nodes have been created
 * @return true if exists through the leaf and false otherwise
 */
bool DataManager::_createBaseTree(HNODE *hnode, HNODE *newnode, QHash<QString, bool> &created){

    if( hnode->children.isEmpty() ) return true;

    foreach( HNODE *cnode, hnode->children) {
        if( _createBaseTree(cnode, newnode, created) && ! created.contains(hnode->attribute->name) ) {
            newnode->children.append(hnode);
            created[hnode->attribute->name] = true;
        }
    }

    return false;
}



/**
 * @brief DataManager::destroyAllHTrees, deallocates all the functional trees
 */
void DataManager::destroyAllHTrees() {
    foreach(ATTRTYPE attr, this->htrees.keys()) {
        destroyHTree(attr);
    }

}

/**
 * @brief DataManager::_destroyHTree, deletes all the nodes of a  functional (hierarchical) tree
 * rooted at hnode
 * @param hnode
 */
void DataManager::_destroyHTree(HNODE *hnode) {

    foreach( HNODE *child, hnode->children) {

        try {
            // qDebug() << QString("Error while destropying HTree for treetype ") + hnode->attribute->name;
            _destroyHTree(child);
        }
        catch(...) {
         //   qDebug() << QString("Error while destropying HTree for treetype ") + hnode->attribute->name;
            Utilities::showInfo(0, QString("Error while destropying HTree for treetype ") + hnode->attribute->name );
            return;
        }
    }
    delete hnode;
}


/**
 * @brief DataManager::destroyAllAttributes, clears out all the attributes for all types
 */
void DataManager::destroyAllAttributes() {
    foreach( ATTRTYPE attr, this->attributes.keys() ) {
        foreach(ATTRIBUTE *attrib, this->attributes[attr].values())
            delete attrib;

        foreach(QString key, this->attributes[attr].keys())
            this->attributes[attr].remove(key);

        this->attributes.remove(attr);
    }

}

/**
 * @brief DataManager::destroyAllTaxons, destroys all the taxons held in the
 * singleton class of Taxons;
 */
void DataManager::destroyAllTaxons() {
    Taxons *taxons = Taxons::getTaxons();
    taxons->clearTaxons();
}


/**
 * @brief DataManager::destroyHTree, deletes the tree for an attribute
 * @param treeType, the name of the attribute to remove the tree for
 */
void DataManager::destroyHTree(ATTRTYPE treeType ) {

     HTree *htree = this->getHTree(treeType);

     if( htree->getAttrType() == METACYCBASE)  { this->htrees.remove(treeType); return; }

     HNODE *hnode = htree->getRootHNODE();
     try {
      //   qDebug() << "value of htree " << hnode;
        //  qDebug() << QString("Error while destropying HTree for treetype ") + QString::number(htree->getAttrType()) ;
         _destroyHTree(hnode);
     }
     catch(...) {
         qDebug() << QString("Error while destropying HTree for treetype ") + QString::number(htree->getAttrType()) ;
         Utilities::showInfo(0, QString("Error while destropying HTree for treetype ") + QString::number(htree->getAttrType() ));
     }
     hnode = 0;
     this->htrees.remove(treeType);
}


/**
 * @brief DataManager::addTaxons, added the list of taxons to the singleton class Taxons
 * from the file supplied
 * @param sampleName
 */
void  DataManager::addTaxons(const QString &sampleName) {
    Taxons *taxons = Taxons::getTaxons();
    taxons->addTaxons( sampleName, 8);
}


/**
 * @brief DataManager::_createAnORF creates and ORF from the attributes
 * @param attributes attribute list as provided from the orf list tables, orf_annotation
 * @return
 */
ORF *DataManager::_createAnORF(QStringList &attributes, QString &sampleName) {

    ORF *orf = new ORF;


    orf->name =  Utilities::getShortORFId(attributes[0]);

    QString shortContigName =  Utilities::getShortContigId(attributes[1]);

    if( !contigHash.contains(sampleName)) this->contigHash[sampleName] = QHash<QString, CONTIG *>();

    if(contigHash[sampleName].contains(shortContigName))
        orf->contig = this->contigHash[sampleName][shortContigName];
    else {
        CONTIG *contig = new CONTIG;
        contig->name  = shortContigName;
        this->contigHash[sampleName][shortContigName] = contig;
        orf->contig = contig;
    }
    orf->contig->orfList.append(orf);
   // qDebug() << orf->contig->name  << "   " << orf->name << "    ";

   // qDebug() << attributes;
    if( attributes.size() <= 2) return orf;
    if(this->attributes[COG].contains(attributes[2]))
        orf->attributes[COG] = this->attributes[COG][attributes[2]];
    else {
        orf->attributes[COG] = new ATTRIBUTE;
        orf->attributes[COG]->name = attributes[2];
        this->attributes[COG][attributes[2]] = orf->attributes[COG];
    }

    if( attributes.size() <= 3) return orf;
    if(this->attributes[KEGG].contains(attributes[3]))
        orf->attributes[KEGG] = this->attributes[KEGG][attributes[3]];
    else {
        orf->attributes[KEGG] = new ATTRIBUTE;
        orf->attributes[KEGG]->name = attributes[3];
        this->attributes[KEGG][attributes[3]] = orf->attributes[KEGG];
    }

    if( attributes.size() <= 4) return orf;
    if(this->attributes[SEED].contains(attributes[4]))
        orf->attributes[SEED] = this->attributes[SEED][attributes[4]];
    else {
        orf->attributes[SEED] = new ATTRIBUTE;
        orf->attributes[SEED]->name = attributes[4];
        this->attributes[SEED][attributes[4]] = orf->attributes[SEED];
    }

    if( attributes.size() <= 5) return orf;
    if(this->attributes[CAZY].contains(attributes[5]))
        orf->attributes[CAZY] = this->attributes[CAZY][attributes[5]];
    else {
        orf->attributes[CAZY] = new ATTRIBUTE;
        orf->attributes[CAZY]->name = attributes[5];
        this->attributes[CAZY][attributes[5]] = orf->attributes[CAZY];
    }
   // qDebug() << attributes.size();

    return orf;
}


/**
 * @brief DataManager::createORFs, creates the orf for a sample from the given
 *  file
 * @param sampleName, the sample name to add the ORFs for
 * @param fileName, the file than to read the ORF info from
 */
void DataManager::createORFs(QString sampleName) {

   // if(this->ORFsUptoDateList.contains(sampleName) &&  this->ORFsUptoDateList[sampleName] ) return;
    //return if already created;
      if(this->ORFList->contains(sampleName)) {
          return;
      }
   // qDebug() << "ORFs exists for the smaple " << sampleName;
   /* if( this->ORFList->contains(sampleName)) {
        this->ORFList->clear(); //memory leak
       // qDebug() << "Clearing the clog";
    }*/

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
    QString orfTableName = samplercmgr->getFilePath(sampleName, ORFTABLE);

    this->ORFList->insert(sampleName, new QList<ORF *>);

    QHash<QString, bool>  _orfList;

    QFile inputFile(orfTableName);
    unsigned int count = 0;
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QStringList line = in.readLine().remove(("[\\n]")).split(QRegExp("[\\t]"));
            if(line.size() < 5) { continue; }
            ORF *orf = this->_createAnORF(line, sampleName);
            (this->ORFList->value(sampleName))->append(orf);
            _orfList[orf->name] = true;
            count++;
        }
        inputFile.close();
    }


    QString functionalTableName = samplercmgr->getFilePath(sampleName, FUNCTIONALTABLE);

    QString skip= "ORF_ID";
    inputFile.setFileName(functionalTableName);
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QStringList line = in.readLine().remove(("[\\n]")).split(QRegExp("[\\t]"));
            if(line.size() <4 ) { continue; }
            QStringList attributes;
            attributes << line[0] << line[4] << QString("") << QString("") << QString("");
            if( line[0].compare(skip)==0) continue;

            QString shortORFName = Utilities::getShortORFId(attributes[0]);
            if( _orfList.contains(shortORFName) ) continue;
            ORF *orf = this->_createAnORF(attributes, sampleName);
            (this->ORFList->value(sampleName))->append(orf);
            _orfList[orf->name] = true;
            count++;
        }
        inputFile.close();
    }

    this->attributes[COG].clear();
    this->attributes[KEGG].clear();
    this->attributes[SEED].clear();
    this->attributes[CAZY].clear();

 //   this->ORFsUptoDateList[sampleName] = true;
}

/**
 * @brief DataManager::destroyAllORFs deletes all the orfs sample by sample
 */
void DataManager::destroyAllORFs() {
    foreach( QString sampleName, this->ORFList->keys())
        this->destroyORFs(sampleName);
}

/**
 * @brief DataManager::destroyAllContigs, deletes all the contigs one sample at a time
 */
void DataManager::destroyAllContigs() {
    foreach( QString sampleName, this->contigHash.keys())
        this->destroyContigs(sampleName);
}

/**
 * @brief DataManager::destroyContigs, deletes all the contigs for one sample
 * @param sampleName
 */
void DataManager::destroyContigs(QString &sampleName) {

    foreach(QString contigName, this->contigHash[sampleName].keys()) {
        delete this->contigHash[sampleName][contigName];
    }
    this->contigHash.remove(sampleName);
}


/**
 * @brief deleteORF, deletes an orf
 * @param orf
 */
void deleteORF(ORF *orf) {
    delete orf;

}

/**
 * @brief DataManager::destroyORFs, deletes all the orfs for a sample
 * @param sampleName
 */
void DataManager::destroyORFs(QString sampleName) {

    if( !this->ORFList->contains(sampleName)) return;
    const QList<ORF *> *orfList = this->ORFList->value(sampleName);

    unsigned int i =0;
    foreach(ORF *orf, *orfList) {
        i++;
        //delete orf;
        deleteORF(orf);
    }

   // this->ORFList->value(sampleName)
    this->ORFList->remove(sampleName);

}


/**
 * @brief DataManager::addTaxonToORFs, added the Taxon to the ORFs by looking up the functional
 * and taxonomic table for the sample
 * @param sampleName, the name of the sample
 * @param fileName, the file name where the taxonomic mapping is specified.
 */

void DataManager::addTaxonToORFs(const QString &sampleName, const QString &fileName) {

    QFile inputFile(fileName);
    QString orfname, taxon;

    QStringList colValues;

    Taxons *taxons = Taxons::getTaxons();
    if( taxons==0) return;
    ORF *temporf;
    QRegExp comment("#[^\"\\n\\r]*");
    QChar delim = QChar('\t');

    unsigned int col1 = 0, col2 = 8, numCols;

    qDebug() << "Need to remove the redundancy" << sampleName << fileName;
    if (inputFile.open(QIODevice::ReadOnly)) {
        QHash<QString, ORF *> orfHash;
        foreach(ORF *orf, *(this->ORFList->value(sampleName)) ) {
            orfHash[orf->name] = orf;
        }

        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
             QString line = in.readLine().trimmed();
             if (comment.exactMatch(line)) continue;

             colValues = line.split(QRegExp(delim));
             numCols = colValues.length();
             if(numCols <= col2 ) continue;

             orfname = Utilities::getShortORFId(colValues[col1]);
             taxon = colValues[col2];


             if(!orfHash.contains(orfname) ) continue;

             temporf = orfHash[orfname];
             if( taxons->hasTaxon(taxon) )
                 temporf->attributes[TAXON] = taxons->getTaxon(taxon);
             else
                 temporf->attributes[TAXON] = taxons->getTaxon("root");

        }
        inputFile.close();
    }
}





/**
 * @brief DataManager::addNewAnnotationToORFs, adds the METACYC annotations to the
 * orfs that are already added and creates new one if they are not already added
 * @param sampleName
 * @param fileName
 */
void DataManager::addNewAnnotationToORFs(QString sampleName, QString fileName) {


    QFile inputFile(fileName);
    ORF *temporf;
    QString name, alias;


    if (inputFile.open(QIODevice::ReadOnly)) {
        QHash<QString, ORF *> orfHash;
        foreach(ORF *orf, *(this->ORFList->value(sampleName)) ) {
            orfHash[orf->name] = orf;
        }

        QTextStream in(&inputFile);
        QString orfName;
        while ( !in.atEnd() )  {
            QStringList line = in.readLine().remove(QRegExp("[\\n]")).split(QRegExp("[\\t]"));

            if(line.size() < 2) continue;
            name = line[0];
            alias = line[1];

            line.removeFirst();
            line.removeFirst();
            foreach(QString _orfName, line) {

               orfName = Utilities::getShortORFId(_orfName);

               if( orfHash.contains(orfName)) {
                   temporf = orfHash[orfName];
               }
               else {
                  temporf = new ORF;
                  this->ORFList->value(sampleName)->append(temporf);
               }

               temporf->name = orfName;

               if( this->attributes[METACYC].contains(name)) {
                   temporf->attributes[METACYC] = this->attributes[METACYC][name];
               }
               else {
                   temporf->attributes[METACYC]= new ATTRIBUTE;
                   temporf->attributes[METACYC]->name = name;
                   temporf->attributes[METACYC]->alias = alias;
                   this->attributes[METACYC][name] = temporf->attributes[METACYC];
               }
            }
        }
        inputFile.close();
    }
    this->attributes[METACYC].clear();
}




/** This function added the RPKM values to the orfs in the sample
 *\param sampleName, name of the sample
 *\param fileName, name of the file name for the RPKM values
 *
 **/
void DataManager::addRPKMToORFs(QString sampleName, QString fileName) {

    QFile inputFile(fileName);
    ORF *temporf;
    QString orfName;

    bool ok;
    if (inputFile.open(QIODevice::ReadOnly)) {
        QHash<QString, ORF *> orfHash;
        foreach(ORF *orf, *(this->ORFList->value(sampleName)) ) {
            orfHash[orf->name] = orf;
        }

        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QStringList line = in.readLine().remove(QRegExp("[\\n]")).split(QRegExp("[\\t]"));

            if(line.size() < 2) continue;
            orfName = Utilities::getShortORFId(line[0]);

            if( !orfHash.contains(orfName))  continue;
            temporf = orfHash[orfName];

            line[1].toFloat(&ok);
            if (ok) temporf->rpkm = line[1].toFloat();
        }
        inputFile.close();
    }
}


/**
 * @brief DataManager::createSubConnector, it creates a new connector from the old connector
 * for the given type of attrbute attrType
 * @param htree, the functional tree
 * @param hnode, the root node
 * @param connector, the cold connector
 * @param attrType, the attribute to follow, e.g., KEGG, COG, SEED, METACYC
 * @return the new connector.
 */
Connector *DataManager::createSubConnector(HTree *htree, HNODE *hnode, Connector* connector, ATTRTYPE attrType) {
      Connector *newConnector = new Connector;
      newConnector->setAttrType(attrType);

      if( connector->getAttrType() == newConnector->getAttrType())
         htree->copyDataToSubConnector(hnode, connector, newConnector);
      else {
          QHash<ORF *, bool> orfHash;
          foreach(ATTRIBUTE *attrib, connector->connected.keys()) {
              foreach(ORF *orf, connector->connected[attrib]) {
                  orfHash[orf]=true;
              }
          }

          DataManager *datamanager = DataManager::getDataManager();

          HTree* targetTree= datamanager->getHTree(attrType) ;
          htree->copyDataToSubConnector(targetTree->root, newConnector, targetTree, orfHash);
      }
      return newConnector;
}

/**
 * @brief DataManager::getORFList, gets the list of pointers to all the orfs in a
 *  sample
 * @param sampleName
 * @return
 */
QList<ORF *> * DataManager::getORFList(QString sampleName) {
    if( sampleName != QString("temp") && this->ORFList->contains(sampleName)  ) {
        return this->ORFList->value(sampleName);
    }
    return 0;
}


/**
 * @brief DataManager::getORFList, gets the list of pointers to all the orfs in a
 *  sample, in a contig
 * @param sampleName
 * @return
 */
QList<ORF *> DataManager::getORFList(QString sampleName, QString contig) {

    if( this->contigHash.contains(sampleName) && this->contigHash[sampleName].contains(contig)) {
        return this->contigHash[sampleName][contig]->orfList;
    }

    return  QList<ORF *>();
}


/**
 * @brief DataManager::getORFByNames, get the ORF * by the ORF names
 * @param sampleName, name of the sample
 * @param orfNames, QHash that has the orfNames
 * @return
 */
QList<ORF *> DataManager::getORFByNames(const QString &sampleName, const  QHash<QString, bool>  &orfNames) {

    QList<ORF *> orfList;

    if( !ORFList->contains(sampleName) )   return orfList;

    foreach(ORF * orf, *(ORFList->value(sampleName))) {
        if(orfNames.contains(orf->name)) orfList.append(orf);
    }

    return orfList;
 }



/** deletes the connector for a sample
 *\param sampleName, sample name whose connectors is deleted
 *
**/

void DataManager::deleteConnector(QString sampleName) {
    foreach(ATTRTYPE attrType, this->connectors[sampleName].keys()) {
        delete this->connectors[sampleName][attrType];
        this->connectors[sampleName][attrType] = NULL;
    }

}

/**
 * @brief DataManager::deleteAllConnectors, deletes all the connectors associated with the
 * htable for one sample at a time
 */
void DataManager::deleteAllConnectors() {
    foreach(QString sampleName, this->connectors.keys()) {
        this->deleteConnector(sampleName);
    }
    this->connectors.clear();
}


/**
 * @brief DataManager::createConnector, creates the list of subconnectors for a
 * sample  by looking up the list of ORFs, based on a functional tree of attribute;
 * this table is cached if it does not exist already
 * @param sampleName, the name of the sample
 * @param htree, the functional tree
 * @param attrType, the attribute, e.g., KEGG, COG, SEED
 * @param orfList list of pointers to ORFs
 * @return
 */
Connector *DataManager::createConnector(QString sampleName, HTree *htree, ATTRTYPE attrType, QList<ORF *> *orfList ) {
    if( this->connectors.contains(sampleName) && this->connectors[sampleName].contains(attrType)) {
     //   qDebug() << sampleName <<" retrieving  connector " << this->connectors[sampleName][attrType]->getORFList().size();

        return this->connectors[sampleName][attrType];
    }


    Connector *connector = this->_createConnector(sampleName, htree, attrType, orfList);

    return connector;
}


/**
 * @brief DataManager::_createConnector, creates connector from scratch
 * @param sampleName
 * @param htree
 * @param attrType
 * @param orfList
 * @return
 */
Connector *DataManager::_createConnector(QString sampleName, HTree *htree, ATTRTYPE attrType, QList<ORF *> *orfList ) {

    if(htree==0) { qDebug() <<"return zero connector "; return 0; }
    QList<ORF *>::const_iterator it;
    Connector *connector = new Connector;

    //set the appropriate attribute such as COG, KEGG, SEED, METACYC
    connector->setAttrType(attrType);
    HNODE *hnode;

    for(it = orfList->begin(); it!= orfList->end(); ++it ) {
        if( !(*it)->attributes.contains(attrType) ) continue;
      //  qDebug() << (*it)->name;
        hnode = htree->getHNODE((*it)->attributes[attrType]->name);
        if(hnode!=0) {
            connector->addToList(hnode->attribute, *it);
       }
    }

    if( sampleName!=QString("temp") ) {
       if( !this->connectors.contains(sampleName))
          this->connectors[sampleName] = QHash<ATTRTYPE, Connector *>();
       this->connectors[sampleName][attrType] = connector;
     }


    return connector;
}









