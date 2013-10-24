#include "exportbox.h"

ExportBox::ExportBox(TableData* td, QWidget *parent)
    // : QWidget(parent)
{
    this->td = new ExportSource(td);
    this->createWidget();
}

ExportBox::ExportBox(HTableData* td, QWidget *parent)
    // : QWidget(parent)
{
    this->td = new ExportSource(td);
    this->createWidget();
}

void ExportBox::createWidget() {
     QVBoxLayout *vbox = new QVBoxLayout;
     createNonExclusiveGroup(vbox);
     setLayout(vbox);

     setWindowTitle(tr("Export"));
     resize(480, 200);
     this->setAttribute(Qt::WA_DeleteOnClose, true);
     connect(exportButton, SIGNAL(clicked()), this, SLOT(saveAs()) );

 }


bool compareColumns(const EXPORT_SELECT a, const EXPORT_SELECT b) {
    //qDebug() << a->strTemp << "  " << b->strTemp << "  " << QString::compare(a->strTemp,   b->strTemp);
    return a.rank < b.rank;
}


void ExportBox::clickedChoice() {
     QList<EXPORT_SELECT> cols;

     unsigned int numSel =0;
     for(unsigned int i=0; i < Columns.size(); i++)
         if(Columns[i].checkbox->isChecked() )
             numSel++;

     for(unsigned int i=0; i < Columns.size(); i++) {
         if(Columns[i].checkbox->isChecked() ) {
             if( Columns[i].rank == -1) Columns[i].rank = numSel;
             cols.append(Columns[i]);
         }
         else {
             Columns[i].checkbox->setText( Columns[i].name);
             Columns[i].rank = -1;
         }
     }
     qSort(cols.begin(), cols.end(), compareColumns);


     for(unsigned int i=0; i< numSel; i++) {
         cols[i].rank = i;
     }

     foreach(EXPORT_SELECT a, cols) {
         a.checkbox->setText( a.name + " [" + QString::number(a.rank + 1 ) + "]");
     }

 }

 QGroupBox *ExportBox::createNonExclusiveGroup(QVBoxLayout *pvbox)
 {
     QGroupBox *groupBox = new QGroupBox(tr("Select Columns to Export"));
     groupBox->setFlat(true);

     QGridLayout *grid = new QGridLayout;
     for(unsigned int i=0; i < this->td->getHeaders().size(); i++ ) {
         QCheckBox *checkBox1 = new QCheckBox(this->td->getHeader(i));
         connect(checkBox1, SIGNAL( clicked()), this, SLOT(clickedChoice()) );
         grid->addWidget(checkBox1,(int)i/5, i%5);
         EXPORT_SELECT s;
         s.checkbox=checkBox1;
         s.rank = -1;
         s.name = this->td->getHeader(i);
         s.col = i;

         this->Columns.append(s);
     }
     groupBox->setLayout(grid);
     pvbox->addWidget(groupBox);

     if( this->td->isMultiSampleMode()) {
         qDebug() << "multisample";
         QGroupBox *sampleGroupBox = new QGroupBox(tr("Select Samples (to export sequences)"));
         sampleGroupBox->setFlat(true);
         QGridLayout *sampleGrid = new QGridLayout;
         for(unsigned int i=0; i < this->td->getSampleNames().size(); i++ ) {
             QCheckBox *checkBox1 = new QCheckBox(this->td->getSampleName(i));
             connect(checkBox1, SIGNAL( clicked()), this, SLOT(clickedChoice()) );
             sampleGrid->addWidget(checkBox1,(int)i/5, i%5);
         }
         sampleGroupBox->setLayout(sampleGrid);
         pvbox->addWidget(sampleGroupBox);
     }

     exportFormat = new QGroupBox(tr("Export Format"));
     tsvRadio = new QCheckBox(tr("tsv"));
     csvRadio = new QCheckBox(tr("csv"));
     tsvRadio->setChecked(true);

     vbox = new QHBoxLayout;
     vbox->addWidget(tsvRadio);
     vbox->addWidget(csvRadio);

     fasta = new QCheckBox(tr("Contigs"));
     fna = new QCheckBox(tr("ORFs"));
     faa = new QCheckBox(tr("Amino"));

     vbox->addWidget(fasta);
     vbox->addWidget(fna);
     vbox->addWidget(faa);


     vbox->addStretch(1);
     exportFormat->setLayout(vbox);
     pvbox->addWidget(exportFormat, this->td->getHeaders().size(), 0);
     cancelButton = new QPushButton("Cancel");
     exportButton = new QPushButton("Export");
     vbox->addWidget(cancelButton);
     vbox->addWidget(exportButton);


     return groupBox;
 }


 ExportBox::~ExportBox()
{

     /*
     delete  this->exportFormat;
     delete  this->tsvRadio;
     delete this->csvRadio;
     delete this->vbox;
     delete this->cancelButton;
     delete this->exportButton;
     */
     qDebug() << " Export box closed ";

 }


