#include "mdiareawidget.h"

MdiAreaWidget *MdiAreaWidget::mdiAreaWidget =0;

MdiAreaWidget::MdiAreaWidget()
{
}


MdiAreaWidget *MdiAreaWidget::getMdiAreaWidget() {
    if( MdiAreaWidget::mdiAreaWidget==0)
        MdiAreaWidget::mdiAreaWidget = new MdiAreaWidget();

    return MdiAreaWidget::mdiAreaWidget;
}


void MdiAreaWidget::setMdiArea(QMdiArea *mdi) {
    this->mdiArea = mdi;
}

QMdiArea *MdiAreaWidget::getMdiArea() {

    return this->mdiArea;

}


void MdiAreaWidget::addWidget(HTableData *w) {
    this->mdiArea->addSubWindow(w);
    subWindows[w] = true;
    this->mdiArea->cascadeSubWindows();
}


void MdiAreaWidget::removeWidget(HTableData *w) {
    if( subWindows.contains(w)) {
     //   this->mdiArea->removeSubWindow(w);
        QMdiSubWindow* QMdiChild = qobject_cast<QMdiSubWindow*>(w->parentWidget());
        QMdiChild->close();
        this->subWindows.remove(w);
    }
}
