#include "mainframe.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Resources);

    QApplication a(argc, argv);
    MainFrame *w = new MainFrame();
    w->show();
    a.exec();
}
