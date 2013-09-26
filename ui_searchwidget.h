/********************************************************************************
** Form generated from reading UI file 'searchwidget.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHWIDGET_H
#define UI_SEARCHWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SearchWidget
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *keyword;
    QLabel *label_3;
    QComboBox *columnSelect;
    QDialogButtonBox *okAndCancel;
    QCheckBox *caseSensitive;

    void setupUi(QWidget *SearchWidget)
    {
        if (SearchWidget->objectName().isEmpty())
            SearchWidget->setObjectName(QString::fromUtf8("SearchWidget"));
        SearchWidget->resize(443, 196);
        gridLayout = new QGridLayout(SearchWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(SearchWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(16777215, 35));
        QFont font;
        font.setPointSize(18);
        label_2->setFont(font);

        gridLayout->addWidget(label_2, 0, 0, 1, 2);

        label = new QLabel(SearchWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        keyword = new QLineEdit(SearchWidget);
        keyword->setObjectName(QString::fromUtf8("keyword"));

        gridLayout->addWidget(keyword, 1, 1, 1, 1);

        label_3 = new QLabel(SearchWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        columnSelect = new QComboBox(SearchWidget);
        columnSelect->setObjectName(QString::fromUtf8("columnSelect"));

        gridLayout->addWidget(columnSelect, 2, 1, 1, 1);

        okAndCancel = new QDialogButtonBox(SearchWidget);
        okAndCancel->setObjectName(QString::fromUtf8("okAndCancel"));
        okAndCancel->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(okAndCancel, 3, 1, 1, 1);

        caseSensitive = new QCheckBox(SearchWidget);
        caseSensitive->setObjectName(QString::fromUtf8("caseSensitive"));

        gridLayout->addWidget(caseSensitive, 3, 0, 1, 1);


        retranslateUi(SearchWidget);

        QMetaObject::connectSlotsByName(SearchWidget);
    } // setupUi

    void retranslateUi(QWidget *SearchWidget)
    {
        SearchWidget->setWindowTitle(QApplication::translate("SearchWidget", "Form", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SearchWidget", "Search", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SearchWidget", "Keyword", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SearchWidget", "Columns", 0, QApplication::UnicodeUTF8));
        columnSelect->clear();
        columnSelect->insertItems(0, QStringList()
         << QApplication::translate("SearchWidget", "ALL", 0, QApplication::UnicodeUTF8)
        );
        caseSensitive->setText(QApplication::translate("SearchWidget", "Case sensitive", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SearchWidget: public Ui_SearchWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHWIDGET_H
