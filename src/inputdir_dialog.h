#ifndef INPUTDIR_DIALOG_H
#define INPUTDIR_DIALOG_H

#include <QDialog>
#include "ui_inputdir_dialog.h"

class inputdir_dialog : public QDialog, private Ui::inputdir_dialog
{
    Q_OBJECT
    
public:
    explicit inputdir_dialog(QWidget *parent = 0);
    ~inputdir_dialog();
    
public slots:
    void done(int);
private slots:
    void on_toolButton_clicked();
    void on_testInputDir_textChanged(const QString &arg1);
};

#endif // INPUTDIR_DIALOG_H
