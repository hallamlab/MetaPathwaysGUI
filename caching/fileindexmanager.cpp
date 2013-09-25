#include "fileindexmanager.h"


FileIndexManager *FileIndexManager::indexManager =0;

FileIndexManager::FileIndexManager()
{
}


FileIndexManager* FileIndexManager::getFileIndexManager() {
   if( indexManager == 0 )
        indexManager = new FileIndexManager();

    return indexManager;

}


FileIndex *FileIndexManager::getFileIndex(QString fileName, enum SOURCETYPE type) {

    if( indexedFiles.contains(fileName) ) {
        return indexedFiles[fileName];
    }
    FileIndex *iFile = new FileIndex(fileName, type);
    indexedFiles[fileName] = iFile;

    return iFile;
}



