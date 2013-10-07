#include "toolbarmanager.h"


ToolBarManager *ToolBarManager::toolbarmanager = 0;

ToolBarManager *ToolBarManager::getToolBarManager() {
    if(ToolBarManager::toolbarmanager==0)
        ToolBarManager::toolbarmanager = new ToolBarManager();

    return ToolBarManager::toolbarmanager;
}


ToolBarManager::ToolBarManager()
{
}


void ToolBarManager::setToolBar(QToolBar *toolbar ) {
   this->toolbar = toolbar;
}

void ToolBarManager::addTab(HTabWidget *htab, HTableData *htable ) {
    QAction *a = this->toolbar->addWidget(htab);

    connect(htab, SIGNAL(tabClicked(HTabWidget *,SIGNALTYPE)), this, SLOT(tabClicked(HTabWidget *, SIGNALTYPE)) );

    widgets.insert(htab, a);
    tables.insert(htab, htable);
}

void ToolBarManager::removeTab(HTabWidget *htab) {
    if( widgets.contains(htab)) {
       this->toolbar->removeAction( widgets[htab]);
       this->widgets.remove(htab);
     }
}


void ToolBarManager::tabClicked(HTabWidget *htab, SIGNALTYPE type) {
   if(type==CROSS) {
       this->removeTab(htab);
       this->tables[htab]->hide();
       MdiAreaWidget *mdiAreaWidget = MdiAreaWidget::getMdiAreaWidget();
       mdiAreaWidget->removeWidget(this->tables[htab]);
   }

}

