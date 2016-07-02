#include "rundatastats.h"
#include <QDebug>


RunDataStats::RunDataStats(QObject *parent) :
    QAbstractTableModel(parent)
{
     this->initializeTags();
}


void RunDataStats::setFileNames(const QStringList &filenames) {
    this->filenames = filenames;
}


void RunDataStats::setCategories(const QStringList &categories) {
    this->categories = categories;
}

bool compareAttributes(QPair<QString, unsigned int> &pair1, QPair<QString, unsigned int> &pair2 ) {
    if(pair1.second < pair2.second) return true;
    return false;
}



void RunDataStats::initializeTags() {
    this->replace[FULL_REPL][QString("1000")] = QString("Number of sequences in input file BEFORE QC (nucleotide)");
    this->replace[FULL_REPL][QString("1001")] = QString(" min length (bps)");
    this->replace[FULL_REPL][QString("1002")] = QString(" avg length (bps)");
    this->replace[FULL_REPL][QString("1003")] = QString(" max length (bps)");
    this->replace[FULL_REPL][QString("1004")] = QString(" total base pairs (bps)");


    this->replace[FULL_REPL][QString("1005")] = QString("Number of sequences AFTER QC (nucleotide)");
    this->replace[FULL_REPL][QString("1006")] = QString(" min length (bps)");
    this->replace[FULL_REPL][QString("1007")] = QString(" avg length (bps)");
    this->replace[FULL_REPL][QString("1008")] = QString(" max length (bps)");
    this->replace[FULL_REPL][QString("1009")] = QString(" total base pairs (bps)");


    this->replace[FULL_REPL][QString("2000")] = QString("Number of translated ORFs BEFORE QC (amino)");
    this->replace[FULL_REPL][QString("2001")] = QString(" min length (bps)");
    this->replace[FULL_REPL][QString("2002")] = QString(" avg length (bps)");
    this->replace[FULL_REPL][QString("2003")] = QString(" max length (bps)");
    this->replace[FULL_REPL][QString("2004")] = QString(" total base pairs (bps)");


    this->replace[FULL_REPL][QString("2005")] = QString("Number of translated ORFs AFTER QC (amino)");
    this->replace[FULL_REPL][QString("2006")] = QString(" min length (bps)");
    this->replace[FULL_REPL][QString("2007")] = QString(" avg length (bps)");
    this->replace[FULL_REPL][QString("2008")] = QString(" max length (bps)");
    this->replace[FULL_REPL][QString("2009")] = QString(" total base pairs (bps)");



}


/**
 * @brief RunDataStats::readStatFiles, this function reads the stats files for the runs
 * @return
 */
bool RunDataStats::readStatFiles() {
    RunData *rundata = RunData::getRunData();
    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();

    // look into the output folders
    samplercmgr->setOutPutFolders(rundata->getSampleFolderMap());

    QString runstatsFile;
    //access individual files
    foreach(QString filename, filenames) {
       runstatsFile = samplercmgr->getFilePath(filename, RUNSTATS);
       this->readDataFromFile(runstatsFile, filename);
    }

    QList< QPair<QString, unsigned int> > pairs;

    QPair<QString, unsigned int> pair;

    /*
    // correct code to tag maps for 6005
    if( this->codeToTagLine.contains(QString("6005"))) {
        this->codeToTagLine[QString("6999")] =  this->codeToTagLine[QString("6005")];
        this->codeToTagLine.remove(QString("6005"));
    }

    //also correct it for every sample
    foreach( QString sample,this->filenames) {
        if( this->statsData[sample].contains(QString("6005"))) {
            this->statsData[sample][QString("6999")] =  this->statsData[sample][QString("6005")];
            this->statsData[sample].remove(QString("6005"));
        }
    }
*/

    // this sorts the ranks of the attributes by key/code
    foreach( QString attribute, this->codeToTagLine.keys()) {
        pair.first = attribute;
        bool ok;
        attribute.toInt(&ok);
        pair.second =  ok ?  attribute.toInt(&ok) : 0;
        pairs.append(pair);

    //   qDebug() << "  +++ " << pair.first << "   " << pair.second;
    }


    // now sort them
    qSort(pairs.begin(), pairs.end(), compareAttributes);


    this->dataVectorMap.clear();
    typedef QPair<QString, unsigned int> RankPair;

    // now add the partial label related data in to the statsData
    foreach(QString sample, this->filenames)
        foreach(QString code, this->partialstatsData[sample].keys())
            this->statsData[sample][code] = this->partialstatsData[sample][code];


    int count = 0;
    foreach( RankPair pair, pairs ) { 
       // replace with the new taglines
        if( this->replace[FULL_REPL].contains(pair.first) ) {
            this->codeToTagLine[pair.first]=  this->replace[FULL_REPL][pair.first];
        }
        this->orderCodes.append(pair.first);

        this->dataVectorMap[pair.first] = QList<QString>();

        foreach(QString sample, this->filenames) {
            if( this->statsData[sample].contains(pair.first))
                this->dataVectorMap[pair.first].append( this->statsData[sample][pair.first]);
            else
                this->dataVectorMap[pair.first].append("0");
        }
        count++;
    }
    return true;
    /*
    foreach(QString code, this->orderCodes) {
        qDebug() << code << "   " << this->codeToTagLine[code];
    }*/
}

