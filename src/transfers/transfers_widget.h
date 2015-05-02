#ifndef TRANSFERS_WIDGET_H
#define TRANSFERS_WIDGET_H

#include <QWidget>
#include <QMenu>
#include <QStringList>
#include <QTimer>

#include "ui_transfers_widget.h"
#include "qed2khandle.h"

class TransferModel;
class PeerModel;
class QSortFilterProxyModel;

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

    void refreshModels();

    void trSortChanged(int logicalIndex, Qt::SortOrder order);
    void pSortChanged(int logicalIndex, Qt::SortOrder order);
    void on_actionShow_all_transfers_triggered();
    void displayHSMenu(const QPoint&);
    void on_trView_doubleClicked(const QModelIndex &index);
protected:
    void keyPressEvent ( QKeyEvent * event) ;
private:
    QString getHashFromRow(int row) const;
    QStringList getSelectedHashes() const;
    QModelIndex mapToSource(const QModelIndex& index) const;
    TransferModel* tmodel;
    PeerModel* pmodel;
    QSortFilterProxyModel* tsort_model;
    QSortFilterProxyModel* psort_model;
    QMenu* menu;
    QTimer m_refreshTimer;
};

#endif // TRANSFERS_WIDGET_H
