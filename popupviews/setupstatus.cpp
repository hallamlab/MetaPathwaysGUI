#include "setupstatus.h"

SetUpStatus::SetUpStatus(QStringList messages, QWidget *parent )
{
    this->messages = messages;
    this->parent = parent;
    this->createWidget();
}



void SetUpStatus::createWidget( ) {

    // createNonExclusiveGroup(vbox);
     this->addMessage();

     setWindowTitle(tr("Setup Status"));

     this->setAttribute(Qt::WA_DeleteOnClose, true);
 //    connect(exportButton, SIGNAL(clicked()), this, SLOT(saveAs()) );
  //   connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelWindow()) );


     this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
     scroll = new QScrollArea;
     scroll->setWindowTitle("Setup Config Status");
     scroll->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
     scroll->setWidget(this);
     QSize wSize = this->size();

     int w = wSize.width() < 600 ? wSize.width() :600;
     int h = wSize.height() < 300 ? wSize.height() : 300;
     scroll->resize(w, h);
     scroll->setMaximumSize(wSize.width() + 10, wSize.height() + 10);
     scroll->show();

 }


void SetUpStatus::addMessage()
{
    QVBoxLayout *pvbox = new QVBoxLayout;

    QFont font = QLabel().font();
    font.setPointSize(20);
    font.setBold(true);

    QGroupBox *groupbox1 = new QGroupBox;
    QVBoxLayout *python = new QVBoxLayout;
    QLabel *header1 = new QLabel("Python Executable");
    header1->setFont(font);
    QLabel *message1 = new QLabel( messages[3]);
    python->addWidget(header1);
    python->addWidget(message1);
    groupbox1->setLayout(python);

    QGroupBox *groupbox2 = new QGroupBox;
    QVBoxLayout *metapathways = new QVBoxLayout;
    QLabel *header2 = new QLabel("MetaPathways Directory"); header2->setFont(font);
    QLabel *message2 = new QLabel( messages[0]);
    metapathways->addWidget(header2);
    metapathways->addWidget(message2);
    groupbox2->setLayout(metapathways);

    QGroupBox *groupbox3 = new QGroupBox;
    QVBoxLayout *executables = new QVBoxLayout;
    QLabel *header3 = new QLabel("Dir for OS specific executables : "); header3->setFont(font);
    QLabel *message3 = new QLabel( messages[2]);
    executables->addWidget(header3);
    executables->addWidget(message3);
    groupbox3->setLayout(executables);

    QGroupBox *groupbox4 = new QGroupBox;
    QVBoxLayout *databases = new QVBoxLayout;
    QLabel *header4 = new QLabel("Databases directory :"); header4->setFont(font);
    QLabel *message4 = new QLabel( messages[1]);
    databases->addWidget(header4);
    databases->addWidget(message4);
    groupbox4->setLayout(databases);


    QGroupBox *groupbox5 = new QGroupBox;
    QVBoxLayout *pgdbs = new QVBoxLayout;
    QLabel *header5 = new QLabel("PGDB Folder Path :"); header5->setFont(font);
    QLabel *message5 = new QLabel( messages[4]);
    pgdbs->addWidget(header5);
    pgdbs->addWidget(message5);
    groupbox5->setLayout(pgdbs);


    QGroupBox *groupbox6 = new QGroupBox;
    QVBoxLayout *pathwaytools = new QVBoxLayout;
    QLabel *header6 = new QLabel("Pathway-Tools executables :"); header6->setFont(font);
    QLabel *message6 = new QLabel( messages[5]);
    pathwaytools->addWidget(header6);
    pathwaytools->addWidget(message6);
    groupbox6->setLayout(pathwaytools);


    QGroupBox *groupbox = new QGroupBox;
    QHBoxLayout* vbox = new QHBoxLayout;
    QPushButton * cancelButton = new QPushButton("Cancel");
    QPushButton *saveButton = new QPushButton("Save");
    QSpacerItem * horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    vbox->addItem(horizontalSpacer);
    vbox->addWidget(cancelButton);
    vbox->addWidget(saveButton);
    groupbox->setLayout(vbox);


    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveConfig() ) );
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(destroy()));

    pvbox->addWidget(groupbox2);
    pvbox->addWidget(groupbox4);

    pvbox->addWidget(groupbox3);
    pvbox->addWidget(groupbox1);

    pvbox->addWidget(groupbox5);
    pvbox->addWidget(groupbox6);
    pvbox->addWidget(groupbox);
    this->setLayout(pvbox);
}

void SetUpStatus::destroy() {
    this->close();
    scroll->close();
}

void SetUpStatus::saveConfig() {
    emit saveConfigFile();
    this->destroy();
}