void RunDataStats::clear() {
    this->dataVectorMap.clear();
    this->statsData.clear();
    this->codeToTagLine.clear();
    this->orderCodes.clear();
    this->partialstatsData.clear();
    this->partialOrder.clear();

}

/**
 * @brief RunDataStats::insertIntoPartial, this inserts a new field into the partial Order to be
 * able to order the labels globally consistentaly across all samples
 * @param field
 * @param baserange
 */

void RunDataStats::insertIntoPartial( QString field, unsigned int baserange) {
    if( this->partialOrder.isEmpty() )
        this->partialOrder[field] = baserange;
    else { // not empty
        if(!this->partialOrder.contains(field))
           this->partialOrder[field] = baserange + this->partialOrder.size();
    }

}

/**
 * @brief RunDataStats::getParialCode, return the code from the partialOrder
 * @param field
 * @return
 */
QString RunDataStats::getPartialCode(QString field) {
    if( !this->partialOrder.contains(field)) return QString("");

    return  QString::number(this->partialOrder[field]);


}

/**
 * @brief RunDataStats::readDataFromFile, this function reads one stats file
 * @param filename, name of the stats file
 * @param sample, name of the sample
 * @return
 */
bool RunDataStats::readDataFromFile(const QString &filename, const QString &sample) {
    QFileInfo fileinfo(filename);

    this->statsData[sample] = QHash<QString, QString>();
    this->partialstatsData[sample] = QHash<QString, QString>();

    if (!fileinfo.exists()) return false;

    QFile inputFile(filename);

    unsigned int codeValue;
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();
            QStringList fields = line.split(QChar('\t'));
            if( fields.length()!= 3) continue;

            bool ok;
            fields[0].trimmed().toInt(&ok);
            codeValue = ok ? fields[0].toInt(&ok) : 0;

            if( codeValue >= 5000 && codeValue <= 8000) {


                if(codeValue >= 5000 && codeValue < 5500 ) {
                    fields[1] = fields[1].replace(QString("Total Protein Annotations"), QString("Total number of hits in"), Qt::CaseSensitive);
                    this->insertIntoPartial(fields[1], 5000);
                }

                else if(codeValue >= 5500 && codeValue < 6000 ) {
                    QString derived = fields[1];

                    derived.replace(QString("Number of ORFs with hits in"), QString("% of ORFs with hits in"), Qt::CaseSensitive);

                    fields[1].replace(QString("Number of ORFs with hits in"), QString("Number of ORFs with hits in"), Qt::CaseSensitive);
                    this->insertIntoPartial(fields[1], 5500);


                    this->insertIntoPartial(derived, 5800);
                    QString code = this->getPartialCode(derived);
                    this->partialstatsData[sample][code] = fields[2].trimmed();
                    this->codeToTagLine[code] = derived;

                }

                else if(codeValue >= 7000 && codeValue < 8000 ) {
                    fields[1] = fields[1].replace(QString("Taxonomic hits in"), QString("rRNA hits meeting user defined thresholds from"), Qt::CaseSensitive);
                    this->insertIntoPartial(fields[1], 7000);
                }

                else if(codeValue >= 6000 && codeValue < 7000 ) {

                    if( fields[1].compare("Total Protein Annotations")==0) {
                        this->partialOrder[fields[1]] = 6999;
                    }
                    else {
                        fields[1] = fields[1].replace(QString("Protein Annotations from"), QString("Annotations meeting user defined thresholds from"), Qt::CaseSensitive);
                        this->insertIntoPartial(fields[1], 6000);
                    }
                }

                QString code = this->getPartialCode(fields[1]);

             //   qDebug() << code << "  " << fields[1];
                if(code.isEmpty()) continue;

                this->partialstatsData[sample][code] = fields[2].trimmed();
                this->codeToTagLine[code] = fields[1].trimmed();

                //do not process any more
                continue;
            }


            this->codeToTagLine[fields[0].trimmed()] = fields[1].trimmed();
            this->statsData[sample][fields[0].trimmed()] = fields[2].trimmed();
        }
        inputFile.close();


        foreach( QString code, this->partialstatsData[sample].keys()) {
            unsigned int codeValue;
            bool ok;
            code.trimmed().toInt(&ok);
            codeValue = ok ? code.toInt(&ok) : 0;
            if(codeValue>= 5800 && codeValue < 5850) {

                QString totORF = this->statsData[sample].contains("2005") ? this->statsData[sample][QString("2005")] : QString("0");

                totORF.trimmed().toInt(&ok);
                unsigned int totnumORFs = ok ? totORF.toInt(&ok) : 0;

                this->partialstatsData[sample][code].toInt(&ok);
                unsigned int annotCount = ok ? this->partialstatsData[sample][code].toInt(&ok) : 0;

                if( totnumORFs ==0 )
                    this->partialstatsData[sample][code] = QString("0");
                else {
                    float percent = ((float)annotCount/(float)totnumORFs*100);

                    this->partialstatsData[sample][code] =  QString::number(percent, '%4.2f', 2);
                }
            }
        }


    }

    return true;
}

