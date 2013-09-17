#ifndef GENOMEVIEW_H
#define GENOMEVIEW_H

#include <QGraphicsView>
class GeneBrowser;

class GenomeView : public QGraphicsView
{
public:
    GenomeView();

private:
     virtual void wheelEvent(QWheelEvent* event);

    GeneBrowser *gBrowser;
    unsigned int XSIZE;
    unsigned int YSIZE;
};

#endif // GENOMEVIEW_H
