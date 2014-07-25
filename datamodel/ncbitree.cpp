#include "ncbitree.h"


NCBITree *NCBITree::ncbitree = 0;

NCBITree::NCBITree()
{

    this->begin_pattern.setPattern("^#");
}


NCBITree *NCBITree::getNCBITree() {
    if(  NCBITree::ncbitree ==0 ) {
         NCBITree::ncbitree = new NCBITree;
         NCBITree::ncbitree->loadTree();
    }

    return NCBITree::ncbitree;
}

/**
 * @brief LCAStar::loadTree, loads the NCBI tree
 */
void NCBITree::loadTree() {

    DataManager *datamanager = DataManager::getDataManager();
    QString ncbifile = datamanager->getResourceFile(NCBITREEFILE);


    ProgressView progressbar("Reading NCBI tree file \n" +  ncbifile, 0, 0, 0);

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

    inputFile.close();
    progressbar.hide();
}



