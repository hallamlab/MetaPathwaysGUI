#ifndef __FILEINDEXMANAGER_H
#define __FILEINDEXMANAGER_H
#include "fileindex.h"
#include "types.h"

class FileIndexManager
{
public:
    static FileIndexManager *getFileIndexManager();

    FileIndex *getFileIndex(QString sampleName, QString fileName, RESOURCE type, bool createNew =false);

    bool hasFileIndex(QString fileName, RESOURCE type);

    bool deleteFileIndex(FileIndex *fileIndex) ;
    bool writeFileIndex(FileIndex *fileindex, QString filePath);
    FileIndex* readFileIndex( QString sampleName, QString filePath,  RESOURCE type) ;

private:
    FileIndexManager();
    static FileIndexManager *indexManager;
    QHash<QString, QHash<RESOURCE, FileIndex *> > indexedFiles;
};


#endif // __FILEINDEXMANAGER_H

