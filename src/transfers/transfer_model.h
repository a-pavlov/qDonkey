#ifndef TRANSFER_MODEL_H
#define TRANSFER_MODEL_H

#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include <QTimer>

#include "transfermodel_item.h"
#include "qtlibed2k/qed2ksession.h"

class TransferModel : public QAbstractListModel {
    Q_OBJECT
    Q_DISABLE_COPY(TransferModel)

public:
    explicit TransferModel(QObject *parent = 0);
    ~TransferModel();
    inline int rowCount(const QModelIndex& index = QModelIndex()) const { Q_UNUSED(index); return m_transfers.size(); }
    int columnCount(const QModelIndex &parent=QModelIndex()) const { Q_UNUSED(parent); return TransferModelItem::TM_END; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int transferRow(const QString &hash) const;
    QString transferHash(int row) const;
    void setRefreshInterval(int refreshInterval);
    Qt::ItemFlags flags(const QModelIndex &index) const;
signals:
    void transferAdded(TransferModelItem *transferItem);
    void transferAboutToBeRemoved(TransferModelItem *transferItem);

public slots:
    void removeTransfer(const QString& hash);

private slots:
    void addTransfer(const QED2KHandle& h);
    void handleTransferUpdate(const QED2KHandle& h);
    void notifyTransferChanged(int row);
    void forceModelRefresh();
    void handleTransferAboutToBeRemoved(const QED2KHandle& h, bool);
    void processActiveTransfers();
private:
    void beginInsertTransfer(int row);
    void endInsertTransfer();
    void beginRemoveTransfer(int row);
    void endRemoveTransfer();
    void processUncheckedTransfers();
private:
    int m_refreshInterval;
    QList<TransferModelItem*> m_transfers;
    QString m_path;
    QList<QED2KHandle> m_uncheckedTransfers;
    QTimer m_refreshTimer;
};

#endif // TRANSFER_MODEL_H
