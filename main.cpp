#include "mainframe.h"
#include <QApplication>
#include <QString>
#include <QSettings>
#include <QDebug>

#include "datamodel/lcastar.h"

#undef MEGAN_VIEW

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Resources);

    // if purge is given as an argument to MetaPathways2, it'll delete the old settings
    // use this as a failsafe
    if(argc > 1){
        QString purge(argv[1]);
        if (purge.compare("purge")==0){
            QSettings settings("HallamLab", "MetaPathways");
            settings.clear();
        }
    }


/*
    LCAStar *lcastr = LCAStar::getLCAStar();
    lcastr->setParameters(1,1,0.90);
    qDebug() << "loading ";
    QStringList taxonlist;
    taxonlist << QString("Deltaproteobacteria") << QString("Deltaproteobacteria") << QString("Alphaproteobacteria") <<
                  QString("Betaproteobacteria") << QString("Betaproteobacteria") <<  QString("Chloroflexi") << QString("green plants");;
    QString lcastartaxon = lcastr->lca_star(taxonlist);
    qDebug() << " The result is : " << lcastartaxon;
    qDebug() << "Loaded ";
  */


  QApplication a(argc, argv);
    MainFrame *w = new MainFrame();

    w->show();
    a.exec();
}
