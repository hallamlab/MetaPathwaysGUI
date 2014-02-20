#ifndef SAMPLERESOURCEMANAGER_H
#define SAMPLERESOURCEMANAGER_H
#include <QString>
#include "types.h"
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
    void setOutPutPath(QString outputDir);
    void setUseResourceFolder(bool flag);
    QString getFilePath(QString sampleName,  RESOURCE resource);
    FileIndex *getFileIndex(QString sampleName, RESOURCE resource);

private:
    SampleResourceManager();
    static SampleResourceManager* sampleResourceManager;
    static QString resourceFolderName;

    QString OUTPUTPATH;
    bool useResourceFolder;

};

#endif // SAMPLERESOURCEMANAGER_H
