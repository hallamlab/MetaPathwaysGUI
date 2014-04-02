#include "mainframe.h"
#include <QApplication>
#include <QString>
#include <QSettings>
#include <QDebug>

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

    QApplication a(argc, argv);
    MainFrame *w = new MainFrame();
    w->show();
    a.exec();
}
