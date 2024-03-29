/*
 * Helper functions for the GUI.
 */

#include "utilities.h"


Utilities::Utilities()
{
}

/**
 * @brief Utilities::read_metadata_file, reads the metadata file
 * @param metadatafile
 * @param samples
 */
void Utilities::read_metadata_file(QString fileName, QHash<QString, QStringList> &samples ) {
    QFile inputFile(fileName);

    QStringList names;
    QRegExp comment("^#");
    QRegExp typePatt("EnvironmentType");
    int numTypes = 0;

    if (inputFile.open(QIODevice::ReadOnly) && inputFile.exists())
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine().trimmed();


            if( line.trimmed().isEmpty() ) continue;
            names = line.split("\t");
            if( names.size() ==0 ) continue;

            if( line.indexOf(comment) > -1 && numTypes ==0 ) {
                foreach(QString name, names) {
                    if( name.indexOf(typePatt) > -1 ) numTypes++;
                }
             }
             else {
                QString sampleName = names[0];
                samples[sampleName] = QStringList();
                names.pop_front();
                for(int i=0; i < names.size() && i < numTypes; i++) {
                    samples[sampleName].append(names[i]);
                    if( names[i].trimmed().isEmpty() or names[i].trimmed().compare(QString("-"))==0) break;
                }
             }
       }
    }

}

/**
 * @brief Utilities::readOrfContigList, reads a text file that splits them into words, to extract
 * orf and contig
 * @param fileName
 * @param list
 */
void Utilities::readOrfContigList(QString fileName,  QString sampleName, QHash<QString, bool> &list, SEQNAME type) {
    QFile inputFile(fileName);

    QStringList names;

    if (inputFile.open(QIODevice::ReadOnly) && inputFile.exists())
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine().trimmed();
            names = Utilities::getSplitNames(line);
            foreach(QString name, names) {
                if(name.isEmpty()) continue;
                list.insert(name, true);
            }
       }
    }
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
    CONFIG_NAME_MAP["orf_prediction:translation_table"] = "orf_predictionTRANSLATION_TABLE";

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

    CONFIG_NAME_MAP["metapaths_steps:PREPROCESS_INPUT"] = "metapaths_stepsPREPROCESS_INPUT";
    CONFIG_NAME_MAP["metapaths_steps:ORF_PREDICTION"] = "metapaths_stepsORF_PREDICTION";
    CONFIG_NAME_MAP["metapaths_steps:ORF_TO_AMINO"] = "metapaths_stepsORF_TO_AMINO";
    CONFIG_NAME_MAP["metapaths_steps:FILTER_AMINOS"] = "metapaths_stepsFILTER_AMINOS";
    CONFIG_NAME_MAP["metapaths_steps:COMPUTE_REFSCORES"] = "metapaths_stepsCOMPUTE_REFSCORES";
    CONFIG_NAME_MAP["metapaths_steps:FUNC_SEARCH"] = "metapaths_stepsFUNC_SEARCH";
    CONFIG_NAME_MAP["metapaths_steps:PARSE_FUNC_SEARCH"] = "metapaths_stepsPARSE_FUNC_SEARCH";
    CONFIG_NAME_MAP["metapaths_steps:SCAN_tRNA"] = "metapaths_stepsSCAN_tRNA";
    CONFIG_NAME_MAP["metapaths_steps:SCAN_rRNA"] = "metapaths_stepsSCAN_rRNA";
    CONFIG_NAME_MAP["metapaths_steps:ANNOTATE_ORFS"] = "metapaths_stepsANNOTATE_ORFS";
    CONFIG_NAME_MAP["metapaths_steps:PATHOLOGIC_INPUT"] = "metapaths_stepsPATHOLOGIC_INPUT";
    CONFIG_NAME_MAP["metapaths_steps:GENBANK_FILE"] = "metapaths_stepsGENBANK_FILE";
    CONFIG_NAME_MAP["metapaths_steps:CREATE_ANNOT_REPORTS"] = "metapaths_stepsCREATE_ANNOT_REPORTS";
    CONFIG_NAME_MAP["metapaths_steps:MLTREEMAP_CALCULATION"] = "metapaths_stepsMLTREEMAP_CALCULATION";
    CONFIG_NAME_MAP["metapaths_steps:BUILD_PGDB"] = "metapaths_stepsBUILD_PGDB";
    CONFIG_NAME_MAP["metapaths_steps:COMPUTE_RPKM"] = "metapaths_stepsCOMPUTE_RPKM";

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
 * Checks to see if a file is empty. Returns 1 if it is, else 0;
 */
