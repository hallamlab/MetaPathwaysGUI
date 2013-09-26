/********************************************************************************
** Form generated from reading UI file 'gridchoice.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDCHOICE_H
#define UI_GRIDCHOICE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GridChoice
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QFrame *line_2;
    QLabel *label_2;
    QPushButton *EC2Instance;
    QFrame *line;
    QLabel *label_3;
    QPushButton *NonEC2Instance;

    void setupUi(QWidget *GridChoice)
    {
        if (GridChoice->objectName().isEmpty())
            GridChoice->setObjectName(QString::fromUtf8("GridChoice"));
        GridChoice->resize(484, 265);
        gridLayout = new QGridLayout(GridChoice);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(GridChoice);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(15);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        line_2 = new QFrame(GridChoice);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 1, 0, 1, 1);

        label_2 = new QLabel(GridChoice);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setWordWrap(true);

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        EC2Instance = new QPushButton(GridChoice);
        EC2Instance->setObjectName(QString::fromUtf8("EC2Instance"));

        gridLayout->addWidget(EC2Instance, 3, 0, 1, 1);

        line = new QFrame(GridChoice);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 4, 0, 1, 1);

        label_3 = new QLabel(GridChoice);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setWordWrap(true);

        gridLayout->addWidget(label_3, 5, 0, 1, 1);

        NonEC2Instance = new QPushButton(GridChoice);
        NonEC2Instance->setObjectName(QString::fromUtf8("NonEC2Instance"));

        gridLayout->addWidget(NonEC2Instance, 6, 0, 1, 1);


        retranslateUi(GridChoice);

        QMetaObject::connectSlotsByName(GridChoice);
    } // setupUi

    void retranslateUi(QWidget *GridChoice)
    {
        GridChoice->setWindowTitle(QApplication::translate("GridChoice", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GridChoice", "Adding a grid to MetaPathways", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("GridChoice", "I have an Amazon EC2 account, and would like to create a new grid.", 0, QApplication::UnicodeUTF8));
        EC2Instance->setText(QApplication::translate("GridChoice", "Specify Amazon EC2 Instance", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("GridChoice", "I want to specify a non-EC2 grid.", 0, QApplication::UnicodeUTF8));
        NonEC2Instance->setText(QApplication::translate("GridChoice", "Specify Non-EC2 Grid Instance", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GridChoice: public Ui_GridChoice {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDCHOICE_H
