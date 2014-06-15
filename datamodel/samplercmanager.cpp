#include "samplercmanager.h"


SampleResourceManager* SampleResourceManager::sampleResourceManager =0;
QString SampleResourceManager::resourceFolderName = QString(".resource");

SampleResourceManager::SampleResourceManager()
{


}

SampleResourceManager* SampleResourceManager::getSampleResourceManager() {
    if( SampleResourceManager::sampleResourceManager==0)
        SampleResourceManager::sampleResourceManager = new SampleResourceManager();
    return sampleResourceManager;
}

 void SampleResourceManager::setOutPutPath(QString outputDir) {
     this->OUTPUTPATH= outputDir;
 }


 void SampleResourceManager::setUseResourceFolder(bool flag) {
     this->useResourceFolder = flag;
 }



QString SampleResourceManager::getFilePath(const QString &sampleName,  RESOURCE type) {
     QString path;

     switch(type) {
         case NUCSTATS:
            path = OUTPUTPATH + "/" + sampleName + "/run_statistics/" + sampleName + ".nuc.stats";
            break;
         case AMINOSTATS:
            path = OUTPUTPATH + "/" + sampleName + "/run_statistics/" + sampleName + ".amino.stats";
            break;
         case CONTIGLENGTH:
            path = OUTPUTPATH + "/" + sampleName + "/run_statistics/" + sampleName + ".contig.lengths.txt";
            break;
         case ORFLENGTH:
            path = OUTPUTPATH + "/" + sampleName + "/run_statistics/" + sampleName + ".orf.lengths.txt";
            break;
         case NUCFASTA:
            path = OUTPUTPATH  +  "/" + sampleName + "/preprocessed/" + sampleName + ".fasta";
            break;
         case AMINOFAA:
            path = OUTPUTPATH + "/" + sampleName + "/orf_prediction/" + sampleName + ".faa";
            break;
         case NUCFNA:
            path = OUTPUTPATH + "/" + sampleName + "/orf_prediction/" + sampleName + ".fna";
            break;
         case FUNCTIONALTABLE:
            path = OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + "functional_and_taxonomic_table.txt";
            break;
         case MEGANTREE:
            path = OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + "megan_tree.tre";
            break;
         case FUNCTIONAL_SRC1:
            path = OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + sampleName + ".1.txt";
            break;
         case ORFTABLE:
            path = OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + "ORF_annotation_table.txt";
            break;
         case ORFMETACYC:
            path =OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + sampleName.toLower() + ".metacyc.orf.annots.txt";
            break;
         case ORFRPKM:
            path =OUTPUTPATH + "/" + sampleName + "/results/rpkm/" + sampleName + ".orf_rpkm.txt";
            break;
         case MEGANLASTFILE:
            path = OUTPUTPATH  +  "/" + sampleName + "/blast_results/" + sampleName + ".refseq.LASTout";
            break;
         case MEGANBLASTFILE:
            path = OUTPUTPATH  +  "/" + sampleName + "/blast_results/" + sampleName + ".refseq.BLASTout";
            break;
         case RUNSTATS:
            path = OUTPUTPATH + "/" + sampleName + "/run_statistics/" + sampleName + ".run.stats.txt";
            break;
         case rRNATABLES:
            path =OUTPUTPATH + "/" + sampleName + "/results/rRNA/";
            break;
         case tRNATABLES:
            path =OUTPUTPATH + "/" + sampleName + "/results/tRNA/";
            break;
         case ERRORS:
            path = OUTPUTPATH + "/" + sampleName + "/"+ "errors_warnings_log.txt";
            break;
         case GLOBAL_ERRORS:
            path = OUTPUTPATH + "/" + sampleName + "/"+ "global_errors_warnings.txt";
            break;
         default:
            path = "";
            break;
     }
     return path;

 }

/**
 * @brief SampleResourceManager::extractDBName, extracts the database name from the file name
 * @param sampleName, name of the sample
 * @param TableFile, the actual text file
 * @return
 */

QString SampleResourceManager::extractDBName(const QString &sampleName, const QString &TableFile, RESOURCE type) {

    QRegExp pattern;
    switch(type) {
        case rRNATABLE:
             pattern.setPattern(sampleName + "[.](.*)[.]rRNA.stats.txt$");
             break;
         default:
             return QString();
             break;
    }



    unsigned int pos = pattern.indexIn(TableFile, 0);
    if( pos >= 0 )
        return pattern.cap(1);

    return QString();
}



