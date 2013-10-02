#include "selectsamples.h"
#include "ui_selectsamples.h"

SelectSamples::SelectSamples(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectSamples)
{
    ui->setupUi(this);
    sampleGrid = this->findChild<QGridLayout *>("sampleGrid");
    decisionBox = this->findChild<QDialogButtonBox *>("decisionBox");
    connect(decisionBox, SIGNAL(accepted()), this, SLOT(sendSelection()) );

}

SelectSamples::~SelectSamples()
{
    delete ui;
}


void SelectSamples::addSamples(QStringList samples) {
   checkboxes.clear();
   for(unsigned int i=0; i < samples.size(); i++ ) {
        QCheckBox *checkBox = new QCheckBox(samples.at(i));
        sampleGrid->addWidget(checkBox,(int)i/1, i%1);
        checkboxes.append(checkBox);
   }
}


void SelectSamples::setReceiver(ResultWindow *resultWindow  ) {
    this->resultWindow = resultWindow;
}


void SelectSamples::sendSelection() {
   QList<bool> selection;
   foreach(QCheckBox *decision, checkboxes) {
       selection.append(decision->checkState());
   }
   this->resultWindow->receiveSelection(selection);

}
