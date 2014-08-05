#include "largetable.h"

LargeTable::LargeTable()
{
     numCols = 0;
     this->addLCAStar = false;
}


LargeTable::LargeTable(unsigned int i): numCols(i)
{
}


LargeTable::LargeTable(const QString fileName, const QChar delim, bool ignoreComments, bool firstRowAsHeaders, QList<TYPE> _types){
    this->types = new enum TYPE[ _types.length()];

    this->numCols = _types.size();
    this->addLCAStar = false;

    unsigned int i =0;
    foreach(enum TYPE type, _types) {
        *(this->types + i) = type;
        i++;
    }

    int read = readDataFile(fileName, delim, firstRowAsHeaders);

    pivotPointer = 0.0;
    lastUsedField = 1000000;
    sortingDirectionForward = true;


}

// read select columns
LargeTable::LargeTable(const QString fileName, const QChar delim,  bool ignoreComments, bool firstRowAsHeaders, QList<TYPE> &_types, QList<unsigned int> &columns, QRegExp filter){
    this->types = new enum TYPE[ _types.length()];

    this->numCols = _types.size();
    this->addLCAStar = false;

    unsigned int i =0;
    foreach(enum TYPE type, _types) {
        *(this->types + i) = type;
        i++;
    }

    int read = this->readDataFile(fileName, delim, columns, filter, firstRowAsHeaders);

    pivotPointer = 0.0;
    lastUsedField = 1000000;
    sortingDirectionForward = true;


}

LargeTable::~LargeTable() {


    foreach(ROW *r, wholeTableData ){
        delete r;
    }
}


int LargeTable::getPivot(){
    return this->pivotPointer;
}

void LargeTable::setPivot(int pivot){
    this->pivotPointer = pivot;
}

/**
 * @brief LargeTable::wholeTableData, copies all the rows to the filtered
 * list
 */
void LargeTable::copyWholeToFilter() {
    this->tableData.clear();
    foreach(ROW *row, this->wholeTableData)
        this->tableData.append(row);
}

/**
 * @brief LargeTable::readDataFile, reads file of data from a file with the given delim and ignore comments flag
 * and firstRowAsHeads flag
 * @param fileName
 * @param delim
 * @param ignoreComments
 * @param firstRowAsHeaders
 * @return
 */
int LargeTable::readDataFile(const QString fileName, const QChar delim,  const bool ignoreComments, const bool firstRowAsHeaders ){
    QFile inputFile(fileName);

    bool ok;

    index = new unsigned int[numCols];

    unsigned int intCounter=0, doubleCounter=0, strCounter=0;
    for(unsigned int i =0; i < numCols; i++ ) {
    //    qDebug() << numCols ;
        if( types[i] == INT)  {index[i] = intCounter; intCounter++;};
        if( types[i] == DOUBLE) { index[i] = doubleCounter; doubleCounter++;}
        if( types[i] ==STRING)  { index[i] = strCounter; strCounter++ ;}
    }

  //  qDebug() << "Reading in the file at 55 " << fileName;
    unsigned int i = -1;
    QRegExp comment("#[^\"\\n\\r]*");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();

            if(i== -1 && firstRowAsHeaders ) {
                colNames = line.split(QRegExp(delim));                
                numCols = colNames.length();
                i++;
                continue;
            }

            if (ignoreComments && comment.exactMatch(line)) continue;

            ROW *list = new ROW;

            QStringList qlist = line.split(QRegExp(delim));

            if( qlist.size() == numCols) {
                for(unsigned int k = 0;k<numCols;k++){

                   if( types[k]== INT) {
                       qlist[k].toInt(&ok);

                       if (ok)
                           list->intVar.append(qlist.at(k).toInt());
                       else
                           list->intVar.append(-1);
                   }
                   else if( types[k]==DOUBLE) {
                      qlist[k].toDouble(&ok);


                      if(ok)
                          list->doubleVar.append(qlist.at(k).toDouble());
                      else
                          list->doubleVar.append(-1);

                   }
                   else if( types[k]==STRING){

                       if( qlist[k].isEmpty()) qlist[k]="-";
                       list->strVar.append(QString(qlist[k]));
                   }
                 }
           //      tableData.append(list);
                 wholeTableData.append(list);
                 list->rank = i;
            }
            i++;
        }
    }else return 0;

    inputFile.close();


    return 0;

}

//selected columns read data file
/**
 * @brief LargeTable::readDataFile
 * @param fileName
 * @param delim
 * @param columns
 * @param filter
 * @param ignoreComments
 * @param firstRowAsHeaders
 * @return
 */