/**
 * @brief RunDataStats::rowCount, gets the number of rows in the data
 * @return
 */
int RunDataStats::rowCount()
{
   return dataVectorMap.size();
}

/**
 * @brief RunDataStats::columnCount, gets the number of columsn in the data/table
 * @return
 */
int RunDataStats::columnCount()
{
   return filenames.count();
}


/**
 * @brief RunDataStats::rowCount, gets the number of rows in the data
 * @return
 */
int RunDataStats::rowCount(const QModelIndex & /* parent */) const
{
   return dataVectorMap.size();
}

/**
 * @brief RunDataStats::columnCount, gets the number of columsn in the data/table
 * @return
 */
int RunDataStats::columnCount(const QModelIndex & /* parent */) const
{
   return filenames.count();
}

/**
 * @brief RunDataStats::data, this function deals with the data retrieval
 * @param index
 * @param role
 * @return
 */

QVariant RunDataStats::data(const QModelIndex &index, int role) const
{
   if (!index.isValid()) return QVariant();

   if (role == Qt::TextAlignmentRole) {
       return int(Qt::AlignLeft | Qt::AlignVCenter);
   }
   else if (role == Qt::DisplayRole) {
       if( index.row() < dataVectorMap.size() ) {
           QString key =  this->orderCodes[index.row()];
           if( index.column() < dataVectorMap[key].size() ) {
               return dataVectorMap[key][index.column()];
           }
           else {
               return QString('0');
           }
       }
       else {
           return QString("0");
       }
   }
   return QVariant();
}


/**
 * @brief RunDataStats::headerData, this function creates the header in the tables
 * @param section
 * @param orientation
 * @param role
 * @return
 */
QVariant RunDataStats::headerData(int col, Qt::Orientation orientation, int role) const
{
   if (role != Qt::DisplayRole)
       return QVariant();

   if( orientation==Qt::Horizontal) {
       if( !this->categories.isEmpty() ) {

           return QString::number(col) + QString("\n") + categories[col] + QString("\n") + filenames[col];
       }
       return QString::number(col) + QString("\n") + QString("") + QString("\n") + filenames[col];
   }


   QString rowAttribute = this->codeToTagLine[this->orderCodes[col]];

   return rowAttribute;

}
