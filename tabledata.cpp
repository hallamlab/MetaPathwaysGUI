#include "tabledata.h"
#include "ui_tabledata.h"
#include "utilities.h"
#include "QDebug"

TableData::TableData(const QString &file, QWidget *parent) :
    QWidget(parent), MetaWidget(),
    ui(new Ui::TableData)
{
    ui->setupUi(this);

    this->file = file;
    titleLabel = this->findChild<QLabel *>("titleLabel");
    statsLabel = this->findChild<QLabel *>("statsLabel");
    tableData = this->findChild<QTableWidget *>("tableData");

    this->setupFromFile(file);
}

void TableData::setupFromFile(const QString &){
    QChar labelDELIM = '$';
    QChar tableDELIM = '#';

    QList<QString> *labelText = Utilities::parseResults(this->file,labelDELIM);

    delete tableData;
    tableData = Utilities::createTable(this->file, tableDELIM);
    this->layout()->addWidget(tableData);

    QString summaryText = "";
    foreach(QString s, *labelText){
        summaryText.append(s.trimmed());
        summaryText.append("\n");
    }

    statsLabel->setText(summaryText);
    titleLabel->setText(this->file);
}

TableData::~TableData()
{
    delete ui;
}
