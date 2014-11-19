#include "statusview.h"

StatusView *StatusView::statusview =0;

StatusView *StatusView::getStatusView() {
    if(StatusView::statusview==0)
        StatusView::statusview = new StatusView;
    return StatusView::statusview;
}

StatusView::StatusView(QObject *parent) :
    QObject(parent)
{


}


QStandardItem * StatusView::getQStandardItem(const QString &name) {
    QStandardItem *standarditem = new QStandardItem(name);
    _createdItemList.append(standarditem);
    return standarditem;
}


void StatusView::readDataFromFile(const QString &absFileName, const QString &filename) {
    QFileInfo fileinfo(absFileName);

    QStandardItem *sample = this->getQStandardItem(filename);

    rootitem->appendRow(sample);

    if (!fileinfo.exists())  {
        return;
    }

    QFile inputFile(absFileName);

    QRegExp stepRegExp("#STEP");
    QHash<QString, QHash<QString, QStringList > > data;
    QString stepName;


    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        while ( !in.atEnd() )  {
            QString line = in.readLine().trimmed();
            QStringList fields = line.split(QChar('\t'));
            if( fields.length() < 2) continue;
            if( fields[0].indexOf(stepRegExp) >=0 ) {
                stepName = fields[1];
                data[stepName] = QHash<QString, QStringList>();
                continue;
            }
            if( stepName.size() == 0) continue;
            if( fields.size() < 2)  continue;


            if(!data[stepName].contains(fields[1]))  data[stepName][fields[1]] = QStringList();

            for(int i = 2 ; i < fields.size(); i++) {
                data[stepName][fields[1]].append(fields[i]);
            }
        inputFile.close();
        }
    }

    QStringList keys = data.keys();
    foreach(QString key, keys) {
        if( data[key].size() ==0) data.remove(key);
    }


    foreach(QString stepName, data.keys()) {
        QStandardItem *stepNameItem = this->getQStandardItem(stepName);
        sample->appendRow(stepNameItem);

        if( data[stepName].size() < 2) continue;

        foreach(QString type, data[stepName].keys()) {
            // this is the type of error
            QStandardItem *errorTypeItem = this->getQStandardItem(type);
            stepNameItem->appendRow(errorTypeItem);

            QList<QStandardItem *> itemList;
            QString errorMessage = "";
            for(int i = 1; i < data[stepName][type].size(); i++) {
                errorMessage += data[stepName][type][i];
            }
            QStandardItem *messageItem =  this->getQStandardItem(errorMessage);
            errorTypeItem->appendRow(messageItem);
        }

    }

    return;

}

void StatusView::createModel() {
    QString runstatsFile;

    QList<QVariant> rootData;
    rootData << "Title" << "Summary";

    model = new QStandardItemModel;
    rootitem = model->invisibleRootItem();


    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
    foreach(QString filename, this->filenames) {
       runstatsFile = samplercmgr->getFilePath(filename, ERRORS);
       this->readDataFromFile(runstatsFile, filename);
    }
    treeView->setModel(model);
    treeView->expandAll();
    treeView->show();

}

void StatusView::setFileList(QStringList &filenames) {
    this->filenames = filenames;
}


void StatusView::showTreeView() {
    treeView = new QTreeView();
    treeView->setWindowTitle("Error and Wornings!");
    treeView->setAttribute(Qt::WA_DeleteOnClose);
    treeView->setAlternatingRowColors(true);
    RunData *rundata= RunData::getRunData();

    SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
     //treeView->show();

    samplercmgr->setOutPutFolders(rundata->getSampleFolderMap());
    this->filenames = rundata->getFileList(rundata->getCurrentInputFormat());
    this->createModel();
}


void StatusView::clearObjects() {
    foreach(QStandardItem *p, this->_createdItemList)
        delete p;
    _createdItemList.clear();

    delete model; model=0;
}
