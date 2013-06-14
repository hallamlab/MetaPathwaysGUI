#ifndef RemoteForm_H
#define RemoteForm_H

#include <QWidget>

namespace Ui {
class RemoteForm;
}

class RemoteForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit RemoteForm(QWidget *parent = 0);
    ~RemoteForm();

public slots:
    void openContinue();
    void closeWindow();

private:
    Ui::RemoteForm *ui;
};

#endif // RemoteForm_H
