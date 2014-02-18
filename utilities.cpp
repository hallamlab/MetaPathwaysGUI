#include "utilities.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QHash>
#include <QSplitter>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <sstream>

Utilities::Utilities()
{
}




/*
 * Validate the currently loaded run parameters.
 */
bool Utilities::validateConfig(const QHash<QString, QString>* PARAMS){
    if( PARAMS ==0 ) return false;

    QString python = PARAMS->value("PYTHON_EXECUTABLE");
    QString perl = PARAMS->value("PERL_EXECUTABLE");
    QString mp = PARAMS->value("METAPATHWAYS_PATH");
    QString blastdbs = PARAMS->value("REFDBS");

    if (python.isEmpty()) return false;
    else if (perl.isEmpty()) return false;
    else if (mp.isEmpty()) return false;
    else if (blastdbs.isEmpty()) return false;
    else return true;
}

/*
 * Create a mapping of a CONFIG_KEY -> FORM_WIDGET_NAME. We cannot directly copy
 * the existing CONFIG key values to the form, because it contains invalid characters (like colon)
 * to assign to variables. So, we should create a hash to reflect an indirect mapping.
 */
QHash<QString,QString> Utilities::createMapping(){
    QHash<QString,QString> CONFIG_NAME_MAP ;

    CONFIG_NAME_MAP["fileInput"] = "fileInput";
    CONFIG_NAME_MAP["REFDBS"] = "REFDBS";

    CONFIG_NAME_MAP["INPUT:format"] = "fileInputFormat";

    CONFIG_NAME_MAP["quality_control:min_length"] = "quality_controlMIN_LENGTH";
    CONFIG_NAME_MAP["quality_control:delete_replicates"] = "quality_controlDELETE_REPLICATES";

    CONFIG_NAME_MAP["orf_prediction:algorithm"] = "orf_predictionALGORITHM";
    CONFIG_NAME_MAP["orf_prediction:min_length"] = "orf_predictionMIN_LENGTH";

    CONFIG_NAME_MAP["annotation:algorithm"] = "annotationALGORITHM";
    CONFIG_NAME_MAP["annotation:dbs"] = "annotationDBS";
    CONFIG_NAME_MAP["annotation:min_bsr"] = "annotationMIN_BSR";
    CONFIG_NAME_MAP["annotation:max_evalue"] = "annotationMAX_EVALUE";
    CONFIG_NAME_MAP["annotation:min_score"] = "annotationMIN_SCORE";
    CONFIG_NAME_MAP["annotation:min_length"] = "annotationMIN_LENGTH";
    CONFIG_NAME_MAP["annotation:max_hits"] = "annotationMAX_HITS";

    CONFIG_NAME_MAP["rRNA:refdbs"] = "rrnaREFDBS";
    CONFIG_NAME_MAP["rRNA:max_evalue"] = "rrnaMAX_EVALUE";
    CONFIG_NAME_MAP["rRNA:min_identity"] = "rrnaMIN_IDENTITY";
    CONFIG_NAME_MAP["rRNA:min_bitscore"] = "rrnaMIN_BITSCORE";

    CONFIG_NAME_MAP["grid_engine:batch_size"] = "grid_engineBATCH_SIZE";
    CONFIG_NAME_MAP["grid_engine:max_concurrent_batches"] = "grid_engineMAX_CONCURRENT_BATCHES";
    CONFIG_NAME_MAP["grid_engine:walltime"] = "grid_engineWALLTIME";
    CONFIG_NAME_MAP["grid_engine:RAM"] = "grid_engineRAM";
    CONFIG_NAME_MAP["grid_engine:user"] = "grid_engineUSER";
    CONFIG_NAME_MAP["grid_engine:server"] = "grid_engineSERVER";

    CONFIG_NAME_MAP["metapaths_steps:PREPROCESS_FASTA"] = "metapaths_stepsPREPROCESS_FASTA";
    CONFIG_NAME_MAP["metapaths_steps:ORF_PREDICTION"] = "metapaths_stepsORF_PREDICTION";
    CONFIG_NAME_MAP["metapaths_steps:GFF_TO_AMINO"] = "metapaths_stepsGFF_TO_AMINO";
    CONFIG_NAME_MAP["metapaths_steps:FILTERED_FASTA"] = "metapaths_stepsFILTERED_FASTA";
    CONFIG_NAME_MAP["metapaths_steps:COMPUTE_REFSCORE"] = "metapaths_stepsCOMPUTE_REFSCORE";
    CONFIG_NAME_MAP["metapaths_steps:BLAST_REFDB"] = "metapaths_stepsBLAST_REFDB";
    CONFIG_NAME_MAP["metapaths_steps:PARSE_BLAST"] = "metapaths_stepsPARSE_BLAST";
    CONFIG_NAME_MAP["metapaths_steps:SCAN_tRNA"] = "metapaths_stepsSCAN_TRNA";
    CONFIG_NAME_MAP["metapaths_steps:STATS_rRNA"] = "metapaths_stepsSTATS_RRNA";
    CONFIG_NAME_MAP["metapaths_steps:SCAN_rRNA"] = "metapaths_stepsSCAN_RRNA";
    CONFIG_NAME_MAP["metapaths_steps:ANNOTATE"] = "metapaths_stepsANNOTATE";
    CONFIG_NAME_MAP["metapaths_steps:PATHOLOGIC_INPUT"] = "metapaths_stepsPATHOLOGIC_INPUT";
    CONFIG_NAME_MAP["metapaths_steps:GENBANK_FILE"] = "metapaths_stepsGENBANK_FILE";
    CONFIG_NAME_MAP["metapaths_steps:CREATE_SEQUIN_FILE"] = "metapaths_stepsCREATE_SEQUIN_FILE";
    CONFIG_NAME_MAP["metapaths_steps:CREATE_REPORT_FILES"] = "metapaths_stepsCREATE_REPORT_FILES";
    CONFIG_NAME_MAP["metapaths_steps:MLTREEMAP_CALCULATION"] = "metapaths_stepsMLTREEMAP_CALCULATION";
    CONFIG_NAME_MAP["metapaths_steps:MLTREEMAP_IMAGEMAKER"] = "metapaths_stepsMLTREEMAP_IMAGEMAKER";
    CONFIG_NAME_MAP["metapaths_steps:PATHOLOGIC"] = "metapaths_stepsPATHOLOGIC";

    CONFIG_NAME_MAP["ptools_settings:taxonomic_pruning"] = "ptools_settingsTAXONOMIC_PRUNING";

    return CONFIG_NAME_MAP;
}

