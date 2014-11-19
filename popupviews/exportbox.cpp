#include "exportbox.h"

ExportBox::ExportBox(TableData* td, QWidget *parent, TABLEEXPORTTYPE type)
    // : QWidget(parent)
{
    this->td = new ExportSource(td);
    this->exportType = type;

    this->createWidget();
}

ExportBox::ExportBox(HTableData* td, QWidget *parent)
    // : QWidget(parent)
{
    this->td = new ExportSource(td);
    this->exportType = OTHERSTABLEEXP;

   // this->setSizePolicy();
    this->createWidget();
}

void ExportBox::createWidget() {
     QVBoxLayout *vbox = new QVBoxLayout;
     createNonExclusiveGroup(vbox);
     setLayout(vbox);

     setWindowTitle(tr("Export"));

     this->setAttribute(Qt::WA_DeleteOnClose, true);
     connect(exportButton, SIGNAL(clicked()), this, SLOT(saveAs()) );
     connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelWindow()) );


     this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
     scroll = new QScrollArea;
     scroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
     scroll->setWidget(this);
     QSize wSize = this->size();

     int w = wSize.width() < 600 ? wSize.width() :600;
     int h = wSize.height() < 300 ? wSize.height() : 300;
     scroll->resize(w, h);
     scroll->setMaximumSize(wSize.width() + 10, wSize.height() + 10);
     scroll->show();

 }

void ExportBox::setAuxName(QString auxname) {
    this->auxName = auxname;
}

void ExportBox::setExportType(TABLEEXPORTTYPE type) {
    this->exportType = type;
}

bool ExportBox::cancelWindow() {
    this->scroll->close();
    this->close();
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
/*
     foreach(EXPORT_SELECT a, cols) {
         a.checkbox->setText( a.name + " [" + QString::number(a.rank + 1 ) + "]");
     }
     */

 }


QStringList ExportBox::getCheckedHeaders() {
    QStringList selectedHeaders;

    for(unsigned int i=0; i < Columns.size(); i++)
        if(Columns[i].checkbox->isChecked() )
            selectedHeaders << Columns[i].name;

    return selectedHeaders;

 }


QStringList ExportBox::getCheckedSamples() {
    QStringList selectedSamples;

    foreach(QCheckBox *chbox, this->samplesCheckboxes)
        if( chbox->isChecked() )
            selectedSamples << chbox->text();

    return selectedSamples;

 }

