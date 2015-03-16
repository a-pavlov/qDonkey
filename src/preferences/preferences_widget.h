#ifndef PREFERENCES_WIDGET_H
#define PREFERENCES_WIDGET_H

#include <QWidget>

#include "ui_preferences_widget.h"

class preferences_widget : public QWidget, private Ui::preferences_widget {
    Q_OBJECT
    
public:
    explicit preferences_widget(QWidget *parent = 0);
    ~preferences_widget();

private:
    void restoreFromPref();
private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void enableApplyButtons();
    void disableApplyButtons();
};

#endif // PREFERENCES_WIDGET_H