/*
 * Constructs labels based on the file given. Uses "$" as a delim.
 */
QList<QLabel *>* Utilities::createLabels(const QString &FILE_NAME, const QChar &DELIM){
    QList<QString>* strings = Utilities::parseResults(FILE_NAME, DELIM);
    QList<QLabel *>* labels = new QList<QLabel *>();
    foreach (QString str, *strings){
        QLabel *temp = new QLabel(str.trimmed());
        labels->append(temp);
    }
    return labels;
}



/*
 * Returns a list of strings which have the first character in each line matching DELIM.
 */
QList<QString>* Utilities::parseResults(const QString &FILE_NAME, const QChar &DELIM){
    QFile inputFile(FILE_NAME);
    QList<QString>* strings = new QList<QString>();
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
           QString line = in.readLine().trimmed();
            if (line.at(0)==DELIM){
                line.remove(0,1);
                strings->append(line);
            }
       }
    }
    inputFile.close();
    return strings;
}

/*
 * Counts the number of stages to be run.
 */
int Utilities::countRunSteps(QHash<QString,QString>* PARAMS){
    QHash<QString,QString>::iterator it;
    int redoOrRunCount = 0;
    for (it=PARAMS->begin();it!=PARAMS->end();++it){
        if (it.value().operator ==("redo") || it.value().operator ==("yes")){
            if (it.key().operator ==("metapaths_steps:BLAST_REFDB")) continue;
            redoOrRunCount++;
        }
    }
    return redoOrRunCount;
}

/*
 * Creates a new file with filename TEMPLATE_FILE. Copies over old lines and the new specified KEY, VALUE
 * pairing for settings.
 */

