#include "hierarchicalview.h"


HierarchicalView *HierarchicalView::hierarchicalview =0;
QHash<QString, QString> *HierarchicalView::history = 0;

HierarchicalView *HierarchicalView::getHierarchicalView() {
    if(HierarchicalView::hierarchicalview==0)
        HierarchicalView::hierarchicalview = new HierarchicalView;
    return HierarchicalView::hierarchicalview;
}

HierarchicalView::HierarchicalView(QWidget *parent) :
    QWidget(parent)
{
    this->resultWindow = 0;

}


QStandardItem * HierarchicalView::getQStandardItem(const QString &name) {
    QStandardItem *standarditem = new QStandardItem(name);
    _createdItemList.append(standarditem);
    return standarditem;
}


void HierarchicalView::selectTree(QStandardItem *headitem, QList< QStringPair>  &selectSamples, bool checked, QString category) {


    if(category.isEmpty() && headitem->checkState() && headitem->hasChildren()) {
        QRegExp num("\\s+\\(\\d+\\)$");
        category = headitem->text().replace(num, QString(""));
    }

    if( !headitem->hasChildren() && ( checked || headitem->checkState()) ) return selectSamples.append( QPair<QString, QString>(headitem->text(), category) );


    for(int j =0; j < headitem->rowCount(); j++) {
       this->selectTree(headitem->child(j,0), selectSamples, (checked || headitem->checkState()), category );
    }


}

int HierarchicalView::addCounts(QStandardItem *headitem) {
    int i =0;
    if( !headitem->hasChildren() ) return 1;

    for(int j =0; j < headitem->rowCount(); j++) {
       i +=  this->addCounts(headitem->child(j,0));
    }

    headitem->setText(headitem->text() + QString(" (%1)").arg(i));

    return i;
}



void HierarchicalView::createModel(QHash<QString, QStringList> &samples) {

    QList<QVariant> rootData;
    rootData << "Title" << "Summary";

    model = new QStandardItemModel;
    if( HierarchicalView::history ==0 ) {
        HierarchicalView::history =new QHash<QString, QString>();
    }

    QStandardItem *item;

    QStringList headers;
    headers << QString("Sample/Category") ;

    model->setHorizontalHeaderLabels(headers  );

    rootitem = model->invisibleRootItem();


    QHash<QString, QStandardItem *> nameItemMap;

    QStandardItem *headitem = rootitem;

    QStringList sample_names = samples.keys();
    qSort(sample_names.begin(), sample_names.end());

    foreach( QString sample, sample_names) {
        headitem = rootitem;

        QStringList categoryList = samples[sample];

        foreach(QString category, categoryList)  {
            //get children
            nameItemMap.clear();
            for(int i =0; i < headitem->rowCount(); i++) {
                if(!nameItemMap.contains(headitem->child(i, 0)->text())  ) {
                    nameItemMap[headitem->child(i, 0)->text()] = headitem->child(i, 0);
                }
            }

            if( !nameItemMap.contains(category) ) {
                item = this->getQStandardItem(category);
                item->setCheckable(true);
                headitem->appendRow( item);
                nameItemMap[category] = item;
            }
            headitem = nameItemMap[category];
        }
        item = this->getQStandardItem(sample);
        item->setCheckable(true);
        item->setForeground(QBrush(QColor(Qt::blue)));

        headitem->appendRow(item);
    }

    this->restoreHistory();

    this->addCounts(rootitem);

    treeView->setModel(model);
   // treeView->expandAll();
    treeView->expandsOnDoubleClick();
    treeView->expandToDepth(2);

    treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    treeView->resize(400,700);

   // treeView->resize(treeView->sizeHint().width(), treeView->sizeHint().height());

    //treeView->show();

    scroll = new QScrollArea;
    scroll->setWindowTitle("Hierarchical Organization of Samples!");
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
    //scroll->setWidget(treeView);
    QSize wSize = treeView->size();
    int w = wSize.width() < 600 ? wSize.width() :600;
    int h = wSize.height() < 300 ? wSize.height() : 300;
    scroll->resize(w + 5, h + 5);
    //scroll->setMaximumSize(wSize.width() + 10, wSize.height() + 10);

    scroll->show();

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(treeView);

    QHBoxLayout *controlLayout = new QHBoxLayout;

    QWidget *controls = new QWidget;

    QPushButton *collapseAll = new QPushButton("Collapse");
    collapseAll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QPushButton *expandAll = new QPushButton("Expand");
    expandAll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QPushButton *defaultExpand = new QPushButton("Default");
    defaultExpand->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QPushButton *selectAll = new QPushButton("Select All");
    expandAll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QPushButton *unselectAll = new QPushButton("Unselect All");
    defaultExpand->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QPushButton *cancel = new QPushButton("Cancel");
    cancel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QPushButton *OK = new QPushButton("OK");
    OK->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    controlLayout->addStretch(0);
    controlLayout->addWidget(collapseAll);
    controlLayout->addWidget(expandAll);
    controlLayout->addWidget(defaultExpand);
    controlLayout->addWidget(selectAll);
    controlLayout->addWidget(unselectAll);
    controlLayout->addWidget(cancel);
    controlLayout->addWidget(OK);

    controls->setLayout(controlLayout);
    vbox->addWidget(controls);

    scroll->setLayout(vbox);

    connect(collapseAll, SIGNAL(clicked()), this, SLOT(collapseTree()));
    connect(expandAll, SIGNAL(clicked()), this, SLOT(expandTree()));
    connect(defaultExpand, SIGNAL(clicked()), this, SLOT(defaultExpandTree()));

    connect(selectAll, SIGNAL(clicked()), this, SLOT(selectAll()));
    connect(unselectAll, SIGNAL(clicked()), this, SLOT(unselectAll()));

    connect(cancel, SIGNAL(clicked()), this, SLOT(cancelChoice()));
    connect(OK, SIGNAL(clicked()), this, SLOT(OKChoice()));


}