QStringList ExportBox::getAllHeaders() {
    QStringList selectedHeaders;

    for(unsigned int i=0; i < Columns.size(); i++)
            selectedHeaders << Columns[i].name;

    return selectedHeaders;

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

     QGroupBox *control = new QGroupBox;
     QHBoxLayout *controlLayout = new QHBoxLayout;

     QPushButton *selectAll = new QPushButton("Select All");
     QPushButton *clearAll = new QPushButton("Clear All");
     controlLayout->addWidget(selectAll);
     controlLayout->addWidget(clearAll);
     control->setLayout(controlLayout);
     controlLayout->addStretch(1);

     groupBox->setLayout(grid);
     pvbox->addWidget(groupBox);
     pvbox->addWidget(control);
     connect(selectAll, SIGNAL(clicked()), this, SLOT(selectAllHeadersSlot()));
     connect(clearAll, SIGNAL(clicked()), this, SLOT(clearAllHeadersSlot()));


     /////////////////////////

     if( this->td->isMultiSampleMode()) {
         this->samplesCheckboxes.clear();
        // qDebug() << "multisample";
         QGroupBox *sampleGroupBox = new QGroupBox(tr("Select Samples (to export sequences)"));
         sampleGroupBox->setFlat(true);
         QGridLayout *sampleGrid = new QGridLayout;
         for(unsigned int i=0; i < this->td->getSampleNames().size(); i++ ) {
             QCheckBox *checkBox1 = new QCheckBox(this->td->getSampleName(i));
             connect(checkBox1, SIGNAL( clicked()), this, SLOT(clickedChoice()) );
             sampleGrid->addWidget(checkBox1,(int)i/5, i%5);
             this->samplesCheckboxes.append(checkBox1);
         }

         control = new QGroupBox;
         controlLayout = new QHBoxLayout;

         selectAll = new QPushButton("Select All");
         clearAll = new QPushButton("Clear All");
         controlLayout->addWidget(selectAll);
         controlLayout->addWidget(clearAll);
         control->setLayout(controlLayout);
         controlLayout->addStretch(1);

         sampleGroupBox->setLayout(sampleGrid);
         pvbox->addWidget(sampleGroupBox);
         pvbox->addWidget(control);

         connect(selectAll, SIGNAL(clicked()), this, SLOT(selectAllSamplesSlot()));
         connect(clearAll, SIGNAL(clicked()), this, SLOT(clearAllSamplesSlot()));
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
     megan = new QCheckBox(tr("MEGAN"));

     tsvRadio->setToolTip("Exports the table in the tsv format");
     csvRadio->setToolTip("Exports the table in the csv format");
     fasta->setToolTip("Exports the contigs (nucleotide sequences) where the ORFs originated");
     fna->setToolTip("Exports the ORFs (predicted gene coding regions of the nucleotide sequences)");
     faa->setToolTip("Exports the translated (in to amino acid sequences) ORFs ");
     megan->setToolTip("Exports the ORF-wise homology search results to further analyze using the MEGAN software");

     vbox->addWidget(fasta);
     vbox->addWidget(fna);
     vbox->addWidget(faa);
     vbox->addWidget(megan);


     if(this->exportType == rRNATABLEEXP) {
         fna->hide();
         faa->hide();
         fasta->setText(tr("rRNA"));
         fasta->setToolTip("Exports the region of the contigs that has a hit in the rRNA database");
     }
     else if(this->exportType == tRNATABLEEXP ) {
         fna->hide();
         faa->hide();
         fasta->setText(tr("tRNA"));
         fasta->setToolTip("Exports the region of the contigs that has the tRNA");
     }
     else {
         fasta->show();
         fna->show();
         faa->show();
         megan->show();
     }

     vbox->addStretch(1);
     exportFormat->setLayout(vbox);
     pvbox->addWidget(exportFormat, this->td->getHeaders().size(), 0);
     cancelButton = new QPushButton("Cancel");
     exportButton = new QPushButton("Export");
     vbox->addWidget(cancelButton);
     vbox->addWidget(exportButton);


     return groupBox;
 }

 void ExportBox::selectAllHeadersSlot() {
     this->setHeadersCheck(true);
 }

 void ExportBox::clearAllHeadersSlot() {
     this->setHeadersCheck(false);
 }


 void ExportBox::setHeadersCheck(bool state) {
     for(unsigned int i=0; i < Columns.size(); i++)
        Columns[i].checkbox->setChecked(state);
 }



 void ExportBox::selectAllSamplesSlot() {
     this->setSamplesCheck(true);
 }

 void ExportBox::clearAllSamplesSlot() {
     this->setSamplesCheck(false);
 }


 void ExportBox::setSamplesCheck(bool state) {
     foreach(QCheckBox *chbox, this->samplesCheckboxes)
        chbox->setChecked(state);
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
   //  qDebug() << " Export box closed ";

 }


typedef struct _EXPORT_FILES_INFO {
    QList<QString>  suffixes;
    QList<QCheckBox *> checkBoxes;
    QList<RESOURCE> resources;

} EXPORT_FILES_INFO;


 /**
  * @brief ExportBox::saveAs saves the selected table columns to txt or csv file and also save the
  * fna, fasta and faa sequences for the selected samples.
  * @return
  */
 bool ExportBox::saveAs()
 {
     QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"));
     QString exportFileName;
     QChar delim;
     QHash<QString, bool> sampleSequencesToExport;

     if (fileName.isEmpty())
         return false;

     QStringList selectedHeaders = this->getCheckedHeaders();
     if( selectedHeaders.size() ==0 ) {
         selectedHeaders = this->getAllHeaders();
     }

     // make a hash of the selected samples
     foreach(QString item,  this->getCheckedSamples()) {
         sampleSequencesToExport.insert(item, true);
     }

     if( csvRadio->isChecked()) {
         exportFileName = fileName.remove(QRegExp("[.]csv$") ) + QString(".csv");
         delim=QChar(',');
//         QLabel *waitScreen = Utilities::ShowWaitScreen(QString("Exporting the table to file ") + exportFileName + QString("!"));
         this->td->saveTableToFile(exportFileName, delim, selectedHeaders );
//         waitScreen->hide();
     }

     if( tsvRadio->isChecked()) {
         exportFileName = fileName.remove(QRegExp("[.]txt$") ) + QString(".txt");
         delim=QChar('\t');
//         QLabel *waitScreen = Utilities::ShowWaitScreen(QString("Exporting the table to file ") + exportFileName + QString("!"));
         this->td->saveTableToFile(exportFileName, delim, selectedHeaders);
//         waitScreen->hide();
     }

     EXPORT_FILES_INFO filesInfo;
     filesInfo.checkBoxes << fasta << fna << faa;
     filesInfo.suffixes << ".fasta" << ".fna" << ".faa";
     filesInfo.resources << NUCFASTA << NUCFNA << AMINOFAA;


     // export the sequences now
      QDir dir(fileName);



      foreach( QString sampleName, this->td->getSampleNames() ) {
      //iterate through each sample
          if( this->td->isMultiSampleMode() && !sampleSequencesToExport.contains(sampleName)) continue;  // skip the sample if it is not selected
             for(unsigned int i = 0; i < filesInfo.suffixes.size(); i++ ) {
                 if( filesInfo.checkBoxes[i]->isChecked()) {
                     if( !dir.exists() && fileName.compare(SELECT_SAMPLE_TAG)!=0) dir.mkpath(fileName);

                     if( i==0 ) { // the trna and rrna tables is treated separately since there only one of them
                          if(this->exportType==tRNATABLEEXP) {
                              exportFileName = fileName +"/" + sampleName + QString(".tRNA") +  QString(filesInfo.suffixes[i]);
                          }
                          else if(this->exportType==rRNATABLEEXP) {
                              exportFileName = fileName +"/" + sampleName + QString(".rRNA") + QString(".") + QString(this->auxName) +    QString(filesInfo.suffixes[i]);
                          }
                          else {  /* other tables */
                              exportFileName = fileName +"/" + sampleName +  QString(filesInfo.suffixes[i]);
                          }
                     }
                     else {
                        exportFileName = fileName +"/" + sampleName + QString(filesInfo.suffixes[i]);
                     }
//                     QLabel *waitScreen = Utilities::ShowWaitScreen(QString("Exporting the table to file ") + exportFileName + QString("!"));

                     this->td->saveSequencesToFile(sampleName, exportFileName, filesInfo.resources[i]);
                     //waitScreen->hide();
                 } //if the sequence type to export selected
             } // fasta, fna and faa files

             //here comes the megan
             if(this->megan->isChecked() ) {
                 RunData *rundata = RunData::getRunData();
                 if( !dir.exists() && fileName.compare(SELECT_SAMPLE_TAG)!=0) dir.mkpath(fileName);
                 exportFileName = fileName +"/" + sampleName + QString(".") + rundata->getValueFromHash("annotation:algorithm", _PARAMS) + QString(".MEGAN.txt");

                 if( rundata->getValueFromHash("annotation:algorithm", _PARAMS).compare("LAST")==0)
                     this->td->saveMeganExportToFile(sampleName, exportFileName, MEGANLASTFILE);
                 else if( rundata->getValueFromHash("annotation:algorithm", _PARAMS).compare("BLAST")==0)
                     this->td->saveMeganExportToFile(sampleName, exportFileName,MEGANBLASTFILE);
             }
      }  // samplewise for


     this->hide();
     this->scroll->close();
     this->destroy();



     return true;
 }


