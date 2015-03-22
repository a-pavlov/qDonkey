#include "peermodel.h"

PeerModel::PeerModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int PeerModel::rowCount(const QModelIndex& parent) const {
    return 0;
}

int PeerModel::columnCount(const QModelIndex& parent) const {
    return COL_COUNT;
}

QVariant PeerModel::data(const QModelIndex& index, int role) const {
    QVariant result;

    if (!index.isValid())
        return result;
    return result;
}

QVariant PeerModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractListModel::headerData(section, orientation, role);

    switch(section) {
        case IP:        return tr("IP");
        case CLIENT:    return tr("Client");
        case FILE:      return tr("File name");
        case PROGRESS:  return tr("Progress");
        case SPEED:     return tr("Speed");
        case TOTAL:     return tr("Bytes");
        default:
            Q_ASSERT(false);
            break;
    }

    return QVariant();
}
