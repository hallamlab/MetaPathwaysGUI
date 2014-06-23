#include "searchwidget.h"
#include "ui_searchwidget.h"

SearchWidget::SearchWidget(TableData* td, QWidget *parent):
   QWidget(parent),
   ui(new Ui::SearchWidget)
{
    ui->setupUi(this);
    this->td = new ExportSource(td);
    this->createWidget();
}

SearchWidget::SearchWidget(HTableData* td, QWidget *parent) :
   QWidget(parent),
   ui(new Ui::SearchWidget)
{
    ui->setupUi(this);
    this->td = new ExportSource(td);
    this->createWidget();
}

void SearchWidget::createWidget()
{
    keyword_1 = this->findChild<QLineEdit *>("keyword_1");
    columnSelect_1 = this->findChild<QComboBox *>("columnSelect_1");
    andORlabel_1 = this->findChild<QLabel *>("andORlabel_1");

    keyword_2 = this->findChild<QLineEdit *>("keyword_2");
    columnSelect_2 = this->findChild<QComboBox *>("columnSelect_2");
    andORlabel_2 = this->findChild<QLabel *>("andORlabel_2");

    keyword_3 = this->findChild<QLineEdit *>("keyword_3");
    columnSelect_3 = this->findChild<QComboBox *>("columnSelect_3");
    andORlabel_3 = this->findChild<QLabel *>("andORlabel_3");

    keyword_4 = this->findChild<QLineEdit *>("keyword_4");
    columnSelect_4 = this->findChild<QComboBox *>("columnSelect_4");
    andORlabel_4 = this->findChild<QLabel *>("andORlabel_4");

    andOR = this->findChild<QCheckBox *>("andOR");

    okAndCancel = this->findChild<QDialogButtonBox *>("okAndCancel");
    caseSensitive = this->findChild<QCheckBox *>("caseSensitive");

    QStringList headers = this->td->getHeaders();

    this->index.append(0); // this is for the ALL option

    for( int i=0; i < headers.size(); i++) {
       // if( this->td->getFieldType(i) == STRING) {
            columnSelect_1->addItem(headers.at(i));
            columnSelect_2->addItem(headers.at(i));
            columnSelect_3->addItem(headers.at(i));
            columnSelect_4->addItem(headers.at(i));
            this->index.append(i+1);
        //}
    }

    connect(andOR, SIGNAL( clicked(bool) ), this, SLOT(toggleANDOR(bool)));
    connect(okAndCancel, SIGNAL(accepted()), this, SLOT(querySet()));
    connect(okAndCancel,SIGNAL(rejected()), this, SLOT(hide()));

    if( this->td->getIndex() == 0)
       connect(this, SIGNAL(lookUp(QString,int, QString, int, QString, int, QString, int, OPTYPE, bool)), this->td->getTableDataPointer(), SLOT(searchQuery(QString,int, QString, int, QString, int, QString, int, OPTYPE, bool)));

    if( this->td->getIndex() == 1)
       connect(this, SIGNAL(lookUp(QString,int, QString, int, QString, int, QString, int, OPTYPE, bool)), this->td->getHTableDataPointer(), SLOT(searchQuery(QString,int, QString, int, QString, int, QString, int, OPTYPE, bool)));
}

void SearchWidget::toggleANDOR(bool checked) {
    if( checked ) {
        this->andORlabel_1->setText("AND");
        this->andORlabel_2->setText("AND");
        this->andORlabel_3->setText("AND");
        this->andORlabel_4->setText("AND");
    }
    else {
        this->andORlabel_1->setText("OR");
        this->andORlabel_2->setText("OR");
        this->andORlabel_3->setText("OR");
        this->andORlabel_4->setText("OR");
    }
}

void SearchWidget::querySet(){
    emit lookUp(
                keyword_1->text(), this->index.at(columnSelect_1->currentIndex()),
                keyword_2->text(), this->index.at(columnSelect_2->currentIndex()),
                keyword_3->text(), this->index.at(columnSelect_3->currentIndex()),
                keyword_4->text(), this->index.at(columnSelect_4->currentIndex()),
                (this->andOR->isChecked() ? AND : OR),
                this->caseSensitive->isChecked()
                );
    this->hide();
}

SearchWidget::~SearchWidget()
{
    delete ui;
}



