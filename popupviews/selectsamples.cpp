#include "selectsamples.h"
#include "ui_selectsamples.h"

SelectSamples::SelectSamples(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectSamples)
{
    ui->setupUi(this);
    sampleGrid = this->findChild<QGridLayout *>("sampleGrid");
    decisionBox = this->findChild<QDialogButtonBox *>("decisionBox");

    selectAll = this->findChild<QPushButton *>("selectAll");
    clearAll = this->findChild<QPushButton *>("clearAll");

    connect(selectAll, SIGNAL(clicked()), this, SLOT(selectAllSlot()));
    connect(clearAll, SIGNAL(clicked()), this, SLOT(clearAllSlot()));
    connect(decisionBox, SIGNAL(accepted()), this, SLOT(sendSelection()) );
    connect(decisionBox, SIGNAL(rejected()), this, SLOT(closeOnCancel()) );

    this->resultWindow = 0;
    this->runconfigWindow = 0;

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    scroll = new QScrollArea;
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    scroll->setWidget(this);
    QSize wSize = this->size();
    int w = wSize.width() < 600 ? wSize.width() :600;
    int h = wSize.height() < 300 ? wSize.height() : 300;
    scroll->resize(w + 5, h + 5);
    scroll->setMaximumSize(wSize.width() + 10, wSize.height() + 10);
    scroll->show();

}


/**
 * @brief SelectSamples::closeWidget, close the widget
 */
void SelectSamples::closeWidget() {
  this->close();
  this->destroy();
  scroll->close();
}

/**
 * @brief SelectSamples::closeOnCancel, close if the cancel buttonis hit
 */
void SelectSamples::closeOnCancel() {
    this->closeWidget();
}

/**
 * @brief SelectSamples::resizeWidget, resize the widget after add the samples
 *
 */
void SelectSamples::resizeWidget() {
  this->adjustSize();
  this->show();


//  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
  //scroll = new QScrollArea;
//  scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

  QSize wSize = this->size();
  int w = wSize.width() < 600 ? wSize.width() :600;
  int h = wSize.height() < 300 ? wSize.height() : 300;
  scroll->resize(w + 5, h + 5);
  scroll->setMaximumSize(wSize.width() + 10, wSize.height() + 10);
  scroll->show();
}


SelectSamples::~SelectSamples()
{
    delete ui;
}


/**
 * @brief SelectSamples::addSamplesUnchecked, add the list of samples unchecked
 * @param samples, the samples to list
 */
void SelectSamples::addSamplesUnchecked(QStringList samples) {
   this->samples = samples;

   for(unsigned int i =0; i < checkboxes.size(); i++) {
       sampleGrid->removeWidget(checkboxes[i]);
   }

   QSignalMapper *signalMapper = new QSignalMapper(this);
   connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(sampleClicked(int)));

   checkboxes.clear();
   for(unsigned int i=0; i < samples.size(); i++ ) {
        QCheckBox *checkBox = new QCheckBox(this->samples.at(i));
        sampleGrid->addWidget(checkBox,(int)i/5, i%5);
        checkBox->setChecked(false);

        checkboxes.append(checkBox);
        signalMapper->setMapping(checkboxes[i], i);
        connect(checkboxes[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        checkBox->show();
   }
   this->resizeWidget();
}


/**
 * @brief SelectSamples::addSamples, add the list of sample names with the already
 * selected samples, checked
 * @param samples, the list of samples
 */
void SelectSamples::addSamples(QStringList samples) {
   this->samples = samples;

   for(unsigned int i =0; i < checkboxes.size(); i++) {
       sampleGrid->removeWidget(checkboxes[i]);
   }

   RunData *rundata = RunData::getRunData();
   QHash<QString, bool> alreadySelectedSamples;
   foreach(QString samplename, rundata->getSamplesSubsetToRun() ) {
       alreadySelectedSamples[samplename] = true;
   }

   QSignalMapper *signalMapper = new QSignalMapper(this);
   connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(sampleClicked(int)));

   checkboxes.clear();
   for(unsigned int i=0; i < samples.size(); i++ ) {
        QCheckBox *checkBox = new QCheckBox(this->samples.at(i));

        if( this->samples.at(i).contains(QRegExp(QString("^[^a-zA-Z]")))  || this->samples.at(i).contains(QString(".")) || this->samples.at(i).length()<=1  ) {
            QPalette p  = checkBox->palette();
            p.setColor( QPalette::Active, QPalette::WindowText, QColor(Qt::red) );
            checkBox->setPalette(p);
            checkBox->setToolTip(QString("Sample names (before suffixes fasta, fas, gbk, etc) beginning with non-alphabets, containing \".\"s, or consisting of one character are not processed"));
            checkBox->setCheckable(false);
        }


        sampleGrid->addWidget(checkBox,(int)i/5, i%5);
        if(alreadySelectedSamples.contains(this->samples.at(i))) {
            checkBox->setChecked(true);
        }
        checkboxes.append(checkBox);
        signalMapper->setMapping(checkboxes[i], i);
        connect(checkboxes[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        checkBox->show();
   }
   this->resizeWidget();
}


void SelectSamples::sampleClicked(int i) {
  if(checkboxes[i]->isChecked())
    this->clickedSamples.append(i);
  else
    this->clickedSamples.removeOne(i);
}

void SelectSamples::selectAllSlot() {
   unsigned int i =0;
   this->clickedSamples.clear();
   foreach(QCheckBox *decision, checkboxes) {
       decision->setChecked(true);
       this->clickedSamples.append(i);
       i++;
   }
}

void SelectSamples::clearAllSlot() {
    foreach(QCheckBox *decision, checkboxes) {
       decision->setChecked(false);
    }
    this->clickedSamples.clear();

}

void SelectSamples::setReceiver(ResultWindow *resultWindow  ) {
    this->resultWindow = resultWindow;
}

void SelectSamples::setReceiver(RunConfig *runconfigWindow  ) {
    this->runconfigWindow    = runconfigWindow;
}


/**
 * @brief SelectSamples::sendSelection, this sends the signal after
 * some samples are selected from the Stages panel
 */
void SelectSamples::sendSelection() {

  // this if for sending to the resultWindow
  QList<unsigned int> selection;
   if(this->resultWindow!=0) {
     foreach(QCheckBox *decision, checkboxes) {
           selection.append(decision->checkState());
     }
     //this->resultWindow->receiveSelection(selection);
     scroll->hide();
     this->resultWindow->receiveSelection(this->clickedSamples);
   }

   // this is for the runconfig, i.e., the set of samples to run
   QList<QString> selectedSamples;
   if( this->runconfigWindow!=0) {
       unsigned int i =0;
       foreach(QCheckBox *decision, checkboxes) {
           if(decision->isChecked())  selectedSamples.append(this->samples.at(i));
           i++;
       }
       scroll->hide();
       this->runconfigWindow->receiveSelection(selectedSamples);
   }

   this->closeWidget();

}
