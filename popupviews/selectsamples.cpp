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
    this->resultWindow = 0;
    this->runconfigWindow = 0;

}

SelectSamples::~SelectSamples()
{
    delete ui;
}


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

   checkboxes.clear();
   for(unsigned int i=0; i < samples.size(); i++ ) {
        QCheckBox *checkBox = new QCheckBox(this->samples.at(i));
        sampleGrid->addWidget(checkBox,(int)i/5, i%5);
        if(alreadySelectedSamples.contains(this->samples.at(i))) {
            checkBox->setChecked(true);
        }
        checkboxes.append(checkBox);
   }
}

void SelectSamples::selectAllSlot() {
    foreach(QCheckBox *decision, checkboxes) {
       decision->setChecked(true);
    }
}

void SelectSamples::clearAllSlot() {
    foreach(QCheckBox *decision, checkboxes) {
       decision->setChecked(false);
    }

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
   QList<bool> selection;
   if(this->resultWindow!=0) {
     foreach(QCheckBox *decision, checkboxes) {
           selection.append(decision->checkState());
     }
     this->resultWindow->receiveSelection(selection);
   }

   QList<QString> selectedSamples;
   if( this->runconfigWindow!=0) {
       unsigned int i =0;
       foreach(QCheckBox *decision, checkboxes) {
           if(decision->isChecked())  selectedSamples.append(this->samples.at(i));
           i++;
       }
      // qDebug() << selectedSamples;
       this->runconfigWindow->receiveSelection(selectedSamples);
   }
}
