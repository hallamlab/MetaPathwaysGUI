#ifndef PROGRESSVIEW_H
#define PROGRESSVIEW_H
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QString>
#include <QWidget>


class ProgressView:public QWidget {
    Q_OBJECT
public:
    ProgressView();
    ProgressView(QString text, unsigned int i, unsigned int j );
    void updateprogress(unsigned int i);
private:
    QVBoxLayout* layout;
    QProgressBar  *progressBar;
    QLabel *message;
};

#endif // PROGRESSVIEW_H