bool Utilities::checkEmptyFile(QString filepath){
    QFile newFile(filepath);
    newFile.open( QIODevice::WriteOnly|QIODevice::Append );
    if (newFile.pos() == 0) {
      return true;
    } else {
      return false;
    }
    newFile.close();
}

/*
 * Creates a new file with filename TEMPLATE_FILE. Copies over old lines and the new specified KEY, VALUE
 * pairing for settings. TYPE specifies if it is a config or param file, as the first will require all values
 * to be wrapped in single quotations, and params do not.
 */

bool Utilities::writeSettingToFile(const QString &TEMPLATE_FILE, const QString TYPE, const QString &KEY, const QString &VALUE, const bool &CREATE, const bool &DELETE){
    QFile inputFile(TEMPLATE_FILE);
    QFile newFile( TEMPLATE_FILE + "_new" );

    if (inputFile.open(QIODevice::ReadOnly) && newFile.open(QIODevice::ReadWrite))
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
                    QString n_value = "";

                    if (splitList.at(0)==KEY){
                        if (DELETE){
                            //we're not going to copy this entry over to the new configuration file
                            continue;
                        }
                        else if (splitList.size()<2){
                            //if only the key exists, the value hasn't been selected
                            //then write out a line with the key and the newly defined value
                            //but for CONFIGS we gotta wrap the bad boy around with single quotes
                            if (TYPE=="CONFIG") n_value =  KEY + " " + "'" +  VALUE + "'" + "\n";
                            else n_value =  KEY + " " +  VALUE + "\n";
                        }else {
                            //the value exists in file, we're replacing it, so replace the
                            //stringlist entry with the new value
                            //but for CONFIGS we gotta wrap the bad boy around with single quotes
                            if (TYPE=="CONFIG") n_value = KEY + " " + "'" +  VALUE + "'" + "\n";
                            else n_value =  KEY + " " + VALUE + "\n";
                        }
                        out << n_value;
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
    }else{
       // msg.warning(0,"Error!\n",QString("Could not write the configuration file to " + TEMPLATE_FILE + ", do you have permissions to write there?"), QMessageBox::Ok);
        return false;
    }
    return false;
}




/**
 * @brief Utilities::getUniqueDBS Returns db names that are unique in a list.
 * 1.) Sort all files alphabetically
 * 2.) Ensure that only strings that do not exist as substrings for
 *     other strings in the list are returned.
 * @param dbs
 * @return
 */
QStringList Utilities::getUniqueDBS(QStringList dbs){
    QHash<QString, bool> unique;

    for (int i=0;i<dbs.length();i++){
        unique[dbs[i]] = true;
    }
    return unique.keys();
}


/**
 * @brief Utilities::getSplitNames splits the names of a db string, in the
 * format as in the template_params.txt file
 * @param nameString, the string
 * @return a list of strings
 */
QStringList Utilities::getSplitNames(QString nameString) {
    QRegExp delim("[\\s\\t,]");
    QStringList splits =  nameString.split(delim);
    QStringList newList;

    foreach(QString split, splits) {
        if( !split.trimmed().isEmpty() )
            newList.append(split.trimmed());
    }

    return newList;
}



/*
 * For parsing the template config or parameter file.
 * Returns a hash of String to Strings, denoting the settings key, value pairs.
 * TYPE can be of "CONFIG" or "PARAMS". Parsing behavior is different for the two, so TYPE needs to be specified always.
 */