int LargeTable::readDataFile(const QString fileName, const QChar delim, QList<unsigned int> &columns, QRegExp filter, const bool ignoreComments, const bool firstRowAsHeaders ){
    QFile inputFile(fileName);
    QList<QString> fields;
    bool ok;

    this->numCols = columns.size();
    index = new unsigned int[numCols];
    unsigned int lastColumn = columns.last();

    unsigned int intCounter=0, doubleCounter=0, strCounter=0;
    for(unsigned int i =0; i < numCols; i++ ) {
    //    qDebug() << numCols ;
        if( types[i] == INT)  {index[i] = intCounter; intCounter++;};
        if( types[i] == DOUBLE) { index[i] = doubleCounter; doubleCounter++;}
        if( types[i] ==STRING)  { index[i] = strCounter; strCounter++ ;}
    }

    colNames.clear();

    unsigned int i = -1;
    QRegExp comment("#[^\"\\n\\r]*");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();

            if(i== -1 && firstRowAsHeaders ) {
                fields = line.split(QRegExp(delim), QString::KeepEmptyParts);

                if(fields.size() <= lastColumn) continue;

                for(unsigned int j=0; j < numCols; j++){
                    colNames << fields[columns[j]] ;
                }
                i++;
                continue;
            }

            if (ignoreComments && comment.exactMatch(line)) continue;

            if(filter.indexIn(line,0)==-1) continue;

            ROW *list = new ROW;

            QStringList qlist = line.split(QRegExp(delim));

            if( qlist.size() >  lastColumn) {
                for(unsigned int k = 0; k < numCols; k++){
                    unsigned int _k = columns[k];

                   if( types[k]== INT) {
                       qlist[_k].toInt(&ok);

                       if (ok)
                           list->intVar.append(qlist.at(_k).toInt());
                       else
                           list->intVar.append(-1);
                   }
                   else if( types[k]==DOUBLE) {
                      qlist[_k].toDouble(&ok);


                      if(ok)
                          list->doubleVar.append(qlist.at(_k).toDouble());
                      else
                          list->doubleVar.append(-1);

                   }
                   else if( types[k]==STRING){

                       if( qlist[_k].isEmpty()) qlist[_k]="-";
                       list->strVar.append(QString(qlist[_k]));
                   }
                 }

                 tableData.append(list);
                 wholeTableData.append(list);
                 list->rank = i;
            }
            i++;
        }
    }else return 0;

    inputFile.close();

    return 0;
}

class CompareIntField {
  public:
    CompareIntField()  {
    }
    bool operator()(const  ROW *a, const ROW *b) {
        return (a->intTemp <=  b->intTemp);
    }

};

class CompareStrField {
  public:
    CompareStrField(){
    }

    bool operator()(const  ROW *a, const ROW *b) {
        return a->strTemp <=  b->strTemp;
    }
};

class CompareDoubleField {
  public:
    CompareDoubleField() {
    }
    bool operator()(const  ROW *a, const ROW *b) {
        return a->doubleTemp<=  b->doubleTemp;
    }
};

void LargeTable::copyDataToSort(unsigned int k) {
    if(types[k]==INT) {
       foreach(ROW *r, tableData)
           r->intTemp = r->intVar[index[k]];
    }

    if(types[k]==DOUBLE)
       foreach(ROW *r, tableData)
           r->doubleTemp = r->doubleVar[index[k]];

    if(types[k]==STRING) {
       foreach(ROW *r, tableData)
           r->strTemp = r->strVar[index[k]];

    }
}

bool compareInt(const  ROW *a, const ROW *b) {
   // qDebug() << a->intTemp << "  " << b->intTemp;
    return a->intTemp<   b->intTemp;
}


bool compareDouble(const  ROW *a, const ROW *b) {
   // qDebug() << a->intTemp << "  " << b->intTemp;
    return a->doubleTemp<   b->doubleTemp;
}

bool compareStr(const  ROW *a, const ROW *b) {
    //qDebug() << a->strTemp << "  " << b->strTemp << "  " << QString::compare(a->strTemp,   b->strTemp);
    return a->strTemp< b->strTemp;
}


bool compareIntRev(const  ROW *a, const ROW *b) {
   // qDebug() << a->intTemp << "  " << b->intTemp;
    return a->intTemp >   b->intTemp;
}

bool compareDoubleRev(const  ROW *a, const ROW *b) {
   // qDebug() << a->intTemp << "  " << b->intTemp;
    return a->doubleTemp >   b->doubleTemp;
}

bool compareStrRev(const  ROW *a, const ROW *b) {
    //qDebug() << a->strTemp << "  " << b->strTemp << "  " << QString::compare(a->strTemp,   b->strTemp);
    return a->strTemp > b->strTemp;
}

void LargeTable::sortByField(unsigned int fieldNum) {

    if(lastUsedField!=fieldNum) {
        copyDataToSort(fieldNum);
        lastUsedField = fieldNum;
        sortingDirectionForward = true;
    }
    if( types[fieldNum]== INT) {
        if(sortingDirectionForward)
           qSort(tableData.begin(), tableData.end(), compareInt);
        else
           qSort(tableData.begin(), tableData.end(), compareIntRev);
    }

    if( types[fieldNum]== DOUBLE) {
        if(sortingDirectionForward)
           qSort(tableData.begin(), tableData.end(), compareDouble);
        else
           qSort(tableData.begin(), tableData.end(), compareDoubleRev);
    }

    if( types[fieldNum]== STRING) {
        if(sortingDirectionForward)
           qSort(tableData.begin(), tableData.end(), compareStr);
        else
           qSort(tableData.begin(), tableData.end(), compareStrRev);
    }

    if(sortingDirectionForward)
        sortingDirectionForward = false;
    else
        sortingDirectionForward = true;
}



