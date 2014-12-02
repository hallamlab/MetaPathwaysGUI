#include "statsqgroupbox.h"

StatsQGroupBox::StatsQGroupBox(QWidget *parent) :
    QGroupBox(parent)
{
}


void StatsQGroupBox::setModel(RunDataStats *model) {
    this->model = model;
    this->statsTableView->setModel(this->model);
}


void StatsQGroupBox::exportTable() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"));
    QFile outFile(fileName);

    int numRows = this->model->rowCount();
    int numCol = this->model->columnCount();

    QModelIndex index;

    if (outFile.open(QIODevice::WriteOnly |  QIODevice::Text)) {
        QTextStream out(&outFile);
        QString outStr;
        for(int col = 0; col < numCol; col ++) {
            outStr  +=  QString("\t") + this->model->headerData(col, Qt::Horizontal, Qt::DisplayRole).toString();
        }
        outStr += QString("\n");
        out << outStr;

        for(int row = 0; row < numRows; row++ ) {
            outStr =  this->model->headerData(row, Qt::Vertical, Qt::DisplayRole).toString();
            for(int col =0; col < numCol; col++) {
                index = this->model->index(row, col);
                outStr += QString("\t") + this->model->data(index, Qt::DisplayRole).toString();
             }
            outStr += QString("\n");
            out << outStr;
        }
        outFile.close();
    }
}
