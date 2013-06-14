#ifndef ProgressDialog_H
#define ProgressDialog_H

#include <QWidget>

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QWidget
{
    Q_OBJECT
    
public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();
    
private:
    Ui::ProgressDialog *ui;
};

#endif // ProgressDialog_H