/**
 * @brief SampleResourceManager::getFilePaths gets the list of filenames in a sample of certain RESOURCE type
 * @param sampleName, the sample name
 * @param type, the resource type
 * @return
 */

QStringList SampleResourceManager::getFilePaths(const QString &sampleName,  RESOURCE type) {

     QString folderpath;
     QRegExp pattern;
     switch(type) {
         case rRNATABLES:
              folderpath = this->getFilePath(sampleName, rRNATABLES);
              pattern.setPattern( sampleName + "[.].*.[.]rRNA.stats.txt$");
              break;

         case tRNATABLES:
              folderpath = this->getFilePath(sampleName, tRNATABLES);
              pattern.setPattern(sampleName + "[.]tRNA.stats.txt$");
              break;
         default:
              return QStringList();
              break;
     }

     QStringList files = Utilities::getFilesWithPattern(folderpath, pattern);
  //   qDebug() << folderpath << files;
     return files;



}
bool SampleResourceManager::createFileIndex(QString sampleName, RESOURCE resname) {
    FileIndexManager *fileindexmanager = FileIndexManager::getFileIndexManager();
    FileIndex *fileIndex =0;
    QHash<RESOURCE, QString> resNames;
    resNames[NUCFASTA] = "NUCFASTA";
    resNames[AMINOFAA] = "AMINOFAA";
    resNames[NUCFNA] = "NUCFNA";

    // this is the indexing of the sequences into the resoure folder ".resource" that resides under each
    // sample output folder
    if( this->useResourceFolder ) {
        QDir resSampleDir( this->OUTPUTPATH + "/" + sampleName + "/" + SampleResourceManager::resourceFolderName );
        // if the resource folder does not exist then create it
        if( !resSampleDir.exists() && SELECT_SAMPLE_TAG.compare(sampleName)!=0 ) {
            resSampleDir.mkpath(resSampleDir.absolutePath());
        }

        if( resname == AMINOFAA || resname == NUCFASTA || resname == NUCFNA)
        fileIndex = fileindexmanager->getFileIndex(sampleName, this->getFilePath(sampleName, resname), resname, true);

        QFileInfo resourceFile(resSampleDir.absolutePath() + "/" + resNames[resname] + ".txt");
        fileindexmanager->writeFileIndex(fileIndex, resourceFile.absoluteFilePath());

        fileindexmanager->deleteFileIndex(fileIndex);  // delete only from the memory
    }

    return true;


}

FileIndex * SampleResourceManager::getFileIndex(QString sampleName, RESOURCE resname, bool reindex ) {
     FileIndexManager *fileindexmanager = FileIndexManager::getFileIndexManager();
     FileIndex *fileIndex =0;
     QHash<RESOURCE, QString> resNames;
     resNames[NUCFASTA] = "NUCFASTA";
     resNames[AMINOFAA] = "AMINOFAA";
     resNames[NUCFNA] = "NUCFNA";


     // this is the indexing of the sequences into the resoure folder ".resource" that resides under each
     // sample output folder
     if( this->useResourceFolder ) {
         QDir resSampleDir( this->OUTPUTPATH + "/" + sampleName + "/" + SampleResourceManager::resourceFolderName );

         // if the resource folder does not exist then create it
         if( !resSampleDir.exists() && SELECT_SAMPLE_TAG.compare(sampleName)!=0 ) {
             resSampleDir.mkpath(resSampleDir.absolutePath());
         }
         QFileInfo resourceFile(resSampleDir.absolutePath() + "/" + resNames[resname] + ".txt");

         // load the index file if you are not reindexing and the resource files already exists
         if( reindex == false && resourceFile.exists() && !fileindexmanager->hasFileIndex(sampleName, resname) ) {
              fileIndex = fileindexmanager->readFileIndex(sampleName, resourceFile.absoluteFilePath(), resname);
              fileIndex->setSourceFile(this->getFilePath(sampleName, resname));
         }
         else {  // if the resource folder does not have the right index files.
             if( resname == AMINOFAA || resname == NUCFASTA || resname == NUCFNA)
                fileIndex = fileindexmanager->getFileIndex(sampleName, this->getFilePath(sampleName, resname), resname);

             if(! resourceFile.exists() )
                fileindexmanager->writeFileIndex(fileIndex, resourceFile.absoluteFilePath());
         }
     }
     else {
        if( resname == AMINOFAA || resname == NUCFASTA || resname == NUCFNA)
           fileIndex = fileindexmanager->getFileIndex(sampleName, this->getFilePath(sampleName, resname), resname);
     }
     return fileIndex;
}


