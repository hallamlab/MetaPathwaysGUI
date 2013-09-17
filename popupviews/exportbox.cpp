#include "exportbox.h"


ExportBox::ExportBox(TableData* td, QWidget *parent)
    // : QWidget(parent)
 {
     QGridLayout *grid = new QGridLayout;
     this->td = td;

     createNonExclusiveGroup(grid);

     setLayout(grid);

     setWindowTitle(tr("Group Boxes"));
     resize(480, 320);
 }


bool compareColumns(const EXPORT_SELECT a, const EXPORT_SELECT b) {
    //qDebug() << a->strTemp << "  " << b->strTemp << "  " << QString::compare(a->strTemp,   b->strTemp);
    return a.rank < b.rank;
}


 void ExportBox::clickedChoice() {

     qDebug() << " You clicked ";
     QList<EXPORT_SELECT> cols;

     unsigned int numSel =0;
     for(unsigned int i=0; i < Columns.size(); i++)
         if(Columns[i].checkbox->isChecked() )
             numSel++;

     for(unsigned int i=0; i < Columns.size(); i++) {
         if(Columns[i].checkbox->isChecked() ) {
             if( Columns[i].rank == -1) Columns[i].rank = numSel;
             cols.append(Columns[i]);
         }
         else {
             Columns[i].checkbox->setText( Columns[i].name);
             Columns[i].rank = -1;
         }
     }
     qSort(cols.begin(), cols.end(), compareColumns);


     for(unsigned int i=0; i< numSel; i++) {
         cols[i].rank = i;
     }

     foreach(EXPORT_SELECT a, cols) {
         a.checkbox->setText( a.name + " (" + QString::number(a.rank) + ")");
     }

 }

 QGroupBox *ExportBox::createNonExclusiveGroup(QGridLayout *grid)
 {
     QGroupBox *groupBox = new QGroupBox(tr("Select Columns to Export"));
     groupBox->setFlat(true);

     for(unsigned int i=0; i < this->td->getHeaders().size(); i++ ) {
         QCheckBox *checkBox1 = new QCheckBox(this->td->getHeader(i));
         connect(checkBox1, SIGNAL( clicked()), this, SLOT(clickedChoice()) );
         grid->addWidget(checkBox1,(int)i/1, i%1);
         EXPORT_SELECT s;
         s.checkbox=checkBox1;
         s.rank = -1;
         s.name = this->td->getHeader(i);
         s.col = i;

         this->Columns.append(s);
     }
     groupBox->setLayout(grid);
     return groupBox;
 }



