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




void DataManager::createDataModel() {

    if( dataModelCreated ) return;


    RunData *rundata = RunData::getRunData();

    QString refDBFolder = rundata->getValueFromHash("REFDBS", _CONFIG);
    QString COG_categories = refDBFolder + "/functional_categories/" + "COG_categories.txt";
    HTree *htree = createHTree(COG_categories);

    htree->hashNodes(htree->root);
    this->htrees[COG] = htree;
  //  htree->printTree(htree->getRootHNODE());



    QString KEGG_categories = refDBFolder + "/functional_categories/" + "KO_classification.txt";
    htree = createHTree(KEGG_categories);
    htree->hashNodes(htree->root);
    this->htrees[KEGG] = htree;

    QString MetaCyc_hierarchy = refDBFolder + "/functional_categories/" + "metacyc_hierarchy.txt";
    htree = createHTree(MetaCyc_hierarchy);
    htree->hashNodes(htree->root);
    this->htrees[METACYC] = htree;

    QString Seed_subsystems = refDBFolder + "/functional_categories/" + "SEED_subsystems.txt";
    htree = createHTree(Seed_subsystems);
    htree->hashNodes(htree->root);
    this->htrees[SEED] = htree;

    this->setDataModelCreated(true);
 //   dataModelCreated=true;
}

HTree *DataManager::createHTree(QString refDBFileName) {
    HTree *htree = new HTree();

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


void DataManager::destroyAllHTrees() {
    foreach(ATTRTYPE attr, this->htrees.keys()) {
        destroyHTree(attr);
    }

}

void DataManager::_destroyHTree(HNODE *hnode) {
    foreach( HNODE *child, hnode->children) {
        _destroyHTree(child);
    }
    delete hnode;
}



void DataManager::destroyAllAttributes() {
    foreach( ATTRTYPE attr, this->attributes.keys() ) {
        foreach(ATTRIBUTE *attrib, this->attributes[attr].values())
            delete attrib;

        foreach(QString key, this->attributes[attr].keys())
            this->attributes[attr].remove(key);

        this->attributes.remove(attr);
    }

}

void DataManager::destroyHTree(ATTRTYPE refDB ) {
     HTree *htree = this->getHTree(refDB);
     HNODE *hnode = htree->getRootHNODE();
     _destroyHTree(hnode);
     hnode = 0;
     this->htrees.remove(refDB);

}

ORF *DataManager::_createAnORF(QStringList &attributes) {

    ORF *orf = new ORF;
    QHash<QString, CONTIG *> contigHash;

    orf->name =  Utilities::getShortORFId(attributes[0]);

    QString shortContigName =  Utilities::getShortContigId(attributes[1]);

    if(contigHash.contains(shortContigName))
        orf->contig = contigHash[shortContigName];
    else {
        CONTIG *contig = new CONTIG;
        contig->name  = shortContigName;
        contigHash[shortContigName] = contig;
        orf->contig = contig;
    }

    if(this->attributes[COG].contains(attributes[2]))
        orf->attributes[COG] = this->attributes[COG][attributes[2]];
    else {
        orf->attributes[COG] = new ATTRIBUTE;
        orf->attributes[COG]->name = attributes[2];
        this->attributes[COG][attributes[2]] = orf->attributes[COG];
    }

    if(this->attributes[KEGG].contains(attributes[3]))
        orf->attributes[KEGG] = this->attributes[KEGG][attributes[3]];
    else {
        orf->attributes[KEGG] = new ATTRIBUTE;
        orf->attributes[KEGG]->name = attributes[3];
        this->attributes[KEGG][attributes[3]] = orf->attributes[KEGG];
    }

    if(this->attributes[SEED].contains(attributes[4]))
        orf->attributes[SEED] = this->attributes[SEED][attributes[4]];
    else {
        orf->attributes[SEED] = new ATTRIBUTE;
        orf->attributes[SEED]->name = attributes[4];
        this->attributes[SEED][attributes[4]] = orf->attributes[SEED];
    }
    return orf;
}


void DataManager::createORFs(QString sampleName, QString fileName) {

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
    this->ORFList->insert(sampleName, new QList<ORF *>);

    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QStringList line = in.readLine().remove(("[\\n]")).split(QRegExp("[\\t]"));
            if(line.size() < 4) continue;
            (this->ORFList->value(sampleName))->append(this->_createAnORF(line));
        }
        inputFile.close();
    }

    this->attributes[COG].clear();
    this->attributes[KEGG].clear();
    this->attributes[SEED].clear();

 //   this->ORFsUptoDateList[sampleName] = true;
}


void DataManager::destroyAllORFs() {
    foreach( QString sampleName, this->ORFList->keys())
        this->destroyORFs(sampleName);
}

void DataManager::destroyORFs(QString sampleName) {

    if( !this->ORFList->contains(sampleName)) return;
    const QList<ORF *> *orfList = this->ORFList->value(sampleName);

    foreach(ORF *orf, *orfList) {
        delete orf;
    }

    this->ORFList->remove(sampleName);

}


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
        while ( !in.atEnd() )  {
            QStringList line = in.readLine().remove(QRegExp("[\\n]")).split(QRegExp("[\\t]"));

            if(line.size() < 2) continue;
            name = line[0];
            alias = line[1];

            line.removeFirst();
            line.removeFirst();
            foreach(QString orfName, line) {
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


// it creates a new connector from the old connector for the given type of attrbute attrType
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

QList<ORF *> * DataManager::getORFList(QString sampleName) {
    if( sampleName != QString("temp") && this->ORFList->contains(sampleName)  ) {
        return this->ORFList->value(sampleName);
    }

    qDebug() << "did not find orf list " << this->ORFList->contains(sampleName) << sampleName;
    return 0;
}

void DataManager::deleteConnector(QString sampleName) {
    foreach(ATTRTYPE attrType, this->connectors[sampleName].keys()) {
        delete this->connectors[sampleName][attrType];
        this->connectors[sampleName][attrType] = NULL;
    }

}

void DataManager::deleteAllConnectors() {
    foreach(QString sampleName, this->connectors.keys()) {
        this->deleteConnector(sampleName);
    }
    this->connectors.clear();
}


Connector *DataManager::createConnector(QString sampleName, HTree *htree, ATTRTYPE attrType, QList<ORF *> *orfList ) {
    if( this->connectors.contains(sampleName) && this->connectors[sampleName].contains(attrType)) {
     //   qDebug() << sampleName <<" retrieving  connector " << this->connectors[sampleName][attrType]->getORFList().size();

        return this->connectors[sampleName][attrType];
    }

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

  //  qDebug() << "created a new conn of size " << connector->getNumOfORFs();
    return connector;
}
