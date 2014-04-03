#ifndef PROGRESSVIEW_H
#define PROGRESSVIEW_H
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QLabel>
#include <QString>
#include <QWidget>
#include <QApplication>


class ProgressView:public QWidget {
    Q_OBJECT
public:
    ProgressView(QWidget *parent = 0);
    ProgressView(QString text, unsigned int i, unsigned int j, QWidget *parent );
    void updateprogress(unsigned int i);
private:
    QVBoxLayout* layout;
    QProgressBar  *progressBar;
    QLabel *message;
};

#endif // PROGRESSVIEW_H
