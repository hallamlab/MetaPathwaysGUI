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



QString SampleResourceManager::getFilePath(QString sampleName,  RESOURCE type) {
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
            path =OUTPUTPATH + "/" + sampleName + "/results/annotation_table/" + "ORF_annotation_metacyc_table.txt";
            break;
         default:
            path = "";
            break;
     }
     return path;

 }


FileIndex * SampleResourceManager::getFileIndex(QString sampleName, RESOURCE resname ) {
     FileIndexManager *fileindexmanager = FileIndexManager::getFileIndexManager();
     FileIndex *fileIndex =0;
     QHash<RESOURCE, QString> resNames;
     resNames[NUCFASTA] = "NUCFASTAT";
     resNames[AMINOFAA] = "AMINOFAA";

     qDebug() << " output file  " << this->OUTPUTPATH;
     if( this->useResourceFolder ) {

         QDir resSampleDir( this->OUTPUTPATH + "/" + SampleResourceManager::resourceFolderName + "/" + sampleName );

         if( !resSampleDir.exists()) {
             resSampleDir.mkpath(resSampleDir.absolutePath());
         }
         QFileInfo resourceFile(resSampleDir.absolutePath() + "/" + resNames[resname] + ".txt");

         if( resourceFile.exists() && !fileindexmanager->hasFileIndex(sampleName, resname) ) {
              fileIndex = fileindexmanager->readFileIndex(sampleName, resourceFile.absoluteFilePath(), resname);
              fileIndex->setSourceFile(this->getFilePath(sampleName, resname));
         }
         else {
             if( resname == AMINOFAA || resname == NUCFASTA)
                fileIndex = fileindexmanager->getFileIndex(sampleName, this->getFilePath(sampleName, resname), resname);

             if(! resourceFile.exists() )
                fileindexmanager->writeFileIndex(fileIndex, resourceFile.absoluteFilePath());
         }
     }
     else {
        if( resname == AMINOFAA || resname == NUCFASTA)
           fileIndex = fileindexmanager->getFileIndex(sampleName, this->getFilePath(sampleName, resname), resname);
     }
     return fileIndex;
}