QHash<QString,QString> Utilities::parseFile(const QString &TEMPLATE_FILE, const QString TYPE){
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
            QStringList splitList;
            if (!comment.exactMatch(line)){
                //if the line doesn't begin with a comment hash
                if(TYPE=="CONFIG"){
                    // find first occurance of a single quote, get the rest of the line from that
                    int position = line.indexOf("'",0);
                    QString wrappedValue = line.mid(position).trimmed();
                    line.remove(position,wrappedValue.length());
                    line = line.trimmed();
                }else{
                    splitList = line.split(splitRegex);
                }

                QString key;

                if (!splitList.isEmpty()){
                    //qDebug() << splitList;
                    //if the list has a key and value for us to use
                    key = splitList.at(0);

                    QString value;
                    splitList.removeFirst();

                    foreach(QString part, splitList) {
                        value += " " + part;
                    }
                    value = value.trimmed();

                    configs.insert(key,value);
                }
            }
       }
    }else{
        QMessageBox msg;
        msg.warning(0,"Error!\n",QString("Could not read the configuration file to " + TEMPLATE_FILE + ", does that file exist?"), QMessageBox::Ok);
        return QHash<QString,QString>();
    }
    inputFile.close();
    return configs;
}


QHash<QString,QString> Utilities::parseFileConfig(const QString &CONFIG_FILE){
    QFile inputFile(CONFIG_FILE);

    QHash<QString, QString> configs;

    if (inputFile.open(QIODevice::ReadOnly) && inputFile.exists())
    {
       QTextStream in(&inputFile);
       while ( !in.atEnd() )
       {
            QString line = in.readLine();
            QRegExp comment("^#");
            QRegExp splitRegex("[\\s\\t]");
            QStringList splitList;
            if (line.indexOf(comment) != -1) continue;

            splitList = line.split(splitRegex);
            if( splitList.size()< 2) continue;



            QStringList cleanKeyValue;
            foreach(QString part, splitList) {
                QString str = part.remove(QRegExp("\"")).remove(QRegExp("\'"));
                if( str.isEmpty() ) continue;
                cleanKeyValue.append(str);
            }
            if( cleanKeyValue.size()!=2) continue;
            configs.insert(cleanKeyValue[0], cleanKeyValue[1]);
       }
    }else{
        QMessageBox msg;
        msg.warning(0,"Error!\n",QString("Could not read the configuration file to " + CONFIG_FILE + ", does that file exist?"), QMessageBox::Ok);
        return QHash<QString,QString>();
    }
    inputFile.close();
    return configs;
}


