/****************************************************************************
 **
 ** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 /*
     treemodel.cpp

     Provides a simple tree model to show how to create and use hierarchical
     models.
 */

 #include <QtGui>

 #include "treeitem.h"
 #include "treemodel.h"
#include<QDebug>

 TreeModel::TreeModel(QObject *parent)
     : QAbstractItemModel(parent)
 {
     QList<QVariant> rootData;
     rootData << "Sample/Step" << "Error" << "Note";
     rootItem = new TreeItem(rootData);
 }


 void TreeModel::setSampleNames(const QStringList &samplenames) {
     this->sampleNames = samplenames;
 }


 TreeModel::~TreeModel()
 {
     delete rootItem;
 }

 int TreeModel::columnCount(const QModelIndex &parent) const
 {
     if (parent.isValid())
         return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
     else
         return rootItem->columnCount();
 }

 QVariant TreeModel::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();

     TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

     return item->data(index.column());
 }

 Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
 {
     if (!index.isValid())
         return 0;

     return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
 }

 QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                                int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
         return rootItem->data(section);

     return QVariant();
 }

 QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
             const
 {
     if (!hasIndex(row, column, parent))
         return QModelIndex();

     TreeItem *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<TreeItem*>(parent.internalPointer());

     TreeItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

 QModelIndex TreeModel::parent(const QModelIndex &index) const
 {
     if (!index.isValid())
         return QModelIndex();

     TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
     TreeItem *parentItem = childItem->parent();

     if (parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }

 int TreeModel::rowCount(const QModelIndex &parent) const
 {
     TreeItem *parentItem;
     if (parent.column() > 0)
         return 0;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<TreeItem*>(parent.internalPointer());

     return parentItem->childCount();
 }

 void TreeModel::readFiles() {
     TreeItem *parent = this->rootItem;

     SampleResourceManager *samplercmgr = SampleResourceManager::getSampleResourceManager();
     RunData *rundata = RunData::getRunData();
     QString OUTPUTPATH = rundata->getParams()["folderOutput"];
     samplercmgr->setOutPutPath(OUTPUTPATH);

     foreach(QString filename, this->sampleNames) {
        QHash<QString, QHash<QString, QList<QStringList > > > data;
        QString runstatsFile = samplercmgr->getFilePath(filename, ERRORS);
        if( this->readDataFromFile(runstatsFile, filename, data) )
           this->addWarningsFromSample(filename, data, parent, true);
        else  this->addWarningsFromSample(filename, data, parent, false);
       // qDebug() << data;
     }
 }


 // add the sample errors and warning to it
 TreeItem *TreeModel::addWarningsFromSample(QString sampleName, const QHash<QString, QHash<QString, QList< QStringList > > > &data, TreeItem* parent, bool fileFound) {
      QList<QVariant> columnData;

      // add the samplename
      columnData << sampleName;
      if( fileFound )
         columnData << "" << "";
      else
         columnData <<" Error status file not found or generated" << "";


      //  qDebug() << columnData;
      TreeItem *treeitem = new TreeItem(columnData, parent);
      parent->appendChild(treeitem);
      parent  = treeitem;
      // loop over the steps
      QHash<QString, TreeItem *> stepsInUse;

      foreach(QString stepName, data.keys()) {
          columnData.clear();
          columnData << stepName << "" << "";
          if( !stepsInUse.contains(stepName)) {
              treeitem = new TreeItem(columnData, parent);
              parent->appendChild(treeitem);
              stepsInUse[stepName] = treeitem;
          }
          TreeItem *stepParent = stepsInUse[stepName];

          // now go over the ERROR/WARNINGs
          QHash<QString, TreeItem *> errorTypesInUse;
          foreach(QString errorType, data[stepName].keys()) {
              columnData.clear();
              columnData << errorType << "" << "";
              if( !errorTypesInUse.contains(errorType)) {
                  treeitem = new TreeItem(columnData, stepParent);
                  stepParent->appendChild(treeitem);
                  errorTypesInUse[errorType] = treeitem;
              }

              TreeItem *errorParent = errorTypesInUse[errorType];

            //  qDebug() << data[stepName][errorType];
              int i = 1;
              foreach(QStringList errorList, data[stepName][errorType]) {
                  columnData.clear();
                  columnData <<  QString::number(i++);

                  foreach(QString errwarmsg, errorList) {
                      columnData << errwarmsg;
                  }

                  treeitem = new TreeItem(columnData, errorParent);
                  errorParent->appendChild(treeitem);
              /*    qDebug() << "\n\n";
                  qDebug() << errorType;
                  qDebug() << errorList;
                  qDebug() << columnData.size() << this->columnCount() << errorParent->childCount();
                  qDebug() << columnData; */

              }
          } // done with errors and warnings


      }


      return treeitem;
 }

 bool TreeModel::readDataFromFile(const QString &absFileName, const QString &filename, QHash<QString, QHash<QString, QList<QStringList > > > &data) {
     QFileInfo fileinfo(absFileName);

     if (!fileinfo.exists())  {
         return false;
     }

     QFile inputFile(absFileName);

     QRegExp stepRegExp("#STEP");
     QString stepName;


     if (inputFile.open(QIODevice::ReadOnly)) {
         QTextStream in(&inputFile);
         while ( !in.atEnd() )  {
             QString line = in.readLine().trimmed();
             QStringList fields = line.split(QChar('\t'));
             if( fields.length() < 2) continue;
             if( fields[0].indexOf(stepRegExp) >=0 ) {
                 stepName = fields[1];
                 data[stepName] = QHash<QString, QList<QStringList> >();
                 continue;
             }
             if( stepName.size() == 0) continue;
             if( fields.size() < 2)  continue;


             if(!data[stepName].contains(fields[1]))  data[stepName][fields[1]] = QList<QStringList>();

             QStringList list;
             for(int i = 2 ; i < fields.size(); i++) {
                 list.append(fields[i]);
             }
             data[stepName][fields[1]].append(list);

         inputFile.close();
         }
     }

     QStringList keys = data.keys();
     foreach(QString key, keys) {
         if( data[key].size() ==0) data.remove(key);
     }

     return true;

 }


