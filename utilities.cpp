#include "utilities.h"
#include "iostream"
#include "qstring.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qregexp.h"
#include "qhash.h"
#include "qsplitter.h"

Utilities::Utilities()
{
}

/*
 * Create a mapping of a CONFIG_KEY -> FORM_WIDGET_NAME. We cannot directly copy
 * the existing CONFIG key values to the form, because it contains invalid characters (like colon)
 * to assign to variables. So, we should create a hash to reflect an indirect mapping.
 */
QHash<QString,QString>* Utilities::createMapping(){
    QHash<QString,QString> *CONFIG_NAME_MAP = new QHash<QString,QString>();

    CONFIG_NAME_MAP->operator []("INPUT:format") = "fileInputFormat";

    CONFIG_NAME_MAP->operator []("quality_control:min_length") = "quality_controlMIN_LENGTH";
    CONFIG_NAME_MAP->operator []("quality_control:delete_replicates") = "quality_controlDELETE_REPLICATES";

    CONFIG_NAME_MAP->operator []("orf_prediction:algorithm") = "orf_predictionALGORITHM";
    CONFIG_NAME_MAP->operator []("orf_prediction:min_length") = "orf_predictionMIN_LENGTH";

    CONFIG_NAME_MAP->operator []("annotation:algorithm") = "annotationALGORITHM";
    CONFIG_NAME_MAP->operator []("annotation:min_bsr") = "annotationMIN_BSR";
    CONFIG_NAME_MAP->operator []("annotation:max_evalue") = "annotationMAX_EVALUE";
    CONFIG_NAME_MAP->operator []("annotation:min_score") = "annotationMIN_SCORE";
    CONFIG_NAME_MAP->operator []("annotation:min_length") = "annotationMIN_LENGTH";
    CONFIG_NAME_MAP->operator []("annotation:max_hits") = "annotationMAX_HITS";

    CONFIG_NAME_MAP->operator []("rRNA:refdbs") = "rrnaREFDBS";
    CONFIG_NAME_MAP->operator []("rRNA:max_evalue") = "rrnaMAX_EVALUE";
    CONFIG_NAME_MAP->operator []("rRNA:min_identity") = "rrnaMIN_IDENTITY";
    CONFIG_NAME_MAP->operator []("rRNA:min_bitscore") = "rrnaMIN_BITSCORE";

    CONFIG_NAME_MAP->operator []("grid_engine:batch_size") = "grid_engineBATCH_SIZE";
    CONFIG_NAME_MAP->operator []("grid_engine:max_concurrent_batches") = "grid_engineMAX_CONCURRENT_BATCHES";
    CONFIG_NAME_MAP->operator []("grid_engine:walltime") = "grid_engineWALLTIME";
    CONFIG_NAME_MAP->operator []("grid_engine:RAM") = "grid_engineRAM";
    CONFIG_NAME_MAP->operator []("grid_engine:user") = "grid_engineUSER";
    CONFIG_NAME_MAP->operator []("grid_engine:server") = "grid_engineSERVER";

    CONFIG_NAME_MAP->operator []("metapaths_steps:PREPROCESS_FASTA") = "metapaths_stepsPREPROCESS_FASTA";
    CONFIG_NAME_MAP->operator []("metapaths_steps:ORF_PREDICTION") = "metapaths_stepsORF_PREDICTION";
    CONFIG_NAME_MAP->operator []("metapaths_steps:GFF_TO_AMINO") = "metapaths_stepsGFF_TO_AMINO";
    CONFIG_NAME_MAP->operator []("metapaths_steps:FILTERED_FASTA") = "metapaths_stepsFILTERED_FASTA";
    CONFIG_NAME_MAP->operator []("metapaths_steps:COMPUTE_REFSCORE") = "metapaths_stepsCOMPUTE_REFSCORE";
    CONFIG_NAME_MAP->operator []("metapaths_steps:BLAST_REFDB") = "metapaths_stepsBLAST_REFDB";
    CONFIG_NAME_MAP->operator []("metapaths_steps:PARSE_BLAST") = "metapaths_stepsPARSE_BLAST";
    CONFIG_NAME_MAP->operator []("metapaths_steps:SCAN_tRNA") = "metapaths_stepsSCAN_TRNA";
    CONFIG_NAME_MAP->operator []("metapaths_steps:STATS_rRNA") = "metapaths_stepsSTATS_RRNA";
    CONFIG_NAME_MAP->operator []("metapaths_steps:SCAN_rRNA") = "metapaths_stepsSCAN_RRNA";
    CONFIG_NAME_MAP->operator []("metapaths_steps:ANNOTATE") = "metapaths_stepsANNOTATE";
    CONFIG_NAME_MAP->operator []("metapaths_steps:PATHOLOGIC_INPUT") = "metapaths_stepsPATHOLOGIC_INPUT";
    CONFIG_NAME_MAP->operator []("metapaths_steps:GENBANK_FILE") = "metapaths_stepsGENBANK_FILE";
    CONFIG_NAME_MAP->operator []("metapaths_steps:CREATE_SEQUIN_FILE") = "metapaths_stepsCREATE_SEQUIN_FILE";
    CONFIG_NAME_MAP->operator []("metapaths_steps:CREATE_REPORT_FILES") = "metapaths_stepsCREATE_REPORT_FILES";
    CONFIG_NAME_MAP->operator []("metapaths_steps:MLTREEMAP_CALCULATION") = "metapaths_stepsMLTREEMAP_CALCULATION";
    CONFIG_NAME_MAP->operator []("metapaths_steps:MLTREEMAP_IMAGEMAKER") = "metapaths_stepsMLTREEMAP_IMAGEMAKER";
    CONFIG_NAME_MAP->operator []("metapaths_steps:PATHOLOGIC") = "metapaths_stepsPATHOLOGIC";

    return CONFIG_NAME_MAP;
}