typedef struct _EXPORT_FILES_INFO {
    QList<QString>  suffixes;
    QList<QCheckBox *> checkBoxes;
    QList<RESOURCE> resources;

} EXPORT_FILES_INFO;

 bool ExportBox::saveAs()
 {
     QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"));
     QString exportFileName;
     QChar delim;

     if (fileName.isEmpty())
         return false;

     QList<QRegExp> invalidSuffixes;
     invalidSuffixes << QRegExp("[.].csv$") << QRegExp("[.].txt$") << QRegExp("[.].fasta$")\
                     << QRegExp("[.].faa$") << QRegExp("[.].fna$");




     foreach( QRegExp reg, invalidSuffixes) {
        if( fileName.indexOf(reg) >= 0  ) {
            QMessageBox msg;
            msg.warning(0,"Invalid file name to export !", "File/Folder name should not end in  .txt, .csv, .faa, .fna and .fasta ", QMessageBox::Ok);
            return false;
        }
     }


     if( csvRadio->isChecked()) {
         exportFileName = fileName + QString(".csv");
         delim=QChar(',');
         QLabel *waitScreen = Utilities::ShowWaitScreen(QString("Exporting the table to file ") + exportFileName + QString("!"));
         this->td->saveTableToFile(exportFileName, delim);
         waitScreen->hide();
     }

     if( tsvRadio->isChecked()) {
         exportFileName = fileName + QString(".txt");
         delim=QChar('\t');
         QLabel *waitScreen = Utilities::ShowWaitScreen(QString("Exporting the table to file ") + exportFileName + QString("!"));
         this->td->saveTableToFile(exportFileName, delim);
         waitScreen->hide();
     }
     EXPORT_FILES_INFO filesInfo;
     filesInfo.checkBoxes << fasta << fna << faa;
     filesInfo.suffixes << ".fasta" << ".fna" << ".faa";
     filesInfo.resources << NUCFASTA << NUCFNA << AMINOFAA;

     if( this->td->isMultiSampleMode() ) {
         QDir dir(fileName);
         if( !dir.exists() ) dir.mkpath(fileName);
         foreach( QString sampleName, this->td->getSampleNames())
             for(unsigned int i = 0; i < filesInfo.suffixes.size(); i++ ) {
                 if( filesInfo.checkBoxes[i]->isChecked()) {
                     exportFileName = fileName +"/" + sampleName + QString(filesInfo.suffixes[i]);
                     QLabel *waitScreen = Utilities::ShowWaitScreen(QString("Exporting the table to file ") + exportFileName + QString("!"));
                     this->td->saveSequencesToFile(sampleName, exportFileName, filesInfo.resources[i]);
                     waitScreen->hide();
                 }
             }
     }else {
         for(unsigned int i = 0; i < filesInfo.suffixes.size(); i++ ) {
             foreach( QString sampleName, this->td->getSampleNames() )
               if( filesInfo.checkBoxes[i]->isChecked() ) {
                   exportFileName = fileName + QString(filesInfo.suffixes[i]);
                   QLabel *waitScreen = Utilities::ShowWaitScreen(QString("Exporting the table to file ") + exportFileName + QString("!"));
                   this->td->saveSequencesToFile(sampleName, exportFileName, filesInfo.resources[i]);
                   waitScreen->hide();
               }
         }
     }


     /*
     if( faa->isChecked()) {
         exportFileName = fileName + QString(".faa");
         QLabel *waitScreen = Utilities::ShowWaitScreen(QString("Exporting the table to file ") + exportFileName + QString("!"));
         this->td->saveSequencesToFile(exportFileName, AMINOFAA);
         waitScreen->hide();
     }


     if( fasta->isChecked()) {
         exportFileName = fileName + QString(".fasta");
         QLabel *waitScreen = Utilities::ShowWaitScreen(QString("Exporting the table to file ") + exportFileName + QString("!"));
         this->td->saveSequencesToFile(exportFileName, NUCFASTA);
         waitScreen->hide();
     }

     if( fna->isChecked()) {
         exportFileName = fileName + QString(".fna");
         QLabel *waitScreen = Utilities::ShowWaitScreen(QString("Exporting the table to file ") + exportFileName + QString("!"));
         this->td->saveSequencesToFile(exportFileName, NUCFNA);
         waitScreen->hide();
     }
*/
     this->hide();
     this->destroy();


     return true;
 }


