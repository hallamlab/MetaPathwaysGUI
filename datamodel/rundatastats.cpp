#include "rundatastats.h"
#include <QDebug>


RunDataStats::RunDataStats(QObject *parent) :
    QAbstractTableModel(parent)
{

}


void RunDataStats::setFileNames(const QStringList &filenames) {
    this->filenames = filenames;
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

    foreach(QString sample, this->statsData.keys()) {
        foreach(QString stat, this->statsData[sample].keys()) {
            this->statNames[stat] =true;
        }
    }

    this->dataVectorMap.clear();

    foreach(QString statName, this->statNames.keys()) {
        this->dataVectorMap[statName] = QList<QString>();
        foreach(QString sample, this->filenames) {
            if( this->statsData[sample].contains(statName))
                this->dataVectorMap[statName].append( this->statsData[sample][statName]);
            else
                this->dataVectorMap[statName].append("0");
        }
    }


}

void RunDataStats::clear() {
    this->dataVectorMap.clear();
    this->statNames.clear();
    this->statsData.clear();
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
            if( fields.length()!= 2) continue;


            pair.first= fields[0].trimmed();
            pair.second= fields[1].trimmed();
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

   //    qDebug() << dataVectorMap.size() << " , " << index.row() << ", " << index.column();
      // qDebug() << dataVectorMap.keys();
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
   return (dataVectorMap.begin()+ section).key();
}
