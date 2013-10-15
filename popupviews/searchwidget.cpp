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
    keyword = this->findChild<QLineEdit *>("keyword");
    columnSelect = this->findChild<QComboBox *>("columnSelect");
    okAndCancel = this->findChild<QDialogButtonBox *>("okAndCancel");
    caseSensitive = this->findChild<QCheckBox *>("caseSensitive");

    QStringList headers = this->td->getHeaders();

    this->index.append(0); // this is for the ALL option
    for( int i=0; i < headers.size(); i++) {
       // if( this->td->getFieldType(i) == STRING) {
            columnSelect->addItem(headers.at(i));
            this->index.append(i+1);
        //}
    }


    connect(okAndCancel, SIGNAL(accepted()), this, SLOT(querySet()));
    connect(okAndCancel,SIGNAL(rejected()), this, SLOT(hide()));
    //connect(this, SIGNAL(lookUp(QString,int, bool)), this->td, SLOT(searchQuery(QString,int, bool)));
}


void SearchWidget::querySet(){
    QString query = keyword->text();
    int columnIndex = this->index.at(columnSelect->currentIndex());

    emit lookUp(query, columnIndex, this->caseSensitive->isChecked());
    this->hide();
}

SearchWidget::~SearchWidget()
{
    delete ui;
}



