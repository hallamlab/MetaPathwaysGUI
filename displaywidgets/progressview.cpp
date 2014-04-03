#include "displaywidgets/progressview.h"

ProgressView::ProgressView(QWidget *parent)
{
}


ProgressView::ProgressView(QString text, unsigned int i , unsigned int j, QWidget *parent) {


    layout = new QVBoxLayout(this);

    if(i < j ) {
        progressBar = new QProgressBar(parent);
       progressBar->setRange(i, j);
       progressBar->setAlignment(Qt::AlignCenter);
       progressBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
       layout->addWidget(progressBar);
    }

    message = new QLabel();
    message->setTextFormat(Qt::RichText);
    message->setText( QString("<FONT COLOR='#000000' FONT SIZE = 6>") +  text  + QString("</>") );
    message->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    layout->addWidget(message);

    this->setStyleSheet("QWidget {padding:0px; margin-left:0px;  border:1px grey; border-style:ridge}");
    this->setContentsMargins(0,0,0,0);
    layout->setMargin(0);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->show();
}

void ProgressView::updateprogress(unsigned int i) {
    this->progressBar->setValue(i);
    qApp->processEvents();
    this->progressBar->update();

}
