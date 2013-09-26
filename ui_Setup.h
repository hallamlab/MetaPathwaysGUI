/********************************************************************************
** Form generated from reading UI file 'Setup.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETUP_H
#define UI_SETUP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Setup
{
public:
    QLabel *label;
    QFrame *line;
    QLabel *pythonLabel;
    QLabel *PYTHON_EXECUTABLE;
    QPushButton *pythonBrowseButton;
    QLabel *perlLabel;
    QPushButton *perlBrowseButton;
    QLabel *PERL_EXECUTABLE;
    QLabel *metapathwaysLabel;
    QPushButton *metapathwaysBrowseButton;
    QLabel *METAPATHWAYS_PATH;
    QLabel *label_8;
    QPushButton *saveButton;
    QPushButton *cancelButton;
    QFrame *line_2;
    QFrame *line_3;
    QLabel *metapathwaysLabel_2;
    QFrame *line_4;
    QPushButton *databaseButton;
    QLabel *REFDBS;
    QFrame *line_5;
    QLineEdit *pythonExecTxt;
    QLineEdit *perlExecTxt;
    QLineEdit *pathMetaPathwaysTxt;
    QLineEdit *dbDirectoryTxt;

    void setupUi(QWidget *Setup)
    {
        if (Setup->objectName().isEmpty())
            Setup->setObjectName(QString::fromUtf8("Setup"));
        Setup->resize(661, 611);
        label = new QLabel(Setup);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(260, 10, 91, 31));
        QFont font;
        font.setPointSize(24);
        label->setFont(font);
        line = new QFrame(Setup);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(30, 60, 581, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        pythonLabel = new QLabel(Setup);
        pythonLabel->setObjectName(QString::fromUtf8("pythonLabel"));
        pythonLabel->setGeometry(QRect(20, 80, 271, 31));
        QFont font1;
        font1.setPointSize(18);
        pythonLabel->setFont(font1);
        PYTHON_EXECUTABLE = new QLabel(Setup);
        PYTHON_EXECUTABLE->setObjectName(QString::fromUtf8("PYTHON_EXECUTABLE"));
        PYTHON_EXECUTABLE->setGeometry(QRect(20, 110, 401, 16));
        pythonBrowseButton = new QPushButton(Setup);
        pythonBrowseButton->setObjectName(QString::fromUtf8("pythonBrowseButton"));
        pythonBrowseButton->setGeometry(QRect(500, 90, 114, 32));
        perlLabel = new QLabel(Setup);
        perlLabel->setObjectName(QString::fromUtf8("perlLabel"));
        perlLabel->setGeometry(QRect(20, 190, 191, 21));
        perlLabel->setFont(font1);
        perlBrowseButton = new QPushButton(Setup);
        perlBrowseButton->setObjectName(QString::fromUtf8("perlBrowseButton"));
        perlBrowseButton->setGeometry(QRect(500, 190, 114, 32));
        PERL_EXECUTABLE = new QLabel(Setup);
        PERL_EXECUTABLE->setObjectName(QString::fromUtf8("PERL_EXECUTABLE"));
        PERL_EXECUTABLE->setGeometry(QRect(20, 210, 401, 16));
        metapathwaysLabel = new QLabel(Setup);
        metapathwaysLabel->setObjectName(QString::fromUtf8("metapathwaysLabel"));
        metapathwaysLabel->setGeometry(QRect(20, 300, 221, 31));
        metapathwaysLabel->setFont(font1);
        metapathwaysBrowseButton = new QPushButton(Setup);
        metapathwaysBrowseButton->setObjectName(QString::fromUtf8("metapathwaysBrowseButton"));
        metapathwaysBrowseButton->setGeometry(QRect(500, 310, 114, 32));
        METAPATHWAYS_PATH = new QLabel(Setup);
        METAPATHWAYS_PATH->setObjectName(QString::fromUtf8("METAPATHWAYS_PATH"));
        METAPATHWAYS_PATH->setGeometry(QRect(20, 330, 401, 16));
        label_8 = new QLabel(Setup);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(140, 40, 351, 21));
        label_8->setWordWrap(true);
        saveButton = new QPushButton(Setup);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setGeometry(QRect(320, 530, 121, 27));
        cancelButton = new QPushButton(Setup);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(200, 530, 111, 27));
        line_2 = new QFrame(Setup);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(30, 160, 581, 21));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(Setup);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(30, 280, 581, 20));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        metapathwaysLabel_2 = new QLabel(Setup);
        metapathwaysLabel_2->setObjectName(QString::fromUtf8("metapathwaysLabel_2"));
        metapathwaysLabel_2->setGeometry(QRect(20, 420, 171, 31));
        metapathwaysLabel_2->setFont(font1);
        line_4 = new QFrame(Setup);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(30, 400, 581, 20));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        databaseButton = new QPushButton(Setup);
        databaseButton->setObjectName(QString::fromUtf8("databaseButton"));
        databaseButton->setGeometry(QRect(500, 430, 114, 32));
        REFDBS = new QLabel(Setup);
        REFDBS->setObjectName(QString::fromUtf8("REFDBS"));
        REFDBS->setGeometry(QRect(20, 450, 401, 16));
        line_5 = new QFrame(Setup);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setGeometry(QRect(0, 510, 441, 20));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);
        pythonExecTxt = new QLineEdit(Setup);
        pythonExecTxt->setObjectName(QString::fromUtf8("pythonExecTxt"));
        pythonExecTxt->setGeometry(QRect(260, 90, 221, 22));
        perlExecTxt = new QLineEdit(Setup);
        perlExecTxt->setObjectName(QString::fromUtf8("perlExecTxt"));
        perlExecTxt->setGeometry(QRect(260, 190, 221, 22));
        pathMetaPathwaysTxt = new QLineEdit(Setup);
        pathMetaPathwaysTxt->setObjectName(QString::fromUtf8("pathMetaPathwaysTxt"));
        pathMetaPathwaysTxt->setGeometry(QRect(260, 310, 221, 22));
        dbDirectoryTxt = new QLineEdit(Setup);
        dbDirectoryTxt->setObjectName(QString::fromUtf8("dbDirectoryTxt"));
        dbDirectoryTxt->setGeometry(QRect(260, 430, 221, 22));
        QWidget::setTabOrder(pythonBrowseButton, perlBrowseButton);
        QWidget::setTabOrder(perlBrowseButton, metapathwaysBrowseButton);
        QWidget::setTabOrder(metapathwaysBrowseButton, databaseButton);
        QWidget::setTabOrder(databaseButton, cancelButton);
        QWidget::setTabOrder(cancelButton, saveButton);

        retranslateUi(Setup);

        QMetaObject::connectSlotsByName(Setup);
    } // setupUi

    void retranslateUi(QWidget *Setup)
    {
        Setup->setWindowTitle(QApplication::translate("Setup", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Setup", "Setup", 0, QApplication::UnicodeUTF8));
        pythonLabel->setText(QApplication::translate("Setup", "Python Executable", 0, QApplication::UnicodeUTF8));
        PYTHON_EXECUTABLE->setText(QApplication::translate("Setup", "TextLabel", 0, QApplication::UnicodeUTF8));
        pythonBrowseButton->setText(QApplication::translate("Setup", "Browse", 0, QApplication::UnicodeUTF8));
        perlLabel->setText(QApplication::translate("Setup", "Perl Executable", 0, QApplication::UnicodeUTF8));
        perlBrowseButton->setText(QApplication::translate("Setup", "Browse", 0, QApplication::UnicodeUTF8));
        PERL_EXECUTABLE->setText(QApplication::translate("Setup", "TextLabel", 0, QApplication::UnicodeUTF8));
        metapathwaysLabel->setText(QApplication::translate("Setup", "MetaPathways Directory", 0, QApplication::UnicodeUTF8));
        metapathwaysBrowseButton->setText(QApplication::translate("Setup", "Browse", 0, QApplication::UnicodeUTF8));
        METAPATHWAYS_PATH->setText(QApplication::translate("Setup", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("Setup", "MetaPathways needs to know where these are to work.", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("Setup", "Save Settings", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("Setup", "Cancel", 0, QApplication::UnicodeUTF8));
        metapathwaysLabel_2->setText(QApplication::translate("Setup", "Database Directory", 0, QApplication::UnicodeUTF8));
        databaseButton->setText(QApplication::translate("Setup", "Browse", 0, QApplication::UnicodeUTF8));
        REFDBS->setText(QApplication::translate("Setup", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Setup: public Ui_Setup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETUP_H
