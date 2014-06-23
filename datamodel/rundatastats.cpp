#include "rundatastats.h"
#include <QDebug>


RunDataStats::RunDataStats(QObject *parent) :
    QAbstractTableModel(parent)
{

}


void RunDataStats::setFileNames(const QStringList &filenames) {
    this->filenames = filenames;
}

bool compareAttributes(QPair<QString, unsigned int> &pair1, QPair<QString, unsigned int> &pair2 ) {
    if(pair1.second > pair2.second) return true;
    return false;
}

bool RunDataStats::readStatFiles() {
    RunData *rundata = RunData::getRunData();
    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();

    QString OUTPUTPATH = rundata->getParams()["folderOutput"];
    samplercmgr->setOutPutPath(OUTPUTPATH);

    QString runstatsFile;
    foreach(QString filename, filenames) {
       runstatsFile = samplercmgr->getFilePath(filename, RUNSTATS);
       this->readDataFromFile(runstatsFile, filename);
    }


    QList< QPair<QString, unsigned int> > pairs;

    QPair<QString, unsigned int> pair;


    foreach( QString attribute, this->ranking.keys()) {
        pair.first = attribute;
        pair.second = this->ranking[attribute];
        pairs.append(pair);
    }

    qSort(pairs.begin(), pairs.end(), compareAttributes);


    foreach(QString sample, this->statsData.keys()) {
        foreach(QString stat, this->statsData[sample].keys()) {
            this->statNames[stat] =true;
        }
    }

    this->dataVectorMap.clear();

    typedef QPair<QString, unsigned int> RankPair;

    foreach( RankPair pair, pairs ) {
     //   qDebug() << pair.second;
     //   if( !statNames.contains(pair.first)) continue;
        this->dataVectorMap[pair.first] = QList<QString>();
        foreach(QString sample, this->filenames) {
            if( this->statsData[sample].contains(pair.first))
                this->dataVectorMap[pair.first].append( this->statsData[sample][pair.first]);
            else
                this->dataVectorMap[pair.first].append("0");
        }
    }


}

void RunDataStats::clear() {
    this->dataVectorMap.clear();
    this->statNames.clear();
    this->statsData.clear();
    this->ranking.clear();
}

bool RunDataStats::readDataFromFile(const QString &filename, const QString &sample) {
    QFileInfo fileinfo(filename);

    this->statsData[sample] = QHash<QString, QString>();

    if (!fileinfo.exists())  {
        return false;
    }

    QFile inputFile(filename);
    QList<QPair<QString, QString > > cols;
    QPair<QString, QString> pair;

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();
            QStringList fields = line.split(QChar('\t'));
            if( fields.length()!= 3) continue;

            bool ok;
            fields[0].trimmed().toInt(&ok);
            if( ok )
               this->ranking[fields[0].trimmed() + " " + fields[1]] = fields[0].trimmed().toInt(&ok);
            else
               this->ranking[fields[0].trimmed() + " " +fields[1]] = 0;

            pair.first= fields[0].trimmed() + " " +  fields[1].trimmed();
            pair.second= fields[2].trimmed();
            cols.append(pair);
        }
        inputFile.close();


        for( QList<QPair<QString, QString> >::iterator it= cols.begin(); it!= cols.end(); it++) {
            this->statsData[sample][it->first] =  it->second;
        }
    }

    return true;
}



int RunDataStats::rowCount(const QModelIndex & /* parent */) const
{
   return dataVectorMap.size();
}
int RunDataStats::columnCount(const QModelIndex & /* parent */) const
{
   return filenames.count();
}



QVariant RunDataStats::data(const QModelIndex &index, int role) const
{
   int count;
   if (!index.isValid())
       return QVariant();

   if (role == Qt::TextAlignmentRole) {
       return int(Qt::AlignRight | Qt::AlignVCenter);
   }
   else if (role == Qt::DisplayRole) {

       if( index.row() < dataVectorMap.size() ) {
           QString key =  (dataVectorMap.begin()+ index.row()).key();
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


QVariant RunDataStats::headerData(int section, Qt::Orientation orientation, int role) const
{
   if (role != Qt::DisplayRole)
       return QVariant();

   if( orientation==Qt::Horizontal) return filenames[section];
   QString rowAttribute = (dataVectorMap.begin()+ section).key();

   QRegExp re("^\\d+");

   return rowAttribute.remove(re);

  // return (dataVectorMap.begin()+ section).key();
}
