#include "datamanager.h"


DataManager *DataManager::datamanager = 0;

DataManager *DataManager::getDataManager() {
    if( datamanager==0)
        datamanager = new DataManager;
    return datamanager;

}

DataManager::DataManager()
{
}


HTree *DataManager::getHTree(ATTRTYPE atrType) {

    if(htrees.contains(atrType))
        return htrees[atrType];
    else
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

Connector *DataManager::getConnector(QString source, QString sink, QString sampleName) {




}



CATEGORYNODE DataManager::createCategoryNode(QString line) {
    QRegExp d1("^[\\t]");
        QRegExp d2("^[\\t][\\t]");
            QRegExp d3("^[\\t][\\t][\\t]");
                QRegExp d4("^[\\t][\\t][\\t][\\t]");
                    QRegExp d5("^[\\t][\\t][\\t][\\t][\\t]");
                        QRegExp d6("^[\\t][\\t][\\t][\\t][\\t][\\t]");
                            QRegExp d7("^[\\t][\\t][\\t][\\t][\\t][\\t][\\t]");

    QString name;
    unsigned int depth;

    if( line.indexOf(d5) > -1 ) {
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

    QSettings settings("HallamLab", "MetaPathways");
    QString refDBFolder = settings.value("REFDBS").toString();
    QString COG_categories = refDBFolder + "/functional_categories/" + "COG_categories.txt";
    HTree *htree = createHTree(COG_categories);
    htree->hashNodes(htree->root);
    this->htrees[COG] = htree;

    QString KEGG_categories = refDBFolder + "/functional_categories/" + "KO_classification.txt";
    htree = createHTree(KEGG_categories);
    htree->hashNodes(htree->root);
    this->htrees[KEGG] = htree;


    //htree->printTree(htree->root);

}

HTree *DataManager::createHTree(QString refDB) {


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

    QFile inputFile(refDB);
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

ORF *DataManager::_createAnORF(QStringList &attributes) {
    ORF *orf = new ORF;
    orf->name = attributes[0];

    if(this->attributes[COG].contains(attributes[2]))
        orf->attributes[COG] = this->attributes[COG][attributes[2]];
    else {
        orf->attributes[COG] = new ATTRIBUTE;
        orf->attributes[COG]->name = attributes[2];
        this->attributes[COG][orf->name] = orf->attributes[COG];
    }

    if(this->attributes[KEGG].contains(attributes[3]))
        orf->attributes[KEGG] = this->attributes[KEGG][attributes[3]];
    else {
        orf->attributes[KEGG] = new ATTRIBUTE;
        orf->attributes[KEGG]->name = attributes[3];
        this->attributes[KEGG][orf->name] = orf->attributes[KEGG];
    }

    return orf;
}


void DataManager::createORFs(QString sampleName, QString fileName) {
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QStringList line = in.readLine().remove(QRegExp("[\\n]")).split(QRegExp("[\\t]"));
            if(line.size() < 4) continue;
            this->ORFList[sampleName].append(this->_createAnORF(line));
        }
        inputFile.close();
    }
    this->attributes[COG].clear();
    this->attributes[KEGG].clear();
}

Connector *DataManager::createConnector(QString sampleName, HTree *htree, ATTRTYPE atrType) {
    if(htree==0) return 0;
    QList<ORF *>::const_iterator it;
    Connector *connector = new Connector;
    HNODE *hnode;

    for(it = this->ORFList[sampleName].begin(); it!= this->ORFList[sampleName].end(); ++it ) {
        hnode = htree->getHNODE((*it)->attributes[atrType]->name);
          if(hnode!=0) {
              //qDebug() << hnode->attribute->name;
              connector->addToList(hnode->attribute, *it);
          }
    }

    this->connectors[sampleName] = QHash<ATTRTYPE, Connector *>();
    this->connectors[sampleName][atrType] = connector;

    return connector;
}
