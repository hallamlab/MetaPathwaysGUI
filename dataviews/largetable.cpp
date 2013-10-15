#include "largetable.h"

LargeTable::LargeTable()
{
     numCols = 0;
}


LargeTable::LargeTable(unsigned int i): numCols(i)
{
}


LargeTable::LargeTable(const QString fileName, const QChar delim, bool ignoreComments, bool firstRowAsHeaders, QList<TYPE> _types){
    this->types = new enum TYPE[ _types.length()];

    this->numCols = _types.size();
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

//selected columns read data file

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
                fields = line.split(QRegExp(delim));
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

    qDebug() << " Sorting by field " << fieldNum;
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

void LargeTable::getData( QList<ROW *> &data, int pivotPoint, unsigned int  deltaW) {
    data.clear();

    for (unsigned int i = pivotPoint; i < pivotPoint + 2*deltaW && i < tableData.length(); i++){
        data.append(tableData.at(i));
    }
}


void LargeTable::getSelectRows(QList<ROW *> & sourcerows,  QList<ROW *> & selectrows, QString searchItem, unsigned int col, enum TYPE type) {
    for (unsigned int i = 0;  i < sourcerows.length(); i++){
       if( sourcerows.at(i)->strVar.at(this->index[col]).indexOf(searchItem, 0, Qt::CaseSensitive) > -1 && searchItem.length() == sourcerows.at(i)->strVar.at(this->index[col]).length() ) {
           selectrows.append(sourcerows[i]);
       }
    }
}

void LargeTable::markRowsSearch(SEARCH &searchFilter) {
    enum DECISION hit;
    unsigned int k=0;
    tableData.clear();
    Qt::CaseSensitivity caseSensitive =  searchFilter.caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

   // Qt::CaseInsensitive

    for (unsigned int i = 0;  i < wholeTableData.length(); i++){
        hit=MAYBE;
        if(searchFilter.searchItems.size()==0 ) hit = YES;
        foreach(unsigned int j, searchFilter.searchCols) {
            foreach(QString searchItem, searchFilter.searchItems) {
                if( searchItem.size() ==0 ) { hit=YES; break; }
                if( searchFilter.type == OR) {
                   if( wholeTableData.at(i)->strVar.at(this->index[j]).indexOf(searchItem, 0, caseSensitive) > -1 ) {
                        hit= YES;
                        break;
                    }
                }

                if( searchFilter.type == AND) {
                   if( wholeTableData.at(i)->strVar.at(this->index[j]).indexOf(searchItem, 0, caseSensitive) == -1 ) {
                        hit= NO;
                        break;
                    }
                }
            }
            if( hit != MAYBE) break;
        }

        if(hit == YES ) {
            tableData.append(wholeTableData.at(i));
            k++;
        }
    }
}