/*
 * Creates a new file with filename TEMPLATE_FILE. Copies over old lines and the new specified KEY, VALUE
 * pairing for settings.
 */

bool Utilities::writeSettingToFile(QString TEMPLATE_FILE, QString KEY, QString VALUE){
    QFile inputFile(TEMPLATE_FILE);
    QFile newFile( TEMPLATE_FILE + "_new" );

    if (inputFile.open(QIODevice::ReadWrite) && newFile.open(QIODevice::ReadWrite))
    {
       QTextStream in(&inputFile);
       QTextStream out(&newFile);

       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            QRegExp comment("#[^\"\\n\\r]*");
            QRegExp splitRegex("\\s");
            QRegExp keepRegex("\\w+");
            bool found = false;

            if (!comment.exactMatch(line)){
                //if the line doesn't begin with a comment hash
                QStringList splitList = line.split(splitRegex);
                //then, split the line up by white space
                splitList = splitList.filter(keepRegex);
                //filter the line into a list, keeping only non-whitespace characters

                if (splitList.count() > 0){
                    //if the list has a key and value for us to use
                    if (splitList.at(0) == KEY ){
                        splitList.replace(1,VALUE);
                        out << splitList.at(0) << " " << splitList.at(1) << "\n";
                        found = true;
                    }
                }
            }
            if (!found) out << line << "\n";
       }
       inputFile.close();
       inputFile.remove(TEMPLATE_FILE);
       //delete old file

       newFile.rename(TEMPLATE_FILE + "_new", TEMPLATE_FILE);
       newFile.close();
       //set new file with the proper name and close
       return true;
    }
    return false;
}

/*
 * For parsing the template config or parameter file.
 * Returns a hash of String to Strings, denoting the settings key, value pairs.
 */
QHash<QString,QString>* Utilities::parseFile(QString TEMPLATE_FILE){
    QFile inputFile(TEMPLATE_FILE);
    QHash<QString, QString> *configs = new QHash<QString,QString>();

    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            QRegExp comment("#[^\"\\n\\r]*");
            QRegExp splitRegex("\\s");
            QRegExp keepRegex("\\w+");
            if (!comment.exactMatch(line)){
                //if the line doesn't begin with a comment hash
                QStringList splitList = line.split(splitRegex);
                //then, split the line up by white space
                splitList = splitList.filter(keepRegex);
                //filter the line into a list, keeping only non-whitespace characters

                QString key;
                QString value;

                if (!splitList.isEmpty()){

                    //if the list has a key and value for us to use
                    key = splitList.at(0);
                    key.replace("'","");

                    if (splitList.size()>1){
                        value = splitList.at(1);
                        value.replace("'","");
                    }
                    else value = "";
                    configs->insert(key,value);
                }
            }
       }
    }
    inputFile.close();
    return configs;
}

