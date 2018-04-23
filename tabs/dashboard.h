/********************************************************************************
** Form generated from reading UI file 'DashBoard.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DASHBOARD_H
#define UI_DASHBOARD_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include "ui_DashBoard.h"

namespace Ui {
    class DashBoard;
} // namespace Ui

class DashBoard: public QWidget
{
    Q_OBJECT

public:
    QPushButton *export_2;
    QFrame *line_6;
    QPushButton *refresh;
    QLabel *phandisamples;
    QPushButton *pushButton;
    QLabel *username;
    QPushButton *download;
    QLabel *downloadedsamples;
    QLabel *label;
    QLabel *label_2;

    DashBoard *getDashBoard();

    ~DashBoard();

    void setupUi(QWidget *Dashboard)
    {
        if (Dashboard->objectName().isEmpty())
            Dashboard->setObjectName(QString::fromUtf8("DashBoard"));
        Dashboard->resize(760, 664);
        Dashboard->setLayoutDirection(Qt::LeftToRight);
        export_2 = new QPushButton(Dashboard);
        export_2->setObjectName(QString::fromUtf8("export_2"));
        export_2->setGeometry(QRect(510, 480, 87, 32));
        line_6 = new QFrame(Dashboard);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setGeometry(QRect(30, 461, 671, 16));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);
        refresh = new QPushButton(Dashboard);
        refresh->setObjectName(QString::fromUtf8("refresh"));
        refresh->setGeometry(QRect(620, 480, 92, 32));
        phandisamples = new QLabel(Dashboard);
        phandisamples->setObjectName(QString::fromUtf8("phandisamples"));
        phandisamples->setGeometry(QRect(50, 90, 241, 31));
        QFont font;
        font.setPointSize(18);
        phandisamples->setFont(font);
        pushButton = new QPushButton(Dashboard);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(640, 0, 99, 27));
        username = new QLabel(Dashboard);
        username->setObjectName(QString::fromUtf8("username"));
        username->setGeometry(QRect(540, 10, 67, 17));
        download = new QPushButton(Dashboard);
        download->setObjectName(QString::fromUtf8("download"));
        download->setGeometry(QRect(400, 480, 87, 32));
        downloadedsamples = new QLabel(Dashboard);
        downloadedsamples->setObjectName(QString::fromUtf8("downloadedsamples"));
        downloadedsamples->setGeometry(QRect(350, 90, 261, 31));
        downloadedsamples->setFont(font);
        label = new QLabel(Dashboard);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(290, 90, 31, 31));
        label_2 = new QLabel(Dashboard);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(630, 90, 31, 31));
        QWidget::setTabOrder(export_2, refresh);

        retranslateUi(Dashboard);

        QMetaObject::connectSlotsByName(Dashboard);
    } // setupUi


    void retranslateUi(QWidget *Dashboard)
    {
        Dashboard->setWindowTitle(QApplication::translate("DashBoard", "Form", 0, QApplication::UnicodeUTF8));
        export_2->setText(QApplication::translate("DashBoard", "Export", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        refresh->setToolTip(QApplication::translate("DashBoard", "Once you are done filling in the paths, please click the proceed button to update the paths", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        refresh->setText(QApplication::translate("DashBoard", "Refresh", 0, QApplication::UnicodeUTF8));
        phandisamples->setText(QApplication::translate("DashBoard", "#Samples in Phandi:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("DashBoard", "Sign In", 0, QApplication::UnicodeUTF8));
        username->setText(QString());
        download->setText(QApplication::translate("DashBoard", "Download", 0, QApplication::UnicodeUTF8));
        downloadedsamples->setText(QApplication::translate("DashBoard", "# Samples downloaded:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DashBoard", "<html><head/><body><p><span style=\" font-size:18pt;\">0</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DashBoard", "<html><head/><body><p><span style=\" font-size:18pt;\">0</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

private:
    static DashBoard *dashboard;
    DashBoard(QWidget *parent = 0);
    Ui::DashBoard *ui;
    //QWidget* parent;
};


#endif // UI_DASHBOARD_H
