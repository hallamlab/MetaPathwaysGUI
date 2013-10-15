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



bool FileIndexManager::writeFileIndex(FileIndex *fileIndex, QString filePath) {
    return fileIndex->writeFileIndex(filePath, FASTA);
}

FileIndex *FileIndexManager::readFileIndex(QString sampleName, QString filePath, RESOURCE type) {
    FileIndex *iFile = new FileIndex();
    iFile->loadFileIndex(filePath, FASTA);
    if( !indexedFiles.contains(sampleName))
        indexedFiles[sampleName] = QHash<RESOURCE, FileIndex *>();

    indexedFiles[sampleName][type] = iFile;
    return iFile;
}

FileIndex *FileIndexManager::getFileIndex(QString sampleName, QString fileName, RESOURCE type) {

    if( indexedFiles.contains(sampleName) && indexedFiles[sampleName].contains(type) ) {
        return indexedFiles[sampleName][type];
    }

    FileIndex *iFile = new FileIndex(fileName, FASTA);
    if( !indexedFiles.contains(sampleName))
        indexedFiles[sampleName] = QHash<RESOURCE, FileIndex *>();

    indexedFiles[sampleName][type] = iFile;

    return iFile;
}


bool FileIndexManager::hasFileIndex(QString sampleName, RESOURCE type) {

    if(indexedFiles.contains(sampleName) && indexedFiles[sampleName].contains(type) ) {
        return true;
    }


    return false;
}

