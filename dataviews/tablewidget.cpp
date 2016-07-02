#include "tablewidget.h"

TableWidget::TableWidget(QWidget *parent) :
    QTableWidget(parent)
{
}


void TableWidget::keyPressEvent(QKeyEvent * event)
{
    // if ctrl_C
    QAbstractItemModel* model = this->model();

    if(event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier) ) {
        QModelIndexList cells = selectedIndexes();

        qSort(cells); // Necessary, otherwise they are in column order
        QString text;

        int numrows = 0;
        if(!cells.empty() ) {
           QString header =model->headerData(cells[0].column() ,Qt::Horizontal ).toString();
           numrows = header.split('\n').size();
        }

        QList<int> colList;
        int curRow = -1;
        foreach (const QModelIndex& cell, cells) {
            if(curRow!= - 1 && curRow!= cell.row()) break;
            colList.append(cell.column());
            curRow = cell.row();
        }

        for(int i = 0; i < 3; i ++) {
            foreach (int col, colList) {
               text += "\t";
               QStringList components= model->headerData(col ,Qt::Horizontal ).toString().split("\n");
               text += components[i];
           }
           text +="\n";
        }

        bool firstRow = true;
        int currentRow = -1; // To determine when to insert newlines
        foreach (const QModelIndex& cell, cells) {
            if ( firstRow) {
                firstRow=false;
                text += model->headerData(cell.row() ,Qt::Vertical ).toString() + "\t";
            } else if (cell.row() != currentRow) {
                // New row
                text += '\n';
                text += model->headerData(cell.row() ,Qt::Vertical ).toString() +"\t";
            } else {
                // Next cell
                text += '\t';
            }

            currentRow = cell.row();
            text += cell.data().toString();
        }
    //    QApplication::clipboard()->setText(text);
      //  event->ignore();
    }
    else
        QTableView::keyPressEvent(event);
}
