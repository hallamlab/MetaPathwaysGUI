/********************************************************************************
** Form generated from reading UI file 'graphdata.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHDATA_H
#define UI_GRAPHDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_GraphData
{
public:
    QGridLayout *gridLayout;
    QPushButton *exportButton;
    QCustomPlot *graph;

    void setupUi(QWidget *GraphData)
    {
        if (GraphData->objectName().isEmpty())
            GraphData->setObjectName(QString::fromUtf8("GraphData"));
        GraphData->resize(670, 853);
        gridLayout = new QGridLayout(GraphData);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        exportButton = new QPushButton(GraphData);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(exportButton->sizePolicy().hasHeightForWidth());
        exportButton->setSizePolicy(sizePolicy);

        gridLayout->addWidget(exportButton, 1, 0, 1, 1, Qt::AlignRight);

        graph = new QCustomPlot(GraphData);
        graph->setObjectName(QString::fromUtf8("graph"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(graph->sizePolicy().hasHeightForWidth());
        graph->setSizePolicy(sizePolicy1);
        graph->setMinimumSize(QSize(646, 700));

        gridLayout->addWidget(graph, 0, 0, 1, 1);


        retranslateUi(GraphData);

        QMetaObject::connectSlotsByName(GraphData);
    } // setupUi

    void retranslateUi(QWidget *GraphData)
    {
        GraphData->setWindowTitle(QApplication::translate("GraphData", "Form", 0, QApplication::UnicodeUTF8));
        exportButton->setText(QApplication::translate("GraphData", "Export", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GraphData: public Ui_GraphData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHDATA_H
