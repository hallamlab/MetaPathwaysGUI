#ifndef SAMPLERESOURCEMANAGER_H
#define SAMPLERESOURCEMANAGER_H
#include <QString>
#include "helper/types.h"
#include "caching/fileindex.h"
#include "caching/fileindexmanager.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>


extern const QString SELECT_SAMPLE_TAG;

class SampleResourceManager
{
public:
    static SampleResourceManager *getSampleResourceManager();
    void setOutPutFolders(QHash<QString, QString>  outputFolders);
    void setUseResourceFolder(bool flag);
    QString getFilePath(const QString &sampleName,  RESOURCE resource);
    QStringList getFilePaths(const QString &sampleName,  RESOURCE resource);
    FileIndex *getFileIndex(QString sampleName, RESOURCE resource, bool reindex = false);
    bool createFileIndex(QString sampleName, RESOURCE resname);

    QString extractDBName(const QString &sampleName, const QString &TableFile, RESOURCE type);
    void setCurrentOutputFolder(QString currentOutputDir);

private:
    SampleResourceManager();
    static SampleResourceManager* sampleResourceManager;
    static QString resourceFolderName;

    QHash<QString, QString> OUTPUTPATH;
    QString CURRENTOUTPUT;
    bool useResourceFolder;

};

#endif // SAMPLERESOURCEMANAGER_H
