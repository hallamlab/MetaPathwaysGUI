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


    qDebug() << "Reading in line 20 " << fileName;
    int read = readDataFile(fileName, delim, firstRowAsHeaders);

    pivotPointer = 0.0;
    lastUsedField = 1000000;
    sortingDirectionForward = true;
    qDebug() << " Created the LargeTable";

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

    int t1;
    double t2;

    index = new unsigned int[numCols];

    unsigned int intCounter=0, doubleCounter=0, strCounter=0;
    for(unsigned int i =0; i < numCols; i++ ) {
        qDebug() << numCols ;
        if( types[i] == INT)  {index[i] = intCounter; intCounter++;};
        if( types[i] == DOUBLE) { index[i] = doubleCounter; doubleCounter++;}
        if( types[i] ==STRING)  { index[i] = strCounter; strCounter++ ;}
    }


    qDebug() << "Reading in the file at 55 " << fileName;
    unsigned int i = 0;
    QRegExp comment("#[^\"\\n\\r]*");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();

            if(i==0 && firstRowAsHeaders ) {
                QStringList fields = line.split(QRegExp(delim));
                colNames = new QStringList(fields);
                numCols = colNames->length();
                i++;
                continue;
            }

            if (ignoreComments && comment.exactMatch(line)) continue;

            ROW *list = new ROW;
            if (i%2 == 0) list->visible = true;
            else list->visible = false;

            QStringList qlist = line.split(QRegExp(delim));
          //  qDebug() << qlist << "  " <<i;
            if( qlist.size() == numCols) {
                for(unsigned int k = 0;k<numCols;k++){
                //qDebug() << "   Field " << qlist[k];
                   if( types[k]== INT) {
                       qlist[k].toInt(&ok);
                    //qDebug() << "   " << "INT";
                       if (ok)
                           list->intVar.append(qlist.at(k).toInt());
                       else
                           list->intVar.append(-1);
                   }
                   else if( types[k]==DOUBLE) {
                      qlist[k].toDouble(&ok);
                   // qDebug() << "   " << "DOUBLE";

                      if(ok)
                          list->doubleVar.append(qlist.at(k).toDouble());
                      else
                          list->doubleVar.append(-1);

                   }
                   else if( types[k]==STRING){
                   // qDebug() << "   " << "STR";
                       if( qlist[k].isEmpty()) qlist[k]="-";
                       list->strVar.append(QString(qlist[k]));
                   }
                 }
                 tableData.append(list);
           }
           i++;
        }
    }else return 0;

    qDebug() << " All done at k = " << i;


    inputFile.close();
    qDebug() << "Closed the file";

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
       qDebug() << "Copied the strings";
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

void LargeTable::getData( QList<ROW *> &data, int pivotPoint, unsigned int  deltaW) {
    data.clear();
    int j = 0;
    for (int i = pivotPoint; j < deltaW && i < tableData.length(); i++){
        if (tableData.at(i)->visible){
            j++;
            data.append(tableData.at(i));
        }
    }
}
