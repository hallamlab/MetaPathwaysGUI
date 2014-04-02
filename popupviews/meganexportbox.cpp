#include "meganexportbox.h"

MeganExportBox::MeganExportBox(QWidget *parent) :
    QWidget(parent)
{
    createWidget();
    this->show();
}




void MeganExportBox::createWidget() {
     QHBoxLayout *Lbox = new QHBoxLayout;

     QGroupBox *exportFormat = new QGroupBox(tr("Export Format"));
     lastRadio = new QRadioButton(tr("LAST"));
     blastRadio = new QRadioButton(tr("BLAST"));
     lastRadio->setChecked(true);

     QHBoxLayout *hbox = new QHBoxLayout;
     hbox->addWidget(lastRadio);
     hbox->addWidget(blastRadio);

     hbox->addStretch(1);
     exportFormat->setLayout(hbox);


     aminoSequences = new QCheckBox(tr("Export Amino Acid Sequences"));
     hbox->addWidget(aminoSequences);

     QPushButton *cancelButton = new QPushButton("Cancel");
     QPushButton *exportButton = new QPushButton("Export");
     hbox->addWidget(cancelButton);
     hbox->addWidget(exportButton);

     Lbox->addWidget(exportFormat);

     this->setLayout(Lbox);
     setWindowTitle(tr("Megan Export"));
     resize(480, 200);
     this->setAttribute(Qt::WA_DeleteOnClose, true);


     connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()) );
     connect(exportButton, SIGNAL(clicked()), this, SLOT(exportMeganFiles()) );
 }

void MeganExportBox::exportMeganFiles() {

    this->hide();

    SampleResourceManager *sampleResourcesManager = SampleResourceManager::getSampleResourceManager();
    RunData *rundata = RunData::getRunData();

    QString meganFile;
    if(lastRadio->isChecked()) {
       meganFile = sampleResourcesManager->getFilePath(rundata->getCurrentSample(), MEGANLASTFILE);
    }
    else{
       meganFile = sampleResourcesManager->getFilePath(rundata->getCurrentSample(), MEGANBLASTFILE);
    }


    QFileInfo fileInfo(meganFile);
    QMessageBox msg;
    if( !fileInfo.exists()) {
        msg.warning(0,"Exportable megan file " + meganFile + " has not been generated yet.!", "Please make sure you have run the BLAST REFDB on Refseq database!", QMessageBox::Ok);
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"));

    if( lastRadio->isChecked()) {
        this->exportMeganTable(meganFile, fileName + QString(".txt"), MEGANLASTFILE);
    }
    else {
        this->exportMeganTable(meganFile, fileName + QString(".txt"), MEGANBLASTFILE);
    }

    if( aminoSequences->isChecked()) {
        QString meganFile =  sampleResourcesManager->getFilePath(rundata->getCurrentSample(), AMINOFAA);
        QFileInfo fileInfo(meganFile);
        QMessageBox msg;
        if( !fileInfo.exists()) {
            msg.warning(0,"Amino acid sequences " + meganFile + " has not been generated yet.!", "Please make sure you have run the ORF PREDICTION step!", QMessageBox::Ok);
            return;
        }
        this->exportMeganFile(meganFile, fileName + QString(".faa"));
    }


}

int  MeganExportBox::countLines(QString fileName) {
    QRegExp comment("^#");

    QFile file(fileName);
    int count =0;
    if(file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while ( !in.atEnd() ){
           QString  line = in.readLine().trimmed();
           if( line.indexOf(comment) == -1)   count++;
        }
        file.close();
    }
    return count;
}


QString  MeganExportBox::lastToBlastFormat(QString line, QChar &sep) {
    QString blastFormat;
    QStringList modifiedList;


    QStringList fields = line.split(sep);

    unsigned int queryEnd;
    unsigned int subjectEnd;

    bool ok1 , ok2;

    fields[2].toInt(&ok1);
    fields[3].toInt(&ok2);
    if( ok1 && ok2) {
        subjectEnd  = fields[2].toInt() + fields[3].toInt() - 1;
    }
    else {
        subjectEnd = 0;
    }

    fields[7].toInt(&ok1);
    fields[8].toInt(&ok2);
    if( ok1 && ok2) {
        queryEnd  = fields[7].toInt() + fields[8].toInt() - 1;
    }
    else {
        queryEnd = 0;
    }



    modifiedList << fields[6] << fields[1] << QString("0") << fields[8]\
                << QString("0") << QString("0") << fields[7] << QString::number(queryEnd, 10) \
                << fields[2] << QString::number(subjectEnd, 10) << QString("0") << fields[0];


    blastFormat = modifiedList.join(sep);

    return blastFormat;
}


void MeganExportBox::exportMeganTable(const QString & meganFile, const QString &fileName, RESOURCE resourceType) {


    ProgressView progressBar1("Please wait while we compute the file size!", 0, 0);
    int numHits = this->countLines(meganFile);
    progressBar1.hide();

    unsigned int onePercent = numHits/100;


    ProgressView progressBar("Please wait while we save the file!", 0, 100);
    QRegExp comment("^#");
    QChar sep('\t');

    QFile inFile(meganFile);
    QFile outFile(fileName);

    int count=0;
    if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outFile);
        if(inFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&inFile);
            while ( !in.atEnd() ){
               QString line = in.readLine().trimmed();
               if( line.indexOf(comment) != -1) continue;
               count++;

               if( count%onePercent==0) {
                   progressBar.updateprogress((unsigned int)( (count*100)/numHits) );
              //     qDebug() << "Percentage completed "<< "  " << (unsigned int)(count/numHits*100)  ;
               }
               QStringList fields = line.split(sep);
               if( fields.size()!= 12) continue;
               QString inBlastFormat;
               if( resourceType == MEGANLASTFILE)
                  inBlastFormat = this->lastToBlastFormat(line, sep);
               else
                  inBlastFormat = line;

               out << inBlastFormat + "\n";
            }
            inFile.close();
        }
        outFile.close();
    }
    progressBar.hide();
}

void MeganExportBox::exportMeganFile(const QString & meganFile, const QString &fileName) {

    ProgressView progressBar1("Please wait while we compute the file size!", 0, 0);
    int numHits = this->countLines(meganFile);
    progressBar1.hide();
    unsigned int onePercent = numHits/100;
    this->hide();
    ProgressView progressBar("Please wait while we save the file!", 0, 100);

    QFile inFile(meganFile);
    QFile outFile(fileName);

    int count=0;
    if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outFile);
        if(inFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&inFile);
            while ( !in.atEnd() ){
               QString line = in.readLine().trimmed();
               count++;

               if( count%onePercent==0) {
                   progressBar.updateprogress((unsigned int)( (count*100)/numHits) );
              //     qDebug() << "Percentage completed "<< "  " << (unsigned int)(count/numHits*100)  ;
               }
               out << line + "\n";
            }
            inFile.close();
        }
        outFile.close();
    }
    progressBar.hide();
}



 MeganExportBox::~MeganExportBox()
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