void HierarchicalView::showTreeView(QHash<QString, QStringList> &samples) {
    treeView = new QTreeView();
    treeView->setWindowTitle("Hierarchical Organization of Samples!");
    treeView->setAttribute(Qt::WA_DeleteOnClose);
    treeView->setAlternatingRowColors(true);

    this->createModel(samples);
}


void HierarchicalView::clearObjects() {
    foreach(QStandardItem *p, this->_createdItemList)
        delete p;
    _createdItemList.clear();

    delete model; model=0;
}


void HierarchicalView::collapseTree(){
     treeView->collapseAll();
}

void HierarchicalView::expandTree() {
     treeView->expandAll();
}

void HierarchicalView::selectAll() {


    QStandardItem *headitem = model->invisibleRootItem();
    QStack<QStandardItem *> itemStack;
    itemStack.push(headitem);
    while( !itemStack.isEmpty() ) {
       headitem = itemStack.pop();
       headitem->setCheckState(Qt::Checked);
       for(int i =0; i < headitem->rowCount(); i++) {
          itemStack.push(headitem->child(i,0));
       }
    }
}

void HierarchicalView::unselectAll() {
    QStandardItem *headitem = model->invisibleRootItem();
    QStack<QStandardItem *> itemStack;
    itemStack.push(headitem);
    while( !itemStack.isEmpty() ) {
       headitem = itemStack.pop();
       headitem->setCheckState(Qt::Unchecked);
       for(int i =0; i < headitem->rowCount(); i++) {
          itemStack.push(headitem->child(i,0));
       }
    }
}


void HierarchicalView::defaultExpandTree() {
     treeView->expandToDepth(2);
}
/**
 * @brief HierarchicalView::cancelChoice, cancel the choice
 */
void HierarchicalView::cancelChoice() {
    HierarchicalView::hierarchicalview =0;
    this->hide();
    this->scroll->close();
    this->destroy();
}

void HierarchicalView::insertIntoHistory(QString text) {

    QRegExp num("\\s+\\(\\d+\\)$");
    QString category = text.replace(num, QString(""));

    HierarchicalView::history->insert(category, category);

}

void HierarchicalView::saveHistory() {

    HierarchicalView::history->clear();

    QStandardItem *headitem = model->invisibleRootItem();
    QStack<QStandardItem *> itemStack;

    itemStack.push(headitem);
    while( !itemStack.isEmpty() ) {
       headitem = itemStack.pop();
       if(headitem->checkState()) this->insertIntoHistory(headitem->text());
       for(int i =0; i < headitem->rowCount(); i++) {
          itemStack.push(headitem->child(i,0));
       }
    }
}

void HierarchicalView::restoreHistory() {
    QStandardItem *headitem = model->invisibleRootItem();
    QStack<QStandardItem *> itemStack;
    QStack<bool> checkStack;


    QRegExp num("\\s+\\(\\d+\\)$");
    QString category;

    itemStack.push(headitem);
    checkStack.push(false);
    while( !itemStack.isEmpty() ) {
       headitem = itemStack.pop();
       checkStack.pop();


       category = headitem->text().replace(num, QString(""));

       if( this->history->contains(category)) headitem->setCheckState(Qt::Checked);

       for(int i =0; i < headitem->rowCount(); i++) {
          itemStack.push(headitem->child(i,0));
          if( !checkStack.isEmpty() && checkStack.top()) {
              checkStack.push(true);
          }
          else {
              checkStack.push(headitem->child(i,0)->checkState());
          }
       }
    }
    this->history->clear();

}

/**
 * @brief HierarchicalView::OKChoice, selected samples and their categories are sent to the resultwindow
 */
void HierarchicalView::OKChoice(){
    rootitem = model->invisibleRootItem();
    QList<QStringPair > selectedSamples;

    this->selectTree(rootitem, selectedSamples, rootitem->checkState());


    RunData *rundata = RunData::getRunData();

    QHash<QString, int> rank;
    int i =0;
    foreach(QString sample, rundata->getOutputFolders() ) {
        rank[sample] = i++;
    }

    QList<unsigned int> selectedSamplesRank;
    QStringList selectedSamplesCategories;
    foreach(QStringPair sample, selectedSamples) {
        if(rank.contains(sample.first)) {
            selectedSamplesRank.append(rank[sample.first]);
            selectedSamplesCategories.append(sample.second);
        }
    }

    this->saveHistory();

    this->hide();
    this->scroll->close();

    if( this->resultWindow!=0 )  this->resultWindow->receiveSelection(selectedSamplesRank, selectedSamplesCategories);

    this->destroy();

}

/**
 * @brief HierarchicalView::setReceiver, sets the receiver to send the selected choices to
 * @param resultWindow
 */
void HierarchicalView::setReceiver(ResultWindow *resultWindow  ) {
      this->resultWindow = resultWindow;
}
