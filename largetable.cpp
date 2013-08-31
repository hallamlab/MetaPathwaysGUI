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
            // process header seperately
          //  qDebug() << line;


            if(i==0 && firstRowAsHeaders ) {
                QStringList fields = line.split(QRegExp(delim));
                colNames = new QStringList(fields);
                numCols = colNames->length();
                i++;
                continue;
            }

            if (ignoreComments && comment.exactMatch(line)) continue;

            ROW *list = new ROW;
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
                    if( qlist[k].isEmpty()) qlist[k]="--";
                    list->strVar.append(QString(qlist[k]));
                }
            }

               if( list->strVar.length() < 5) {
                //for(unsigned int j=0 ; j < 8; j++)
                  qDebug() << qlist << "  " << i;
                /*
                qDebug() << " Type " << types[j];
                qDebug() << "Serious error ine line " << i;
                qDebug() << "number of string values " << list->strVar.length() ;
                qDebug() << "number of int values " << list->intVar.length() ;
                qDebug() << "number of double values " << list->doubleVar.length() ;
                qDebug() << " number of cols " << numCols;
                */
                //exit(0);

               }

               tableData.append(list);
           }
           i++;
         }
    }else return 0;

    qDebug() << " All done at k = " << i;


    inputFile.close();

    return 0;

}



class CompareIntField {
  public:
    CompareIntField(unsigned int field, enum TYPE *_types, unsigned int *_index, unsigned int _numCols)  {
        i = field;
        numCols = _numCols;
        types = new enum TYPE[numCols];
        index = new unsigned int[numCols];
        for(unsigned int j=0 ; j < numCols; j++ ) {
            types[j] = _types[j];
            index[j] = _index[j];
        }

    }
    bool operator()(const  ROW *a, const ROW *b) {
        return (a->intVar.at(index[i]) <=  b->intVar.at( index[i]));
    }
private:
    unsigned int i; //col to compare
    enum TYPE *types;
    unsigned int *index;
    unsigned int numCols;

};

class CompareStrField {
  public:
    CompareStrField(unsigned int field, enum TYPE *_types, unsigned int *_index, unsigned int _numCols){
        i = field;
        numCols = _numCols;
        types = new enum TYPE[numCols];
        index = new unsigned int[numCols];
        for(unsigned int j=0 ; j < numCols; j++ ) {
            types[j] = _types[j];
            index[j] = _index[j];
        }

    }

    bool operator()(const  ROW *a, const ROW *b) {
        return a->strVar.at(index[i]) <=  b->strVar.at( index[i]);
    }
private:
    unsigned int i; //col to compare
    enum TYPE *types;
    unsigned int *index;
    unsigned int numCols;

};

class CompareDoubleField {
  public:
    CompareDoubleField(unsigned int field, enum TYPE *_types, unsigned int *_index, unsigned int _numCols) {
        i = field;
        numCols = _numCols;
        types = new enum TYPE[numCols];
        index = new unsigned int[numCols];
        for(unsigned int j=0 ; j < numCols; j++ ) {
            types[j] = _types[j];
            index[j] = _index[j];
        }
    }
    bool operator()(const  ROW *a, const ROW *b) {
        return a->doubleVar.at(index[i]) <=  b->doubleVar.at( index[i]);
    }
private:
    unsigned int i; //col to compare
    enum TYPE *types;
    unsigned int *index;
    unsigned int numCols;

};


void LargeTable::sortByField(unsigned int fieldNum) {
    if( types[fieldNum]== INT) qSort(tableData.begin(), tableData.end(), CompareIntField(fieldNum, types, index, numCols));
    if( types[fieldNum]== DOUBLE) qSort(tableData.begin(), tableData.end(), CompareDoubleField(fieldNum, types, index, numCols));
    if( types[fieldNum]== STRING) qSort(tableData.begin(), tableData.end(), CompareStrField(fieldNum, types, index, numCols));
}


unsigned int LargeTable::getBegData( int pivotPoint, unsigned int  deltaW) {
    return  pivotPoint;
}

void LargeTable::getData( QList<ROW *> &data, int pivotPoint, unsigned int  deltaW) {
    for (int i =pivotPoint; i <  pivotPoint + deltaW && i < tableData.length(); i++){
        data.append(tableData.at(i));
    }
}
