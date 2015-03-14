#ifndef TRANSFERS_WIDGET_H
#define TRANSFERS_WIDGET_H

#include <QWidget>

#include "ui_transfers_widget.h"

class transfers_widget : public QWidget, private Ui::transfers_widget {
    Q_OBJECT
    
public:
    explicit transfers_widget(QWidget *parent = 0);
    ~transfers_widget();
};

#endif // TRANSFERS_WIDGET_H
