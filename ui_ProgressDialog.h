/********************************************************************************
** Form generated from reading UI file 'ProgressDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSDIALOG_H
#define UI_PROGRESSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProgressDialog
{
public:
    QGridLayout *gridLayout;
    QProgressBar *progressBar;
    QLabel *progressLabel_2;
    QFrame *line_2;
    QLabel *summaryLabel;
    QProgressBar *globalProgressBar;
    QLabel *logLabel;
    QPushButton *cancelButton;
    QTableWidget *summaryTable;
    QTextEdit *standardOut;
    QTextBrowser *logBrowser;
    QLabel *progressLabel;
    QPushButton *hideButton;

    void setupUi(QWidget *ProgressDialog)
    {
        if (ProgressDialog->objectName().isEmpty())
            ProgressDialog->setObjectName(QString::fromUtf8("ProgressDialog"));
        ProgressDialog->resize(775, 1025);
        gridLayout = new QGridLayout(ProgressDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        progressBar = new QProgressBar(ProgressDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        gridLayout->addWidget(progressBar, 1, 0, 1, 2);

        progressLabel_2 = new QLabel(ProgressDialog);
        progressLabel_2->setObjectName(QString::fromUtf8("progressLabel_2"));
        QFont font;
        font.setPointSize(24);
        progressLabel_2->setFont(font);

        gridLayout->addWidget(progressLabel_2, 2, 0, 1, 1);

        line_2 = new QFrame(ProgressDialog);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 5, 0, 1, 2);

        summaryLabel = new QLabel(ProgressDialog);
        summaryLabel->setObjectName(QString::fromUtf8("summaryLabel"));
        summaryLabel->setFont(font);

        gridLayout->addWidget(summaryLabel, 6, 0, 1, 1);

        globalProgressBar = new QProgressBar(ProgressDialog);
        globalProgressBar->setObjectName(QString::fromUtf8("globalProgressBar"));
        globalProgressBar->setValue(24);

        gridLayout->addWidget(globalProgressBar, 3, 0, 1, 2);

        logLabel = new QLabel(ProgressDialog);
        logLabel->setObjectName(QString::fromUtf8("logLabel"));
        logLabel->setFont(font);

        gridLayout->addWidget(logLabel, 6, 1, 1, 1);

        cancelButton = new QPushButton(ProgressDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cancelButton->sizePolicy().hasHeightForWidth());
        cancelButton->setSizePolicy(sizePolicy);
        cancelButton->setMinimumSize(QSize(200, 0));
        cancelButton->setMaximumSize(QSize(300, 16777215));

        gridLayout->addWidget(cancelButton, 4, 1, 1, 1, Qt::AlignRight);

        summaryTable = new QTableWidget(ProgressDialog);
        if (summaryTable->columnCount() < 1)
            summaryTable->setColumnCount(1);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        summaryTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        if (summaryTable->rowCount() < 17)
            summaryTable->setRowCount(17);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(0, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(1, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(2, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(3, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(4, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(5, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(6, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(7, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(8, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(9, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(10, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(11, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(12, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(13, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(14, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(15, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        summaryTable->setVerticalHeaderItem(16, __qtablewidgetitem17);
        summaryTable->setObjectName(QString::fromUtf8("summaryTable"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(summaryTable->sizePolicy().hasHeightForWidth());
        summaryTable->setSizePolicy(sizePolicy1);
        summaryTable->horizontalHeader()->setDefaultSectionSize(100);

        gridLayout->addWidget(summaryTable, 7, 0, 1, 1);

        standardOut = new QTextEdit(ProgressDialog);
        standardOut->setObjectName(QString::fromUtf8("standardOut"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(standardOut->sizePolicy().hasHeightForWidth());
        standardOut->setSizePolicy(sizePolicy2);
        standardOut->setMinimumSize(QSize(750, 200));
        standardOut->setReadOnly(true);

        gridLayout->addWidget(standardOut, 8, 0, 1, 2);

        logBrowser = new QTextBrowser(ProgressDialog);
        logBrowser->setObjectName(QString::fromUtf8("logBrowser"));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(logBrowser->sizePolicy().hasHeightForWidth());
        logBrowser->setSizePolicy(sizePolicy3);
        logBrowser->setMinimumSize(QSize(400, 228));

        gridLayout->addWidget(logBrowser, 7, 1, 1, 1);

        progressLabel = new QLabel(ProgressDialog);
        progressLabel->setObjectName(QString::fromUtf8("progressLabel"));
        progressLabel->setFont(font);

        gridLayout->addWidget(progressLabel, 0, 0, 1, 2);

        hideButton = new QPushButton(ProgressDialog);
        hideButton->setObjectName(QString::fromUtf8("hideButton"));
        sizePolicy.setHeightForWidth(hideButton->sizePolicy().hasHeightForWidth());
        hideButton->setSizePolicy(sizePolicy);
        hideButton->setMinimumSize(QSize(200, 32));
        hideButton->setMaximumSize(QSize(300, 32));

        gridLayout->addWidget(hideButton, 4, 0, 1, 1);


        retranslateUi(ProgressDialog);

        QMetaObject::connectSlotsByName(ProgressDialog);
    } // setupUi

    void retranslateUi(QWidget *ProgressDialog)
    {
        ProgressDialog->setWindowTitle(QApplication::translate("ProgressDialog", "Form", 0, QApplication::UnicodeUTF8));
        progressLabel_2->setText(QApplication::translate("ProgressDialog", "Global Progress", 0, QApplication::UnicodeUTF8));
        summaryLabel->setText(QApplication::translate("ProgressDialog", "Execution Summary", 0, QApplication::UnicodeUTF8));
        logLabel->setText(QApplication::translate("ProgressDialog", "Progress Log", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("ProgressDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = summaryTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ProgressDialog", "Result", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = summaryTable->verticalHeaderItem(0);
        ___qtablewidgetitem1->setText(QApplication::translate("ProgressDialog", "PREPROCESS_FASTA", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = summaryTable->verticalHeaderItem(1);
        ___qtablewidgetitem2->setText(QApplication::translate("ProgressDialog", "ORF_PREDICTION", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = summaryTable->verticalHeaderItem(2);
        ___qtablewidgetitem3->setText(QApplication::translate("ProgressDialog", "GFF_TO_AMINO", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = summaryTable->verticalHeaderItem(3);
        ___qtablewidgetitem4->setText(QApplication::translate("ProgressDialog", "FILTERED_FASTA", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = summaryTable->verticalHeaderItem(4);
        ___qtablewidgetitem5->setText(QApplication::translate("ProgressDialog", "COMPUTE_REFSCORE", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = summaryTable->verticalHeaderItem(5);
        ___qtablewidgetitem6->setText(QApplication::translate("ProgressDialog", "BLAST_REFDB", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = summaryTable->verticalHeaderItem(6);
        ___qtablewidgetitem7->setText(QApplication::translate("ProgressDialog", "PARSE_BLAST", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = summaryTable->verticalHeaderItem(7);
        ___qtablewidgetitem8->setText(QApplication::translate("ProgressDialog", "SCAN_rRNA", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = summaryTable->verticalHeaderItem(8);
        ___qtablewidgetitem9->setText(QApplication::translate("ProgressDialog", "STATS_rRNA", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = summaryTable->verticalHeaderItem(9);
        ___qtablewidgetitem10->setText(QApplication::translate("ProgressDialog", "SCAN_tRNA", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem11 = summaryTable->verticalHeaderItem(10);
        ___qtablewidgetitem11->setText(QApplication::translate("ProgressDialog", "ANNOTATE", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem12 = summaryTable->verticalHeaderItem(11);
        ___qtablewidgetitem12->setText(QApplication::translate("ProgressDialog", "PATHOLOGIC_INPUT", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem13 = summaryTable->verticalHeaderItem(12);
        ___qtablewidgetitem13->setText(QApplication::translate("ProgressDialog", "GENBANK_FILE", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem14 = summaryTable->verticalHeaderItem(13);
        ___qtablewidgetitem14->setText(QApplication::translate("ProgressDialog", "CREATE_REPORT_FILES", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem15 = summaryTable->verticalHeaderItem(14);
        ___qtablewidgetitem15->setText(QApplication::translate("ProgressDialog", "MLTREEMAP_CALCULATION", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem16 = summaryTable->verticalHeaderItem(15);
        ___qtablewidgetitem16->setText(QApplication::translate("ProgressDialog", "MLTREEMAP_IMAGEMAKER", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem17 = summaryTable->verticalHeaderItem(16);
        ___qtablewidgetitem17->setText(QApplication::translate("ProgressDialog", "PATHOLOGIC", 0, QApplication::UnicodeUTF8));
        progressLabel->setText(QApplication::translate("ProgressDialog", "Progress", 0, QApplication::UnicodeUTF8));
        hideButton->setText(QApplication::translate("ProgressDialog", "Show/Hide Advanced Details", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProgressDialog: public Ui_ProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSDIALOG_H
