#include "displayinfo.h"
#include "ui_displayinfo.h"

DisplayInfo::DisplayInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisplayInfo)
{
    ui->setupUi(this);


    textOut = this->findChild<QTextEdit *>("textOut");
    okAndCancel = this->findChild<QDialogButtonBox *>("okAndCancel");
    sourceFile = "";
    sourceType = FASTA;
}

DisplayInfo::~DisplayInfo()
{
    delete ui;
}


void DisplayInfo::addFileIndex(FileIndex *fileIndex, unsigned int col) {

    this->fileIndexes.insert(col, fileIndex);

}

void DisplayInfo::removeFileIndexes() {
    this->fileIndexes.clear();
}

void DisplayInfo::clickedOnCell(int row, int col) {

    qDebug() << "Row " << row << " col " << col;

    this->textOut->setText("Hello how are you?");
    this->show();
}



void DisplayInfo::itemDoubleClicked(QTableWidgetItem * item) {

    QString key = item->text();
    QString result;
    unsigned int column = item->column();

    FileIndex *fileIndex = fileIndexes.contains(column) ? fileIndexes[column] : 0;
    if( fileIndex ==0 )
        result = QString("");
    else
         result = fileIndex->getDataToDisplay(key);

    this->textOut->setText(result);
    this->show();
}