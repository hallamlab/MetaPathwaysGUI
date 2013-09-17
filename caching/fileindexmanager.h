#ifndef __FILEINDEXMANAGER_H
#define __FILEINDEXMANAGER_H
#include "fileindex.h"


class FileIndexManager
{
public:
    static FileIndexManager *getFileIndexManager();

    FileIndex *getFileIndex(QString fileName, enum SOURCETYPE type);

private:
    FileIndexManager();
    static FileIndexManager *indexManager;
    QHash<QString, FileIndex *> indexedFiles;
};


#endif // __FILEINDEXMANAGER_H

