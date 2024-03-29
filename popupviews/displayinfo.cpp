#include "displayinfo.h"
#include "ui_displayinfo.h"

DisplayInfo::DisplayInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisplayInfo)
{
    ui->setupUi(this);


    textOut = this->findChild<QTextEdit *>("textOut");
    okAndCancel = this->findChild<QDialogButtonBox *>("okAndCancel");

    orfMap = this->findChild<QPushButton *>("orfMap");
    connect(orfMap, SIGNAL(clicked()), this, SLOT(openGenomeView()));

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

    // qDebug() << "Row " << row << " col " << col;

    this->textOut->setPlainText("Hello how are you?");
    this->show();
}



void DisplayInfo::itemDoubleClicked(QTableWidgetItem * item) {

    QString key = item->text();
    QString result;
    unsigned int column = item->column();

    FileIndex *fileIndex = fileIndexes.contains(column) ? fileIndexes[column] : 0;
    if( fileIndex ==0 ) {
        result = QString("");
    }
    else {
        if(column ==0 ) {
            key = Utilities::getShortORFId(key);
        }
        else if(column==4) {
            key= Utilities::getShortContigId(key);
        }

         result = fileIndex->getDataToDisplay(key);
         result = Utilities::insertCharacterAtIntervals(result, '\n',10000);
         this->textOut->setText(result);
        //    document = new QTextDocument(result);
        //    this->textOut->setDocument(document);
         this->show();
    }
}


void DisplayInfo::receiveSequenceData( QTableWidgetItem *item, GeneBrowserData gbdata) {
     this->itemDoubleClicked(item);
     this->gbdata = gbdata;
}

void DisplayInfo::openGenomeView() {


    //QScrollArea *area = new QScrollArea;

    GenomeView *gv = new GenomeView;
  //  area->setWidget(gv);
   // area->show();
    gv->showData(this->gbdata);
    gv->show();
//    t->setPopupListener(gv);

}



