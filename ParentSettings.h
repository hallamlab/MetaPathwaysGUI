#ifndef ParentSettings_H
#define ParentSettings_H

#include <QWidget>
#include "AdvancedSettingsTab.h"
#include "SettingsTab.h"

namespace Ui {
class ParentSettings;
}

class ParentSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit ParentSettings(QWidget *parent = 0);
    ~ParentSettings();
    
public slots:
    void openParameterSetup();
    void closeWindow();

private:
    Ui::ParentSettings *ui;
    AdvancedSettingsTab *AdvancedSettings;
    SettingsTab *basicSettingsTab;

};

#endif // ParentSettings_H
