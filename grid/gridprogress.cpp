#include "gridprogress.h"

GridProgress::GridProgress(QWidget *parent) :
    QWidget(parent)
{


    QHash<QString, QList<unsigned int> > data;
    data["server1"] = QList<unsigned int>();
    data["server1"] << 50 << 10 << 40;
    data["server2"] = QList<unsigned int>();
    data["server2"] << 52 << 11 << 41;

    serverData << data;
    data.clear();

    data["server1"] = QList<unsigned int>();
    data["server1"] << 250 << 110 << 140;
    data["server2"] = QList<unsigned int>();
    data["server2"] << 252 << 111 << 141;
    data["serve3"] = QList<unsigned int>();
    data["serve3"] << 254 << 112 << 142;
    data["serve4"] = QList<unsigned int>();
    data["serve4"] << 23 << 11 << 12;
    serverData << data;
    data.clear();

    /////////////////////
    data["server1"] = QList<unsigned int>();
    data["server1"] << 0 << 10;

    data["server2"] = QList<unsigned int>();
    data["server2"] << 5 << 0;
    transitionData << data;
    data.clear();

    data["server1"] = QList<unsigned int>();
    data["server1"] << 0 << 110 << 140 << 7;
    data["server2"] = QList<unsigned int>();
    data["server2"] << 152 << 0 << 141 << 3;
    data["serve3"] = QList<unsigned int>();
    data["serve3"] << 154 << 112 << 0<< 7;
    data["serve4"] = QList<unsigned int>();
    data["serve4"] << 2 << 12 << 0 << 3;
    transitionData << data;


    createEmptyWidget();

    timer = new QTimer();
    timer->start(3000);

    counter =0;
    connect(timer, SIGNAL(timeout()), this, SLOT(updatServerStats())  );
    this->show();
}


void GridProgress::updatServerStats() {
     counter = 2;
     if(!this->hasServersChanged()) return;

     foreach(QWidget *w, serverStatsLabels) {
         gridStates->removeWidget(w);
         delete w;
     }
     serverStatsLabels.clear();
     serverNames.clear();
     this->addServerStats();

}


void GridProgress::addLabel(QGridLayout *layout, QString text, unsigned int row, unsigned int col) {
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setBold(true);
    font.setWeight(1);
    font.setPointSize(20);

    QLabel *label = new QLabel();
    label->setFont( font);
    //label->setText( QString("%1").arg(text,16));
        label->setText( text);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, row, col);
    serverStatsLabels.append(label);
}

void GridProgress::addServerStats() {
    QLabel *label;
    unsigned int i =0,j;
    QFont font;
    font.setFamily(font.defaultFamily());
    font.setBold(true);
    font.setWeight(1);
    font.setPointSize(20);

    QStringList headers;
    headers << QString("") << "Submitted " << "Pending" << "Completed";

    i = 0;
    j = 0;
    foreach(QString header, headers) {
        this->addLabel(gridStates, header, i, j++);
    }

    i++;
    foreach(QString name1, serverData[(counter%3 +1)%2].keys()) {
        j=0;
        this->addLabel(gridStates, name1, i, j++);
        for(int k=0; k < 3; k++ ) {
            this->addLabel(gridStates, QString::number(serverData[(counter%3 +1)%2][name1][k]), i, j++);
        }
        i++;
    }


    j =0;
    this->addLabel(transitionMatrix, QString(), i, j++);
    foreach(QString name1, serverData[ (counter%3 +1)%2].keys())
        this->addLabel(transitionMatrix, name1, i, j++);
    i++;

    foreach(QString name1,  serverData[ (counter%3 +1)%2].keys()) {
        j =0;
        this->addLabel(transitionMatrix, name1, i, j++);

        foreach(QString name2, serverData[ (counter%3 +1)%2].keys()) {
           this->addLabel(transitionMatrix, QString::number(transitionData[(counter%3 +1)%2][name1][0]), i, j++);
        }
        i++;
    }

}


void GridProgress::createEmptyWidget() {
     Vbox = new QVBoxLayout;
     gridStates = new QGridLayout;
     transitionMatrix = new QGridLayout;

     Vbox->addStretch(1);
     Vbox->addLayout(gridStates);
     Vbox->setSpacing(2);
     Vbox->addSpacerItem(new QSpacerItem(0,10));
     Vbox->addLayout(transitionMatrix);
     Vbox->addStretch(1);
     Vbox->setAlignment(Qt::AlignCenter);
     this->setLayout(Vbox);


     this->setAttribute(Qt::WA_DeleteOnClose, true);

 }


bool GridProgress::hasServersChanged() {
    QSet<QString> newServerNames = QSet<QString>::fromList(serverData[ (counter%3 +1)%2].keys());
    QSet<QString> oldServerNames = QSet<QString>::fromList(serverNames);
    return !(newServerNames==oldServerNames);

}




 GridProgress::~GridProgress()
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


