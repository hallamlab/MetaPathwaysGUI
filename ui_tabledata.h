/********************************************************************************
** Form generated from reading UI file 'tabledata.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABLEDATA_H
#define UI_TABLEDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TableData
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QPushButton *exportButton;
    QPushButton *searchButton;

    void setupUi(QWidget *TableData)
    {
        if (TableData->objectName().isEmpty())
            TableData->setObjectName(QString::fromUtf8("TableData"));
        TableData->resize(695, 900);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TableData->sizePolicy().hasHeightForWidth());
        TableData->setSizePolicy(sizePolicy);
        TableData->setMinimumSize(QSize(650, 900));
        gridLayout = new QGridLayout(TableData);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tableWidget = new QTableWidget(TableData);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy1);
        tableWidget->setMinimumSize(QSize(500, 675));
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 3);

        exportButton = new QPushButton(TableData);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(exportButton->sizePolicy().hasHeightForWidth());
        exportButton->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(exportButton, 1, 2, 1, 1);

        searchButton = new QPushButton(TableData);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));
        sizePolicy2.setHeightForWidth(searchButton->sizePolicy().hasHeightForWidth());
        searchButton->setSizePolicy(sizePolicy2);
        searchButton->setMinimumSize(QSize(75, 32));

        gridLayout->addWidget(searchButton, 1, 1, 1, 1);


        retranslateUi(TableData);

        QMetaObject::connectSlotsByName(TableData);
    } // setupUi

    void retranslateUi(QWidget *TableData)
    {
        TableData->setWindowTitle(QApplication::translate("TableData", "Form", 0, QApplication::UnicodeUTF8));
        exportButton->setText(QApplication::translate("TableData", "Export", 0, QApplication::UnicodeUTF8));
        searchButton->setText(QApplication::translate("TableData", "Search", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TableData: public Ui_TableData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABLEDATA_H
