#include "parentwidget.h"
#include "ui_parentwidget.h"
#include "utilities.h"
#include "ProgressDialog.h"
#include "resultwindow.h"
#include "resultpage.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QDebug>
#include <QProcess>
#include <QDockWidget>
#include <QDebug>

ParentWidget::ParentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ParentWidget)
{
    ui->setupUi(this);

    cancelButton = this->findChild<QPushButton *>("cancelButton");
    backButton = this->findChild<QPushButton *>("backButton");

    continueButton->setEnabled(false);
    backButton->hide();

    settingsTab = new SettingsTab(this);
    runConfigTab = new RunConfig(this);

    tab = ui->parentTabWidget;
    tab->clear();
    //delete initial

    connect(continueButton, SIGNAL(clicked()), this, SLOT(continueButtonPressed()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonPressed()));
    connect(backButton, SIGNAL(clicked()), this, SLOT(backButtonPressed()));
    connect(tab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged()));
//    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void ParentWidget::hideContinueButton(){
    continueButton->setDisabled(true);
}


void ParentWidget::tabChanged(){
    if (tab->currentWidget() == runConfigTab){
        backButton->show();
    }else{
        backButton->hide();
    }
    if (tab->currentWidget() == runConfigTab && runConfigTab->selectedFiles == 0){
        continueButton->setEnabled(false);
    }else{
        continueButton->setEnabled(true);
    }
}

void ParentWidget::backButtonPressed(){
    if(tab->currentWidget() == runConfigTab){
        tab->setCurrentWidget(settingsTab);
    }
}



/*
void ParentWidget::executionPrep(){
    //copy the current CONFIG, PARAMS pointers to a newly instantiated RunData instance
    run = RunData::getRunData();


    //set the rrnaDB and annotationDBS so we know how many to look for in the log
    QStringList* rrnaDBS;
    QString rdbs = MainFrame::PARAMS["rRNA:refdbs"];
    rrnaDBS = new QStringList(rdbs.split(","));
    rrnaDBS->removeAll("");

    QStringList* annotationDBS;
    QString adbs = MainFrame::PARAMS["annotation:dbs"];
    annotationDBS = new QStringList(adbs.split(","));
    annotationDBS->removeAll("");

    QStringList *uniqueRRNADBS = new QStringList();
    QStringList *uniqueADBS = new QStringList();

    Utilities::getUniqueDBS(*rrnaDBS, uniqueRRNADBS);
    Utilities::getUniqueDBS(*annotationDBS, uniqueADBS);

    rrnaDBS = uniqueRRNADBS;
    annotationDBS = uniqueADBS;

    run->nADB = annotationDBS->size();
    run->nRRNADB = rrnaDBS->size();

    run->setAnnotationDBS(annotationDBS);
    run->setRRNADBS(rrnaDBS);

    emit continueFromParentSettings();
    //mit showResultsFromParentSettings();

//    ProgressDialog *progress = new ProgressDialog(this);
//    ResultWindow *rw = new ResultWindow(progress);

    //reset MainFrame static instances
//    MainFrame::CONFIG = Utilities::parseFile(MainFrame::DEFAULT_TEMPLATE_CONFIG);
//    MainFrame::PARAMS = Utilities::parseFile(MainFrame::DEFAULT_TEMPLATE_PARAM);
}
*/

void ParentWidget::cancelButtonPressed(){
//    close();
//    this->parentWidget()->close();
}

ParentWidget::~ParentWidget()
{
    delete ui;
}