unsigned int LargeTable::getBegData( int pivotPoint, unsigned int  deltaW) {
    return  pivotPoint;
}


/**
 * @brief LargeTable::setLCAStarAlpha, sets the lca star value
 * @param value
 */
void LargeTable::setLCAStarAlpha(double value) {
    this->lca_star_alpha = value;
}


/**
 * @brief LargeTable::setLCAStarDepth, sets the min depth for the
 * lca depth
 * @param depth
 */
void LargeTable::setLCAStarDepth(unsigned int depth) {    
    this->lca_star_min_depth = depth;
}

/**
 * @brief LargeTable::lca_star, computes the lca star value of the list of orfs
 * @param orfList
 * @param lcastar
 * @return
 */
QString LargeTable::lca_star(const QList<ORF *> &orfList, LCAStar *lcastar) {
    QStringList taxalist;

    foreach( ORF *orf, orfList) {
        taxalist.append( orf->attributes[TAXON]->name);
    }

    lcastar->setLCAStarAlpha(this->lca_star_alpha);
    lcastar->setLCAStarDepth(this->lca_star_min_depth);
    return lcastar->lca_star(taxalist).taxon;

}

/**
 * @brief LargeTable::getData, gets the rows required to fill the visible part of the dta
 * is the data to select, pivotpoint is where the selection starts and deltaW is the width
 * or buffer before the load new data trigger is set off
 * @param data
 * @param pivotPoint
 * @param deltaW
 */
void LargeTable::getData( QList<ROW *> &data, int pivotPoint, unsigned int  deltaW) {
    LCAStar *lcastar = LCAStar::getLCAStar();

    DataManager *datamanager = DataManager::getDataManager();
    unsigned int contigCol = 1, lcaCol = 6;

    data.clear();

    QHash<QString, QString> lcaForContigs;

    for (unsigned int i = pivotPoint; i < pivotPoint + 2*deltaW && i < tableData.length(); i++){
        if(this->addLCAStar ) {
            QString contig = Utilities::getShortContigId(tableData.at(i)->strVar[contigCol]);
            if( !lcaForContigs.contains(contig)) {
                  QList<ORF *> orfs = datamanager->getORFList(this->sampleName, contig);
                  lcaForContigs[contig] = this->lca_star(orfs, lcastar);
            }
            tableData[i]->strVar[lcaCol] = lcaForContigs[contig];
        }

        data.append(tableData.at(i));
    }
}


/**
 * @brief LargeTable::getORFNames, gets the list of ORF names in the tableData
 * @return
 */
QList< QString> LargeTable::getORFNames() {

    QList<QString> orfNames;
    unsigned orfCol = 0;
    for (unsigned int i = 0;  i < tableData.length(); i++){
        QString orfName = Utilities::getShortORFId(tableData.at(i)->strVar[orfCol]);
        orfNames.append(orfName);
    }
    return orfNames;
}


void LargeTable::getSelectRows(QList<ROW *> & sourcerows,  QList<ROW *> & selectrows, QString searchItem, unsigned int col, enum TYPE type) {
    for (unsigned int i = 0;  i < sourcerows.length(); i++){
       if( sourcerows.at(i)->strVar.at(this->index[col]).indexOf(searchItem, 0, Qt::CaseSensitive) > -1 && searchItem.length() == sourcerows.at(i)->strVar.at(this->index[col]).length() ) {
           selectrows.append(sourcerows[i]);
       }
    }
}

void LargeTable::markRowsSearch(QList<SEARCH> &searchFilters, OPTYPE optype, bool caseSensitive ) {
    enum DECISION hit;
    unsigned int k=0;
    tableData.clear();

    Qt::CaseSensitivity _caseSensitive =  caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;


    if(searchFilters.size()==0) {
       for (unsigned int i = 0;  i < wholeTableData.length(); i++){
             tableData.append(wholeTableData.at(i));
       }
       return;
    }

    //consider a row i in the whole data
    for (unsigned int i = 0;  i < wholeTableData.length(); i++){
        //one search at a time
        hit = NO;
        foreach(SEARCH searchFilter, searchFilters) {
            //empty searchquery
            if( searchFilter.searchItem.size()==0 ) continue;

            //one column j in row i
            hit=NO;
            foreach(unsigned int j, searchFilter.searchCols) {
               if( wholeTableData.at(i)->strVar.at(this->index[j]).indexOf(searchFilter.searchItem, 0, _caseSensitive) > -1 ) {  hit= YES; break; }
            }
            if(hit==YES && optype == OR) break;
            if(hit==NO && optype == AND ) break;
        }

        if(hit == YES ) {
           tableData.append(wholeTableData.at(i));
           k++;
        }

    }
}



