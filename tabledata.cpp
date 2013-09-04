#include "tabledata.h"
#include "ui_tabledata.h"
#include "utilities.h"
#include <QScrollBar>
#include <QtCore>
#include <QFile>
#include "QDebug"

TableData::TableData(bool LARGE, bool HAS_COMMENT, const QString &file, QList<enum TYPE> _types ,  QWidget *parent) :
    QWidget(parent), MetaWidget(),
    ui(new Ui::TableData)
{
    ui->setupUi(this);

    this->setNumCols(_types.size());
     qDebug() << "Going to create the file table data form " << file;
    foreach(enum TYPE type, _types) {
        types.append(type);
    }



    this->file = file;
    this->HAS_COMMENT = HAS_COMMENT;
    this->LARGE = LARGE;

    titleLabel = this->findChild<QLabel *>("titleLabel");
    statsLabel = this->findChild<QLabel *>("statsLabel");
    tableWidget = this->findChild<QTableWidget *>("tableWidget");
    dw = 500;
    statsText = new QString();

    top = 0;
    bottom = 0;
qDebug() << "Going to read from  file " << file;
    this->setupFromFile(file);

    if (LARGE){

        connect(tableWidget->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(outputRows(int)));
        connect(tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(headerClicked(int)));
    }
    qDebug() << "created the file table data form " << file;



}

void TableData::setNumCols(unsigned int numCols) {
    this->numCols = numCols;
}

void TableData::headerClicked(int index){
    qDebug() << "sorting";
    this->largeTable->sortByField(index);
    qDebug() << "done sorting";
    tableWidget->clearContents();

    int currPos = tableWidget->verticalScrollBar()->value();
        qDebug() << "getting sorted data at " << currPos;
    this->largeTable->getData(bigdata, currPos,dw);
    qDebug() << "putting it into the table " << bigdata.length();
    this->populateTable(*tableWidget, bigdata, *headers, currPos);
}

void TableData::outputRows(int top){
//    int pivot = qFloor((top+bottom)/2);
//    qDebug() << "Top row" << top;
//    qDebug() << "Bottom row" << bottom;
//    qDebug() << "pivot is at " << pivot;
  //  double bottom = this->tableWidget->verticalHeader()->visualIndexAt(height());

    this->top = top;
    //bottom = (bottom < 0)? largeTable->tableData.length() : bottom ;

    //this->bottom  = bottom;


    updateData(top, dw);


}

void TableData::updateData(double top, double bottom){

 //   double allDataLength = (double)largeTable->tableData.length();



    int pivot =  top;




    qDebug() << "pivot "  << pivot << " range " << top << "  " << bottom;

     if (largeTable->getPivot()  - 0.9 * dw >=  pivot ||
         pivot >= largeTable->getPivot()  + 0.9 *dw){

         largeTable->setPivot(pivot);
         largeTable->getData(bigdata, pivot, dw);
         //int from = largeTable->getBegData(pivot, dw);
 //        tableData->clearContents();


         this->populateTable(*tableWidget, bigdata, *headers, pivot );
 //      tableData->repaint();

    }




}

void TableData::setupFromFile(const QString &file){
    QChar tableDELIM = '#';


       if (!LARGE){
           data = parseFile(file, tableDELIM, this->HAS_COMMENT);
           tableWidget->setRowCount(data->length());
       }
       else{

          largeTable = new LargeTable(file, '\t', true, true, types);

           qDebug() << " Getting the data";
           largeTable->getData(bigdata, 0,dw);
           qDebug() << " Gotten the data";
           qDebug() << "Size of data" << bigdata.length();
           tableWidget->setRowCount(largeTable->tableData.length());
           tableWidget->setColumnCount(largeTable->colNames->length());
           headers = largeTable->colNames;

        }


        titleLabel->setText(file);
        if (HAS_COMMENT) statsLabel->setText(*statsText);
        else statsLabel->hide();



        if( !LARGE && data != 0) {
            qDebug() << " Going to insert small data";
            this->populateTable(*tableWidget, *data, *headers, top);
        }
        if( LARGE && !bigdata.empty() ) {
            qDebug() << "Large data going to read";
           this->populateTable(*tableWidget, bigdata, *headers, top);
        }
        qDebug() << "Done";

}

/*
 * Creates the actual table widget the data given to it, and populates the table with data and headers given with
 * the pointer to the table given.
 */
