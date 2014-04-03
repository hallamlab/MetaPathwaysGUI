#include "rundatastats.h"
#include <QDebug>


RunDataStats::RunDataStats(QObject *parent) :
    QAbstractTableModel(parent)
{
    numcols = 0;

}


void RunDataStats::setFileNames(const QStringList &filenames) {
    this->filenames = filenames;
}


void RunDataStats::readStatFiles() {
    RunData *rundata = RunData::getRunData();
    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();

    QString OUTPUTPATH = rundata->getParams()["folderOutput"];
    samplercmgr->setOutPutPath(OUTPUTPATH);
    QString runstatsFile;
    foreach(QString filename, filenames) {
       runstatsFile = samplercmgr->getFilePath(filename, RUNSTATS);
       this->readDataFromFile(runstatsFile);
    }



}

void RunDataStats::readDataFromFile(const QString &filename) {
    QFileInfo fileinfo(filename);
    qDebug() << filename;
    if (!fileinfo.exists()) return;

    QFile inputFile(filename);

    QList<QStringList> cols;
    QStringList pair;
    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();
            pair = line.split(QChar('\t'));
            if( pair.length()!= 2) continue;
            cols.append(pair);
        }
    }
    foreach(QStringList pair, cols) {

        if(!dataVectorMap.contains(pair[0]) ) {
            dataVectorMap[pair[0]] = QList<QString>();
            for(int i=0; i < numcols; i++) {
                dataVectorMap[pair[0]].append(QString("0"));
            }
        }

        dataVectorMap[pair[0]].append(pair[1]);

    }

    numcols++;

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

   return (dataVectorMap.begin()+ section).key();


}
