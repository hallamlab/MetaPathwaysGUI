/********************************************************************************
** Form generated from reading UI file 'SettingsTab.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSTAB_H
#define UI_SETTINGSTAB_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsTab
{
public:
    QGridLayout *gridLayout;
    QPushButton *rrnaREFDBSButton;
    QLabel *label_17;
    QFrame *line_5;
    QLabel *label_13;
    QLabel *label_4;
    QSpinBox *quality_controlMIN_LENGTH;
    QComboBox *quality_controlDELETE_REPLICATES;
    QLabel *label_8;
    QTextEdit *annotationDBS;
    QSpinBox *annotationMIN_LENGTH;
    QLabel *label_21;
    QLabel *label_19;
    QSpinBox *orf_predictionMIN_LENGTH;
    QLabel *label_6;
    QLabel *label_5;
    QComboBox *orf_predictionALGORITHM;
    QLabel *annotationDBSWarning;
    QTextEdit *annotationMAX_EVALUE;
    QLabel *label_11;
    QTextEdit *rrnaREFDBS;
    QDoubleSpinBox *annotationMIN_BSR;
    QFrame *line;
    QLabel *rrnaREFDBSWarning;
    QLabel *label_15;
    QLabel *label_7;
    QDoubleSpinBox *rrnaMIN_IDENTITY;
    QLabel *label;
    QLabel *label_20;
    QTextEdit *rrnaMAX_EVALUE;
    QFrame *line_2;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label_10;
    QSpinBox *annotationMAX_HITS;
    QSpinBox *annotationMIN_SCORE;
    QSpinBox *rrnaMIN_BITSCORE;
    QLabel *label_16;
    QFrame *line_4;
    QComboBox *annotationALGORITHM;
    QLabel *label_9;
    QLabel *label_22;
    QFrame *line_3;
    QComboBox *ptools_settingsTAXONOMIC_PRUNING;
    QLabel *label_12;
    QLabel *label_14;
    QPushButton *annotationDBSButton;
    QLabel *label_18;

    void setupUi(QWidget *SettingsTab)
    {
        if (SettingsTab->objectName().isEmpty())
            SettingsTab->setObjectName(QString::fromUtf8("SettingsTab"));
        SettingsTab->resize(641, 977);
        gridLayout = new QGridLayout(SettingsTab);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        rrnaREFDBSButton = new QPushButton(SettingsTab);
        rrnaREFDBSButton->setObjectName(QString::fromUtf8("rrnaREFDBSButton"));

        gridLayout->addWidget(rrnaREFDBSButton, 27, 2, 1, 1);

        label_17 = new QLabel(SettingsTab);
        label_17->setObjectName(QString::fromUtf8("label_17"));

        gridLayout->addWidget(label_17, 32, 0, 1, 2);

        line_5 = new QFrame(SettingsTab);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_5, 19, 0, 1, 7);

        label_13 = new QLabel(SettingsTab);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 16, 3, 1, 2);

        label_4 = new QLabel(SettingsTab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QFont font;
        font.setPointSize(18);
        label_4->setFont(font);

        gridLayout->addWidget(label_4, 20, 0, 1, 3);

        quality_controlMIN_LENGTH = new QSpinBox(SettingsTab);
        quality_controlMIN_LENGTH->setObjectName(QString::fromUtf8("quality_controlMIN_LENGTH"));
        quality_controlMIN_LENGTH->setMaximum(99999999);
        quality_controlMIN_LENGTH->setSingleStep(1);
        quality_controlMIN_LENGTH->setValue(180);

        gridLayout->addWidget(quality_controlMIN_LENGTH, 5, 2, 1, 1);

        quality_controlDELETE_REPLICATES = new QComboBox(SettingsTab);
        quality_controlDELETE_REPLICATES->setObjectName(QString::fromUtf8("quality_controlDELETE_REPLICATES"));
        quality_controlDELETE_REPLICATES->setEditable(true);
        quality_controlDELETE_REPLICATES->setMaxVisibleItems(2);
        quality_controlDELETE_REPLICATES->setMaxCount(2);

        gridLayout->addWidget(quality_controlDELETE_REPLICATES, 6, 2, 1, 1);

        label_8 = new QLabel(SettingsTab);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 10, 0, 1, 1);

        annotationDBS = new QTextEdit(SettingsTab);
        annotationDBS->setObjectName(QString::fromUtf8("annotationDBS"));
        annotationDBS->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        annotationDBS->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        annotationDBS->setLineWrapMode(QTextEdit::WidgetWidth);

        gridLayout->addWidget(annotationDBS, 11, 2, 1, 4);

        annotationMIN_LENGTH = new QSpinBox(SettingsTab);
        annotationMIN_LENGTH->setObjectName(QString::fromUtf8("annotationMIN_LENGTH"));
        annotationMIN_LENGTH->setMaximum(1000);
        annotationMIN_LENGTH->setValue(60);

        gridLayout->addWidget(annotationMIN_LENGTH, 16, 5, 1, 1);

        label_21 = new QLabel(SettingsTab);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        gridLayout->addWidget(label_21, 36, 0, 1, 2);

        label_19 = new QLabel(SettingsTab);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        gridLayout->addWidget(label_19, 30, 3, 1, 2);

        orf_predictionMIN_LENGTH = new QSpinBox(SettingsTab);
        orf_predictionMIN_LENGTH->setObjectName(QString::fromUtf8("orf_predictionMIN_LENGTH"));
        orf_predictionMIN_LENGTH->setMinimum(1);
        orf_predictionMIN_LENGTH->setMaximum(10000);
        orf_predictionMIN_LENGTH->setValue(60);

        gridLayout->addWidget(orf_predictionMIN_LENGTH, 21, 2, 1, 1);

        label_6 = new QLabel(SettingsTab);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 21, 0, 1, 2);

        label_5 = new QLabel(SettingsTab);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 22, 0, 1, 2);

        orf_predictionALGORITHM = new QComboBox(SettingsTab);
        orf_predictionALGORITHM->setObjectName(QString::fromUtf8("orf_predictionALGORITHM"));

        gridLayout->addWidget(orf_predictionALGORITHM, 22, 2, 1, 1);

        annotationDBSWarning = new QLabel(SettingsTab);
        annotationDBSWarning->setObjectName(QString::fromUtf8("annotationDBSWarning"));

        gridLayout->addWidget(annotationDBSWarning, 13, 2, 1, 2);

        annotationMAX_EVALUE = new QTextEdit(SettingsTab);
        annotationMAX_EVALUE->setObjectName(QString::fromUtf8("annotationMAX_EVALUE"));
        annotationMAX_EVALUE->setMinimumSize(QSize(139, 0));
        annotationMAX_EVALUE->setMaximumSize(QSize(16777215, 20));

        gridLayout->addWidget(annotationMAX_EVALUE, 17, 2, 1, 1);

        label_11 = new QLabel(SettingsTab);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout->addWidget(label_11, 17, 0, 1, 1);

        rrnaREFDBS = new QTextEdit(SettingsTab);
        rrnaREFDBS->setObjectName(QString::fromUtf8("rrnaREFDBS"));
        rrnaREFDBS->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        rrnaREFDBS->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        rrnaREFDBS->setLineWrapMode(QTextEdit::WidgetWidth);

        gridLayout->addWidget(rrnaREFDBS, 26, 2, 1, 4);

        annotationMIN_BSR = new QDoubleSpinBox(SettingsTab);
        annotationMIN_BSR->setObjectName(QString::fromUtf8("annotationMIN_BSR"));
        annotationMIN_BSR->setMinimum(0);
        annotationMIN_BSR->setMaximum(1);
        annotationMIN_BSR->setSingleStep(0.01);
        annotationMIN_BSR->setValue(0.01);

        gridLayout->addWidget(annotationMIN_BSR, 14, 2, 1, 1);

        line = new QFrame(SettingsTab);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 8, 0, 1, 7);

        rrnaREFDBSWarning = new QLabel(SettingsTab);
        rrnaREFDBSWarning->setObjectName(QString::fromUtf8("rrnaREFDBSWarning"));

        gridLayout->addWidget(rrnaREFDBSWarning, 28, 2, 1, 3);

        label_15 = new QLabel(SettingsTab);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setFont(font);

        gridLayout->addWidget(label_15, 24, 0, 1, 4);

        label_7 = new QLabel(SettingsTab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setFont(font);

        gridLayout->addWidget(label_7, 9, 0, 1, 4);

        rrnaMIN_IDENTITY = new QDoubleSpinBox(SettingsTab);
        rrnaMIN_IDENTITY->setObjectName(QString::fromUtf8("rrnaMIN_IDENTITY"));
        rrnaMIN_IDENTITY->setDecimals(2);
        rrnaMIN_IDENTITY->setMinimum(0);
        rrnaMIN_IDENTITY->setSingleStep(1);
        rrnaMIN_IDENTITY->setValue(30);

        gridLayout->addWidget(rrnaMIN_IDENTITY, 30, 2, 1, 1);

        label = new QLabel(SettingsTab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);

        gridLayout->addWidget(label, 2, 0, 1, 3);

        label_20 = new QLabel(SettingsTab);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setFont(font);

        gridLayout->addWidget(label_20, 35, 0, 1, 1);

        rrnaMAX_EVALUE = new QTextEdit(SettingsTab);
        rrnaMAX_EVALUE->setObjectName(QString::fromUtf8("rrnaMAX_EVALUE"));
        rrnaMAX_EVALUE->setMaximumSize(QSize(16777215, 20));

        gridLayout->addWidget(rrnaMAX_EVALUE, 32, 2, 1, 1);

        line_2 = new QFrame(SettingsTab);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 1, 0, 1, 7);

        label_3 = new QLabel(SettingsTab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 6, 0, 1, 2);

        label_2 = new QLabel(SettingsTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 5, 0, 1, 2);

        label_10 = new QLabel(SettingsTab);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout->addWidget(label_10, 14, 0, 1, 1);

        annotationMAX_HITS = new QSpinBox(SettingsTab);
        annotationMAX_HITS->setObjectName(QString::fromUtf8("annotationMAX_HITS"));
        annotationMAX_HITS->setMinimum(0);
        annotationMAX_HITS->setMaximum(1000);
        annotationMAX_HITS->setValue(10);

        gridLayout->addWidget(annotationMAX_HITS, 14, 5, 1, 1);

        annotationMIN_SCORE = new QSpinBox(SettingsTab);
        annotationMIN_SCORE->setObjectName(QString::fromUtf8("annotationMIN_SCORE"));
        annotationMIN_SCORE->setMinimum(1);
        annotationMIN_SCORE->setMaximum(10000);

        gridLayout->addWidget(annotationMIN_SCORE, 16, 2, 1, 1);

        rrnaMIN_BITSCORE = new QSpinBox(SettingsTab);
        rrnaMIN_BITSCORE->setObjectName(QString::fromUtf8("rrnaMIN_BITSCORE"));
        rrnaMIN_BITSCORE->setMinimum(1);
        rrnaMIN_BITSCORE->setMaximum(10000);

        gridLayout->addWidget(rrnaMIN_BITSCORE, 30, 5, 1, 1);

        label_16 = new QLabel(SettingsTab);
        label_16->setObjectName(QString::fromUtf8("label_16"));

        gridLayout->addWidget(label_16, 26, 0, 1, 1);

        line_4 = new QFrame(SettingsTab);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_4, 33, 0, 1, 7);

        annotationALGORITHM = new QComboBox(SettingsTab);
        annotationALGORITHM->setObjectName(QString::fromUtf8("annotationALGORITHM"));
        annotationALGORITHM->setEditable(false);
        annotationALGORITHM->setMaxCount(10);

        gridLayout->addWidget(annotationALGORITHM, 10, 2, 1, 1);

        label_9 = new QLabel(SettingsTab);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 11, 0, 1, 1);

        label_22 = new QLabel(SettingsTab);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        QFont font1;
        font1.setPointSize(24);
        label_22->setFont(font1);

        gridLayout->addWidget(label_22, 0, 0, 1, 3);

        line_3 = new QFrame(SettingsTab);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 23, 0, 1, 7);

        ptools_settingsTAXONOMIC_PRUNING = new QComboBox(SettingsTab);
        ptools_settingsTAXONOMIC_PRUNING->setObjectName(QString::fromUtf8("ptools_settingsTAXONOMIC_PRUNING"));

        gridLayout->addWidget(ptools_settingsTAXONOMIC_PRUNING, 36, 2, 1, 1);

        label_12 = new QLabel(SettingsTab);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout->addWidget(label_12, 16, 0, 1, 1);

        label_14 = new QLabel(SettingsTab);
        label_14->setObjectName(QString::fromUtf8("label_14"));

        gridLayout->addWidget(label_14, 14, 3, 1, 1);

        annotationDBSButton = new QPushButton(SettingsTab);
        annotationDBSButton->setObjectName(QString::fromUtf8("annotationDBSButton"));

        gridLayout->addWidget(annotationDBSButton, 12, 2, 1, 1);

        label_18 = new QLabel(SettingsTab);
        label_18->setObjectName(QString::fromUtf8("label_18"));

        gridLayout->addWidget(label_18, 30, 0, 1, 1);

        QWidget::setTabOrder(quality_controlMIN_LENGTH, quality_controlDELETE_REPLICATES);
        QWidget::setTabOrder(quality_controlDELETE_REPLICATES, annotationALGORITHM);
        QWidget::setTabOrder(annotationALGORITHM, annotationDBS);
        QWidget::setTabOrder(annotationDBS, annotationMIN_BSR);
        QWidget::setTabOrder(annotationMIN_BSR, annotationMAX_HITS);
        QWidget::setTabOrder(annotationMAX_HITS, annotationMIN_SCORE);
        QWidget::setTabOrder(annotationMIN_SCORE, annotationMIN_LENGTH);
        QWidget::setTabOrder(annotationMIN_LENGTH, orf_predictionMIN_LENGTH);
        QWidget::setTabOrder(orf_predictionMIN_LENGTH, orf_predictionALGORITHM);
        QWidget::setTabOrder(orf_predictionALGORITHM, rrnaREFDBS);
        QWidget::setTabOrder(rrnaREFDBS, rrnaMIN_IDENTITY);
        QWidget::setTabOrder(rrnaMIN_IDENTITY, rrnaMIN_BITSCORE);
        QWidget::setTabOrder(rrnaMIN_BITSCORE, rrnaMAX_EVALUE);
        QWidget::setTabOrder(rrnaMAX_EVALUE, ptools_settingsTAXONOMIC_PRUNING);

        retranslateUi(SettingsTab);

        quality_controlDELETE_REPLICATES->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingsTab);
    } // setupUi

    void retranslateUi(QWidget *SettingsTab)
    {
        SettingsTab->setWindowTitle(QApplication::translate("SettingsTab", "Form", 0, QApplication::UnicodeUTF8));
        rrnaREFDBSButton->setText(QApplication::translate("SettingsTab", "Select DBs", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("SettingsTab", "Maximum EValue", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("SettingsTab", "Mininum Length", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("SettingsTab", "ORF Prediction", 0, QApplication::UnicodeUTF8));
        quality_controlDELETE_REPLICATES->clear();
        quality_controlDELETE_REPLICATES->insertItems(0, QStringList()
         << QApplication::translate("SettingsTab", "yes", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsTab", "no", 0, QApplication::UnicodeUTF8)
        );
        label_8->setText(QApplication::translate("SettingsTab", "Algorithm", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("SettingsTab", "Taxonomic Pruning", 0, QApplication::UnicodeUTF8));
        label_19->setText(QApplication::translate("SettingsTab", "Mininum Bitscore", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("SettingsTab", "Mininum Length", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("SettingsTab", "Algorithm", 0, QApplication::UnicodeUTF8));
        orf_predictionALGORITHM->clear();
        orf_predictionALGORITHM->insertItems(0, QStringList()
         << QApplication::translate("SettingsTab", "prodigal", 0, QApplication::UnicodeUTF8)
        );
        annotationDBSWarning->setText(QApplication::translate("SettingsTab", "At least one database must be specified!", 0, QApplication::UnicodeUTF8));
        annotationMAX_EVALUE->setHtml(QApplication::translate("SettingsTab", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.000001</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("SettingsTab", "Maximum EValue", 0, QApplication::UnicodeUTF8));
        rrnaREFDBSWarning->setText(QApplication::translate("SettingsTab", "At least one database must be specified!", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("SettingsTab", "rRNA Annotation Parameters", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("SettingsTab", "ORF Annotation Parameters", 0, QApplication::UnicodeUTF8));
        rrnaMIN_IDENTITY->setSuffix(QApplication::translate("SettingsTab", "%", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SettingsTab", "Quality Control", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("SettingsTab", "Pathway Tools Settings", 0, QApplication::UnicodeUTF8));
        rrnaMAX_EVALUE->setHtml(QApplication::translate("SettingsTab", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Lucida Grande'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">0.000001</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("SettingsTab", "Delete Replicates", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SettingsTab", "Mininum Length", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("SettingsTab", "Mininum BSR", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("SettingsTab", "Databases", 0, QApplication::UnicodeUTF8));
        annotationALGORITHM->clear();
        annotationALGORITHM->insertItems(0, QStringList()
         << QApplication::translate("SettingsTab", "blast", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsTab", "last", 0, QApplication::UnicodeUTF8)
        );
        label_9->setText(QApplication::translate("SettingsTab", "Databases", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("SettingsTab", "Run Parameters", 0, QApplication::UnicodeUTF8));
        ptools_settingsTAXONOMIC_PRUNING->clear();
        ptools_settingsTAXONOMIC_PRUNING->insertItems(0, QStringList()
         << QApplication::translate("SettingsTab", "yes", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SettingsTab", "no", 0, QApplication::UnicodeUTF8)
        );
        label_12->setText(QApplication::translate("SettingsTab", "Mininum Score", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("SettingsTab", "Maximum Hits", 0, QApplication::UnicodeUTF8));
        annotationDBSButton->setText(QApplication::translate("SettingsTab", "Select DBs", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("SettingsTab", "Mininum Identity         ", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SettingsTab: public Ui_SettingsTab {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSTAB_H
