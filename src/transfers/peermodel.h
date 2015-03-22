#ifndef PEERMODEL_H
#define PEERMODEL_H

#include <QAbstractListModel>
#include "qed2khandle.h"
#include "qed2ksession.h"

class PeerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        SortRole = Qt::UserRole + 1
    };

    enum Columns { PM_IP, PM_CLIENT, PM_FILE, PM_PROGRESS, PM_SPEED, PM_TOTAL, COL_COUNT };

    explicit PeerModel(QObject *parent = 0);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void populate();
private:
    QList<PeerInfo> m_peers;
    const PeerInfo& at(const QModelIndex&) const;
    quint64 speed(const QModelIndex&) const;
    quint64 total(const QModelIndex&) const;
signals:
    
public slots:
    
};

#endif // PEERMODEL_H
