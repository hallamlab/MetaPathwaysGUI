/********************************************************************************
** Form generated from reading UI file 'graphicsrepresentation.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPHICSREPRESENTATION_H
#define UI_GRAPHICSREPRESENTATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GraphicsRepresentation
{
public:
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView;

    void setupUi(QWidget *GraphicsRepresentation)
    {
        if (GraphicsRepresentation->objectName().isEmpty())
            GraphicsRepresentation->setObjectName(QString::fromUtf8("GraphicsRepresentation"));
        GraphicsRepresentation->setWindowModality(Qt::NonModal);
        GraphicsRepresentation->resize(670, 853);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GraphicsRepresentation->sizePolicy().hasHeightForWidth());
        GraphicsRepresentation->setSizePolicy(sizePolicy);
        GraphicsRepresentation->setMinimumSize(QSize(650, 840));
        horizontalLayout = new QHBoxLayout(GraphicsRepresentation);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        graphicsView = new QGraphicsView(GraphicsRepresentation);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy1);
        graphicsView->setMinimumSize(QSize(600, 200));
        graphicsView->setMaximumSize(QSize(600, 200));
        graphicsView->setLayoutDirection(Qt::LeftToRight);
        graphicsView->setInputMethodHints(Qt::ImhPreferNumbers);
        graphicsView->setSceneRect(QRectF(0, 0, 670, 853));

        horizontalLayout->addWidget(graphicsView);


        retranslateUi(GraphicsRepresentation);

        QMetaObject::connectSlotsByName(GraphicsRepresentation);
    } // setupUi

    void retranslateUi(QWidget *GraphicsRepresentation)
    {
        GraphicsRepresentation->setWindowTitle(QApplication::translate("GraphicsRepresentation", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GraphicsRepresentation: public Ui_GraphicsRepresentation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPHICSREPRESENTATION_H
