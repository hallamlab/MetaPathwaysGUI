#include "tabledata.h"
#include "ui_tabledata.h"
#include "utilities.h"
#include <QFile>
#include "QDebug"

TableData::TableData(bool HAS_COMMENT, const QString &file, QWidget *parent) :
    QWidget(parent), MetaWidget(),
    ui(new Ui::TableData)
{
    ui->setupUi(this);

    this->file = file;
    this->HAS_COMMENT = HAS_COMMENT;
    titleLabel = this->findChild<QLabel *>("titleLabel");
    statsLabel = this->findChild<QLabel *>("statsLabel");
    tableData = this->findChild<QTableWidget *>("tableData");

    statsText = new QString();

    this->setupFromFile(file);
}

void TableData::setupFromFile(const QString &file){
    QChar tableDELIM = '#';

    data = parseFile(file, tableDELIM, this->HAS_COMMENT);
    titleLabel->setText(file);
    if (HAS_COMMENT) statsLabel->setText(*statsText);
    else statsLabel->hide();

    this->populateTable(*tableData, *data, *headers);
}

/*
 * Creates the actual table widget the data given to it, and populates the table with data and headers given with
 * the pointer to the table given.
 */
QTableWidget* TableData::populateTable(QTableWidget &table, const QList<QStringList> &data, const QStringList &headers){
    table.setColumnCount(headers.size());
    table.setSortingEnabled(true);
    table.setHorizontalHeaderLabels(headers);

    int k=0;

    foreach (QStringList sl, data){
        table.insertRow(table.rowCount());
        for (int i=0;i<sl.length();i++){

            //try to cast to an integer
            bool ok;
            sl.at(i).toInt(&ok,10);

            if (ok){
                table.setItem(k,i,new TableNumberItem(sl.at(i)));
            }else{
                table.setItem(k,i,new QTableWidgetItem(sl.at(i)));
            }
            table.item(k,i)->setTextAlignment(Qt::AlignCenter);
        }
        k++;
    }
    table.resizeColumnsToContents();
    table.resizeRowsToContents();
    table.horizontalHeader()->setStretchLastSection(true);
}


/*
 * Parses a result file. If HAS_COMMENT is true, then it expects that there are lines with DELIM in certain lines
 * of the text file.
 */
QList<QStringList>* TableData::parseFile(const QString &file, const QChar &DELIM, bool HAS_COMMENT){
    QFile inputFile(file);
    QList<QStringList>* data = new QList<QStringList>();
    bool firstLine = true;

    if (inputFile.open(QIODevice::ReadOnly)){
        QTextStream in(&inputFile);
        while ( !in.atEnd() ){
            QString line = in.readLine().trimmed();
            QStringList lineSplit = line.split(QRegExp("\\t"));
            lineSplit.removeAll("");

            if (!lineSplit.isEmpty()){
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
                    QStringList *headerList = new QStringList(lineSplit);
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
