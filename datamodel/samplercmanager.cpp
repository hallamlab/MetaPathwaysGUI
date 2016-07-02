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

void SampleResourceManager::setCurrentOutputFolder(QString currentOutputDir) {
 this->CURRENTOUTPUT = currentOutputDir;
}

 void SampleResourceManager::setOutPutFolders(QHash<QString, QString> outputFolders) {
     this->OUTPUTPATH= outputFolders;
 }


 void SampleResourceManager::setUseResourceFolder(bool flag) {
     this->useResourceFolder = flag;
 }



QString SampleResourceManager::getFilePath(const QString &sampleName,  RESOURCE type) {
     QString path;
     QStringList files;
     QString sep = QDir::separator();

     switch(type) {
         case NUCSTATS:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "run_statistics" + sep + sampleName + ".nuc.stats";
            break;
         case AMINOSTATS:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "run_statistics" + sep +  sampleName + ".amino.stats";
            break;
         case CONTIGLENGTH:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "run_statistics" + sep + sampleName + ".contig.lengths.txt";
            break;
         case ORFLENGTH:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "run_statistics" + sep + sampleName + ".orf.lengths.txt";
            break;
         case NUCFASTA:
            path = OUTPUTPATH[sampleName]  + sep + sampleName + sep + "preprocessed" + sep + sampleName + ".fasta";
            break;
         case RENAMEMAP:
            path = OUTPUTPATH[sampleName]  + sep + sampleName + sep + "preprocessed" + sep + sampleName + ".mapping.txt";
            break;
         case AMINOFAA:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "orf_prediction" + sep + sampleName + ".faa";
            break;
         case NUCFNA:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "orf_prediction" + sep +  sampleName + ".fna";
            break;
         case FUNCTIONALTABLE:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep + "annotation_table" + sep + "functional_and_taxonomic_table.txt";
            break;
         case FUNCTIONALTABLELONG:
           path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep + "annotation_table" + sep +  sampleName + ".functional_and_taxonomic_table.txt";
           break;
         case MEGANTREE:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep + "annotation_table" + sep + "megan_tree.tre";
            break;
         case FUNCTIONAL_SRC1:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep + "annotation_table" + sep + sampleName + ".1.txt";
            break;
         case ORFTABLE:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep + "annotation_table" + sep +  "ORF_annotation_table.txt";
            break;  
        case ORFTABLELONG:
           path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep + "annotation_table" + sep +  sampleName + ".ORF_annotation_table.txt";
           break;
         case ORFMETACYC:
            path =OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep + "annotation_table" + sep + sampleName.toLower() + ".metacyc.orf.annots.txt";
            break;
         case ORFREDUCED:
            path =OUTPUTPATH[sampleName] + sep + sampleName + sep + "ptools"  +sep + "reduced.txt";
            break;
         case CUSTOMTABLE:
            path =OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep + "annotation_table" + sep + sampleName.toLower() + ".custom.annot.txt";
            break;
         case ORFRPKM:
            path =OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep + "rpkm" + sep  + sampleName + ".orf_rpkm.txt";
            break;
         case CLUSTERWT:
            path =OUTPUTPATH[sampleName] + sep + sampleName + sep + "orf_prediction" + sep + sampleName +  ".wt.txt";
            break;
         case MEGANLASTFILE:
            files = this->getFilePaths(sampleName, MEGANLASTFILE);
            path = (files.size() > 0 ) ?  files[0] : QString();
            break;
         case MEGANBLASTFILE:
            files = this->getFilePaths(sampleName, MEGANBLASTFILE);
            path = (files.size() > 0 ) ?  files[0] : QString();
            break;
         case BLAST_RESULTS_FOLDER:
            path = OUTPUTPATH[sampleName]  +  sep + sampleName + sep + "blast_results" + sep;
            break;
         case RUNSTATS:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep + "run_statistics" + sep  + sampleName + ".run.stats.txt";
            break;
         case rRNATABLES:
            path =OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep +"rRNA" + sep;
            break;
         case tRNATABLES:
            path =OUTPUTPATH[sampleName] + sep + sampleName + sep + "results" + sep + "tRNA" + sep;
            break;
         case ERRORS:
            path = CURRENTOUTPUT + sep  + sampleName + sep+ "errors_warnings_log.txt";
            break;
         case GLOBAL_ERRORS:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep+ "global_errors_warnings.txt";
            break;
         case STEPS_LOG:
            path = OUTPUTPATH[sampleName] + sep + sampleName + sep+ "metapathways_steps_log.txt";
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
        case rRNA:
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

         case MEGANBLASTFILE:
              folderpath = this->getFilePath(sampleName, BLAST_RESULTS_FOLDER);
              pattern.setPattern(sampleName + "[.]refseq.*[.]BLASTout$");
              pattern.setCaseSensitivity(Qt::CaseInsensitive);
              break;
         case MEGANLASTFILE:
              folderpath = this->getFilePath(sampleName, BLAST_RESULTS_FOLDER);
              pattern.setPattern(sampleName + "[.]refseq.*[.]LASTout$");
              pattern.setCaseSensitivity(Qt::CaseInsensitive);
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
    QString sep = QDir::separator();
    FileIndexManager *fileindexmanager = FileIndexManager::getFileIndexManager();
    FileIndex *fileIndex =0;
    QHash<RESOURCE, QString> resNames;
    resNames[NUCFASTA] = "NUCFASTA";
    resNames[AMINOFAA] = "AMINOFAA";
    resNames[NUCFNA] = "NUCFNA";

    // this is the indexing of the sequences into the resoure folder ".resource" that resides under each
    // sample output folder
    if( this->useResourceFolder ) {
        QDir resSampleDir( this->OUTPUTPATH[sampleName] + sep + sampleName + sep + SampleResourceManager::resourceFolderName );
        // if the resource folder does not exist then create it
        if( !resSampleDir.exists() && SELECT_SAMPLE_TAG.compare(sampleName)!=0 ) {
            resSampleDir.mkpath(resSampleDir.absolutePath());
        }

        if( resname == AMINOFAA || resname == NUCFASTA || resname == NUCFNA)
        fileIndex = fileindexmanager->getFileIndex(sampleName, this->getFilePath(sampleName, resname), resname, true);

        QFileInfo resourceFile(resSampleDir.absolutePath() + sep + resNames[resname] + ".txt");
        fileindexmanager->writeFileIndex(fileIndex, resourceFile.absoluteFilePath());

        fileindexmanager->deleteFileIndex(fileIndex);  // delete only from the memory
    }

    return true;


}

FileIndex * SampleResourceManager::getFileIndex(QString sampleName, RESOURCE resname, bool reindex ) {
     FileIndexManager *fileindexmanager = FileIndexManager::getFileIndexManager();
     QString sep = QDir::separator();
     FileIndex *fileIndex =0;
     QHash<RESOURCE, QString> resNames;
     resNames[NUCFASTA] = "NUCFASTA";
     resNames[AMINOFAA] = "AMINOFAA";
     resNames[NUCFNA] = "NUCFNA";


     // this is the indexing of the sequences into the resoure folder ".resource" that resides under each
     // sample output folder
     if( this->useResourceFolder ) {
         QDir resSampleDir( this->OUTPUTPATH[sampleName] + sep + sampleName + sep + SampleResourceManager::resourceFolderName );

         // if the resource folder does not exist then create it
         if( !resSampleDir.exists() && SELECT_SAMPLE_TAG.compare(sampleName)!=0 ) {
             resSampleDir.mkpath(resSampleDir.absolutePath());
         }
         QFileInfo resourceFile(resSampleDir.absolutePath() + sep + resNames[resname] + ".txt");

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


