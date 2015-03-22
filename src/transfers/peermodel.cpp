#include "peermodel.h"
#include "res.h"

PeerModel::PeerModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int PeerModel::rowCount(const QModelIndex& parent) const {
    return m_peers.size();
}

int PeerModel::columnCount(const QModelIndex& parent) const {
    return COL_COUNT;
}

QVariant PeerModel::data(const QModelIndex& index, int role) const {
    QVariant result;

    if (!index.isValid())
        return result;

    switch(role) {
    case Qt::DisplayRole:  {
        switch(index.column()) {
            case PM_IP:        return at(index).m_address;
            case PM_CLIENT:    return at(index).m_client;
            case PM_FILE:      return at(index).m_file;
            case PM_PROGRESS:  {
                QString res = QString::number(at(index).m_progress, 'f', 1)+"%";
                return res;
            }

            case PM_SPEED:     {
                QString res = misc::friendlyUnit(speed(index))+tr("/s");
                return res;
            }
            case PM_TOTAL:     return misc::friendlyUnit(total(index));
            default:
            break;
            }
        }
        break;
    case Qt::DecorationRole: {
        if (index.column() == PM_IP)
            if (at(index).m_speed_down > 0) return QIcon(res::downloading());
            if (at(index).m_speed_up > 0) return QIcon(res::uploading());
        }
        break;
    case Qt::ForegroundRole: {
            if (at(index).m_speed_down > 0) return QColor("green");
            if (at(index).m_speed_up > 0) return QColor("orange");
        }
        break;
    default:
        break;
    }

    return QVariant();
}

QVariant PeerModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractListModel::headerData(section, orientation, role);

    switch(section) {
        case PM_IP:        return tr("IP");
        case PM_CLIENT:    return tr("Client");
        case PM_FILE:      return tr("File name");
        case PM_PROGRESS:  return tr("Progress");
        case PM_SPEED:     return tr("Speed");
        case PM_TOTAL:     return tr("Bytes");
        default:
            Q_ASSERT(false);
            break;
    }

    return QVariant();
}

void PeerModel::populate() {
    QLinkedList<QED2KHandle> transfers = Session::instance()->getActiveTransfers();
    QList<PeerInfo> peers;

    foreach(QED2KHandle h, transfers) {
        peers.append(h.get_peer_info());
    }

    qSort(peers.begin(), peers.end());

    QList<PeerInfo>::iterator itr = m_peers.begin();
    while(itr != m_peers.end()) {
        QList<PeerInfo>::iterator fi = qBinaryFind(peers.begin(), peers.end(), *itr);
        int row = itr - m_peers.begin();

        if (fi == peers.end()) {
            beginRemoveRows(QModelIndex(), row, row);
            itr = m_peers.erase(itr);
            endRemoveRows();
        } else {
            *itr = *fi;
            fi->m_hack = true;
            emit dataChanged(index(row, 0), index(row, columnCount()-1));
            ++itr;
        }
    }

    foreach(const PeerInfo& p, peers) {
        if (!p.m_hack) {
            emit beginInsertRows(QModelIndex(), m_peers.size(), m_peers.size());
            m_peers.push_back(p);
            endInsertRows();
        }
    }
}

const PeerInfo& PeerModel::at(const QModelIndex& index) const {
    Q_ASSERT(index.row() < rowCount());
    return m_peers.at(index.row());
}

quint64 PeerModel::speed(const QModelIndex& index) const {
    return (at(index).m_speed_down>0)?at(index).m_speed_down:at(index).m_speed_up;
}

quint64 PeerModel::total(const QModelIndex& index) const {
    return (at(index).m_speed_down>0)?at(index).m_total_down:at(index).m_total_up;
}
