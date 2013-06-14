#ifndef AdvancedSettingsTab_H
#define AdvancedSettingsTab_H

#include <QWidget>

namespace Ui {
class AdvancedSettingsTab;
}

class AdvancedSettingsTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit AdvancedSettingsTab(QWidget *parent = 0);
    ~AdvancedSettingsTab();
    
private:
    Ui::AdvancedSettingsTab *ui;
};

#endif // AdvancedSettingsTab_H