bool Utilities::writeSettingToFile(const QString &TEMPLATE_FILE, const QString &KEY, const QString &VALUE, const bool &CREATE, const bool &DELETE){
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

                if (splitList.size() > 0){
                    //if the list has a key and value for us to use
                    if (splitList.at(0)==KEY){
                        if (DELETE){
                            //we're not going to copy this entry over to the new configuration file
                            continue;
                        }
                        else if (splitList.size()<2){
                            //if only the key exists, the value hasn't been selected
                            //then write out a line with the key and the newly defined value
                            out << splitList.at(0) << " " << VALUE << "\n";
                        }else {
                            //the value exists in file, we're replacing it, so replace the
                            //stringlist entry with the new value
                            splitList.replace(1,VALUE);
                            out << splitList.at(0) << " " << splitList.at(1) << "\n";
                        }
                        found = true;
                    }
                }
            }
            if (!found) out << line << "\n";
       }

       if (CREATE){
           out << KEY << " " << VALUE;
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
 * Returns db names that are unique in a list.
 * 1.) Sort all files alphabetically
 * 2.) Ensure that only strings that do not exist as substrings for other strings in the list are returned.
 */


QStringList Utilities::getUniqueDBS(QStringList dbs){
    QHash<QString, bool> unique;

    for (int i=0;i<dbs.length();i++){
        unique[dbs[i]] = true;
    }
    return unique.keys();
}



/*
 * For parsing the template config or parameter file.
 * Returns a hash of String to Strings, denoting the settings key, value pairs.
 */
QHash<QString,QString> Utilities::parseFile(const QString &TEMPLATE_FILE){
    QFile inputFile(TEMPLATE_FILE);
    QHash<QString, QString> configs;

    if (inputFile.open(QIODevice::ReadOnly) && inputFile.exists())
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
                line = line.remove(QRegExp("[,\\s]*$"));
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
                    configs.insert(key,value);
                }
            }
       }
    }
    inputFile.close();
    return configs;
}

QString Utilities::extractAttribute(QString line) {

    QRegExp splitRegex("\\s");
    QRegExp keepRegex("\\w+");
    line = line.remove(QRegExp("[,\\s]*$"));
    QStringList splitList = line.split(splitRegex);
    //then, split the line up by white space
    splitList = splitList.filter(keepRegex);
    //filter the line into a list, keeping only non-whitespace characters

    QString key;
    QString value;

    if (splitList.size()>1){
        //if the list has a and value for us to use
        value = splitList.at(1);
        value.replace("'","");
    }
    else {
        value = "";
    }

    return value;
}

QString Utilities::createToolTipTable(  QList< QList<QString> > tableData ) {

    QString text = "<table border="" frame=void>";
    QList< QList< QString > >::Iterator it;

    for( it = tableData.begin(); it!= tableData.end(); ++it) {
        text.append("<tr>\n");
        unsigned int i=0;
        for(QList<QString>::const_iterator it2 = it->begin();  it2 != it->end(); ++it2) {
            text.append("<td>\n");

            if(i==0) {
                text.append( QString("<b>") +  *it2  +  QString("</b>")  +  "\n");
               i++;
            }
            else
               text.append( *it2  + "\n");

            text.append("</td>\n");
        }
        text.append("</tr>\n");
    }
    text.append("</table>");
    return text;

}



QLabel *Utilities::ShowWaitScreen( QString msg) {
    class SleeperThread : public QThread
    {
          public:
             static void msleep(unsigned long msecs) {QThread::msleep(msecs);}

    };

    QLabel *aWidget = new QLabel(0, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    aWidget->setAttribute(Qt::WA_TranslucentBackground);
    aWidget->setAlignment(Qt::AlignCenter );

    aWidget->setTextFormat(Qt::RichText);
    aWidget->setText( QString("<FONT COLOR='#ff0000' FONT SIZE = 10>") +  msg  + QString("</>"));

    aWidget->show();
    aWidget->repaint();
    return aWidget;
}


QLabel *Utilities::ShowWaitScreen(QString pngFile, QString msg) {
    class SleeperThread : public QThread
    {
          public:
             static void msleep(unsigned long msecs) {QThread::msleep(msecs);}

    };

    QPixmap aPixmap(pngFile);
    QLabel *aWidget = new QLabel(0, Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

    aWidget->setAttribute(Qt::WA_TranslucentBackground);

    aWidget->setAlignment(Qt::AlignCenter );
    aWidget->setPixmap(aPixmap);
    aWidget->show();
    aWidget->repaint();
    return aWidget;
}


QString Utilities::insertCharacterAtIntervals(QString string, QChar insertC, int n) {

    QString newString = "";
    int count = 0;
    foreach(QChar c, string) {
        newString += c;
        if(count%n==0 && count > 0) newString += insertC;
        count++;
    }
    return newString;
}