void TableData::populateTable(QTableWidget &table, const QList<QStringList *> &data, const QStringList &headers, int from){

    table.setColumnCount(headers.size());
    if( !LARGE) table.setSortingEnabled(true);
    table.setHorizontalHeaderLabels(headers);

    int k = from;


    foreach (QStringList *sl, data){
        for (int i=0;i<sl->length();i++){
            //try to cast to an integer
            bool ok;

            sl->at(i).toInt(&ok,10);
            if (ok){
               table.setItem(k,i,new TableNumberItem(sl->at(i)));
            }
            else {
                table.setItem(k,i, new QTableWidgetItem(sl->at(i)) );
            }
            table.item(k,i)->setTextAlignment(Qt::AlignCenter);

           // table.setItem(k,i,0);

        }
        k++;
    }

    /*table.resizeColumnsToContents();
    table.resizeRowsToContents();
    table.horizontalHeader()->setStretchLastSection(true);
    */
}

//BIG DATA
void TableData::populateTable(QTableWidget &table,  QList<ROW *> &data, const QStringList &headers, int from){


    table.setColumnCount(headers.size());
    table.setHorizontalHeaderLabels(headers);

    int k = from;



    table.clearContents();
    foreach( ROW *datum,  data) {
      for( unsigned int i = 0; i < largeTable->numCols; i++) {
          try{
              //table.setItem(k,i,new QTableWidgetItem("aldkfjglkjdsfgjdfsjglkjdsfgjldfkgjjdfglkdfgjldfjgljdsflkjlsdkfjglkjsdflgjlkdsfjgljdfslgjldfgljdsflkgjkdfsjlgkja;sdkfj;alwkj;l435u3209"));
              if( types[i] == INT) {
                 table.setItem(k,i, new QTableWidgetItem(QString::number(datum->intVar.at(largeTable->index[i]))));


              }
              if( types[i] == DOUBLE) table.setItem(k,i,new QTableWidgetItem(QString::number(datum->doubleVar.at(largeTable->index[i]))));


              if(types[i] == STRING) {
                  /*
                  if( datum->strVar.length() <= largeTable->index[i]) {
                      qDebug() << " Length of the data " << data.length();
                      qDebug() << " Wrong access of data";
                      qDebug( ) << " index value " << largeTable->index[i];
                      qDebug() << "num string elements " << datum->strVar.length();
                      qDebug() << "num int elements " << datum->intVar.length();
                      qDebug() << " number of table data " << largeTable->tableData.length();

                  }*/
                  table.setItem(k,i,new QTableWidgetItem(QString(datum->strVar.at(largeTable->index[i]))));
              }

          }
          catch(...) {
              qDebug() << " Failed at " << i << datum << " double " << datum->doubleVar << " int " << datum->intVar << " str " << datum->strVar;

          }
      }
      k++;
    }

          //  qDebug() << ">>>>";
    /*table.resizeColumnsToContents();
    table.resizeRowsToContents();
    table.horizontalHeader()->setStretchLastSection(true);
    */
}




/*
 * Parses a result file. If HAS_COMMENT is true, then it expects that there are lines with DELIM in certain lines
 * of the text file.
 */
QList<QStringList *>* TableData::parseFile(const QString &file, const QChar &DELIM, bool HAS_COMMENT){
    QFile inputFile(file);
    QList<QStringList *>* data = new QList<QStringList *>();
    bool firstLine = true;
    qDebug() << "going to read  the large table";
    if (inputFile.open(QIODevice::ReadOnly)){
        QTextStream in(&inputFile);
        while ( !in.atEnd() ){
            QString line = in.readLine().trimmed();
            QStringList *lineSplit = new QStringList(line.split(QRegExp("\\t")));
            lineSplit->removeAll("");

            if (!lineSplit->isEmpty()){
                if (HAS_COMMENT){
                    //if there are comment lines in the file
                    if (line.at(0)==DELIM){
                        line.remove(0,1);
                        statsText->append(line.trimmed() + '\n');
                        continue; //skip to the next line in the file
                    }
                }
                //take lines as they come
                if (firstLine){
                    QStringList *headerList = lineSplit;
                    headers = headerList;
                    firstLine = false;
                }else {
                    data->append(lineSplit);
                }
            }
        }
    }

    return data;
}

TableData::~TableData()
{
    delete ui;
}
