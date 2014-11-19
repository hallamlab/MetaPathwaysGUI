#ifndef SETUPSTATUS_H
#define SETUPSTATUS_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QLine>
#include <QSpacerItem>
#include <QString>
#include <QStringList>

class SetUpStatus : public QWidget
{
    Q_OBJECT
public:
    explicit SetUpStatus(QStringList messages, QWidget *parent = 0 );
private:
    void createWidget();
    void addMessage();


    QWidget *parent;
    QScrollArea * scroll;
    QStringList messages;
signals:
    void saveConfigFile();
    
public slots:

private slots:
    void saveConfig();
    void destroy();
};

#endif // SETUPSTATUS_H