QString Utilities::extractAttribute(QString line) {

    line = line.remove(QRegExp("[,\\s]*$"));
    //qDebug() << "before ea split";
    QStringList splitList = line.split("'([^']*?)'");
    //qDebug() << "inside ea " << splitList;
    //then, split the line up by white space
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

QString Utilities::getShortORFId(const QString &orfname) {
    //QRegExp orfNameRegEx("_(\\d+_\\d+)$");
    QRegExp orfNameRegEx("(\\d+_\\d+)$");

    QString _orfname = orfname;
    _orfname = _orfname.replace(QString("\""), QString());


    QString shortORFname;
    int pos  = orfNameRegEx.indexIn(_orfname);
    if( pos > -1 ) {
        shortORFname = orfNameRegEx.cap(1);
        //qDebug() <<  shortORFname << "" <<  _orfname;
    }
    else {
        return "";
    }
    return shortORFname;
}


QString Utilities::getShortContigId(const QString &contigname) {
    //QRegExp contigNameRegEx("_(\\d+)$");
    QRegExp contigNameRegEx("(\\d+)$");

    QString _contigname = contigname;
    _contigname = _contigname.replace(QString("\""), QString());

    QString shortContigname;
    int pos  = contigNameRegEx.indexIn(_contigname);
    if( pos > -1 ) {
        shortContigname = contigNameRegEx.cap(1);
    }
    else {
        return "";
    }
    return shortContigname;
}

QString Utilities::prepareToPrint(QString str, QChar delim) {
     QRegExp delimExp(delim);

     QString newStr = str.trimmed();

     int pos1 = delimExp.indexIn(newStr);
     if( pos1 > -1 ) {
         newStr = QString("\"") + newStr + QString("\"");
     }
     return newStr;
 }

/**
 * @brief Utilities::getFilesWithPattern files the list of files in a folder with some required text appearing in the anem
 * @param folderName, the folder to search in
 * @param text, the text should appear in the file name
 * @return
 */

QStringList Utilities::getFilesWithPattern(const QString &folderName, const QRegExp &pattern)
{
    QStringList foundFiles;
    QDir folder(folderName);

    if( ! folder.exists() ) return QStringList();
    QStringList files = folder.entryList();

    for (int i = 0; i <  files.size(); ++i) {
        int pos = pattern.indexIn(files[i]);
        if(pos != -1 ) {
          foundFiles.append(folder.absoluteFilePath(files[i]));
        }
    }
    return foundFiles;
}



unsigned int Utilities::numNonZeros(const QVector<double> &v) {
    unsigned int count =0;
    for(int i =0; i < v.size(); i++)
        if( v[i] > 1e-100) count++;

    return count;
}


void Utilities::removeZeros(QVector<double> &x, QVector<double> &y, unsigned int index  ) {
    QVector<double> _x, _y;
    for(int i =0; i < x.size(); i++ ) {
        if( index ==0 ) {
            if( x[i] > 0.0 )  {
                _x.push_back(x[i]); _y.push_back(y[i]);
            }
        }
        else {
            if( y[i] > 0.0 )  {
                _x.push_back(x[i]); _y.push_back(y[i]);
            }
        }
    }

    x = _x;
    y = _y;
}


unsigned int Utilities::uniqueORFsCount(const  QList<ORF *> &orfList) {
    QHash<QString, bool> _names;

    foreach(ORF *orf, orfList) {
        if( !_names.contains(orf->name)) _names.insert(orf->name, true);
    }

    return _names.size();
}

QString Utilities::getSubSequence(QString str,QString name, unsigned int start, unsigned int end) {

    QString sequence;
    QRegExp begPattern(">");

    foreach( QString seq, str.split("\n") ) {
        if( begPattern.indexIn(seq) > - 1 ) continue;
        sequence.append(seq.trimmed());
    }

    unsigned int len =  static_cast<unsigned int>(sequence.size());

    if( start > len || end > len) return QString();

    if( start > end ) {
        unsigned int temp = start;
        start = end;
        end = temp;
    }

    return  ( QString(">") + name + QString("\n") + sequence.mid(start, (end -start) + 1)  + QString("\n"));
}



/**
 * @brief compareFreq compares to taxon based on the taxon frequency
 * @param a, first taxon
 * @param b, second taxon
 * @return
 */
bool Utilities::compareFreq(const  TaxonFreqQPair &a, const TaxonFreqQPair &b) {
    //qDebug() << a->strTemp << "  " << b->strTemp << "  " << QString::compare(a->strTemp,   b->strTemp);
    return a.second  > b.second;
}

/**
 * @brief showInfo, displaying any information to the user
 * @param info, the string that contains the info
 */
void Utilities::showInfo(QWidget *parent, QString info) {
    QMessageBox::warning(parent, info, info, QMessageBox::Ok);
}


/**
 * @brief Utilities::exportMeganCompatibleFile, exports the LAST/BLAST lines for the set of orfNames to a megan usable table
 * @param inputFileName, the LAST/BLAST result file
 * @param outputFileName, the file to output to
 * @param orfNames, the list of ORFs to output
 * @return
 */
bool Utilities::exportMeganCompatibleFile( QString inputFileName, QString outputFileName,  QHash<QString, bool> orfNames) {
    QFile inputFile(inputFileName);
    QFile outputFile(outputFileName);

    if (outputFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outputFile);
        if(inputFile.open(QIODevice::ReadOnly | QIODevice::Text )) {
           QTextStream in(&inputFile);
           while ( !in.atEnd() )  {
               QString rawline = in.readLine().remove(("[\\n]"));
               QStringList line = rawline.split(QRegExp("[\\t]"));
               if(line.size() != 12) { continue; }
               if( line.size() ==0 ) { continue; }
               if( !orfNames.contains(Utilities::getShortORFId(line[0])) ) { continue; }
               out << rawline << "\n";
           }
           inputFile.close();
        }
        outputFile.close();
        return true;
    }
    return false;
}


