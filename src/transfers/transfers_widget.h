#ifndef TRANSFERS_WIDGET_H
#define TRANSFERS_WIDGET_H

#include <QWidget>
#include <QMenu>
#include <QStringList>

#include "ui_transfers_widget.h"
#include "qed2khandle.h"

class TransferModel;

class transfers_widget : public QWidget, private Ui::transfers_widget {
    Q_OBJECT
    
public:
    explicit transfers_widget(QWidget *parent = 0);
    ~transfers_widget();
private slots:
    void on_actionStart_triggered();

    void on_actionPause_triggered();

    void on_actionRemove_triggered();

    void displayListMenu(const QPoint&);
    void on_actionED2K_link_triggered();

    void on_actionSeries_download_triggered();

    void on_actionFirst_and_last_pieces_first_triggered();

    void on_actionPreview_triggered();

    void on_actionOpen_destination_folder_triggered();

private:
    QString getHashFromRow(int row) const;
    QStringList getSelectedHashes() const;
    QModelIndex mapToSource(const QModelIndex& index) const;
    TransferModel* model;
    QMenu* menu;
};

#endif // TRANSFERS_WIDGET_H
