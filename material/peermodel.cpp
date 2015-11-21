#include "peermodel.h"
#include "res.h"
#include <QTimer>

PeerModel::PeerModel(QObject *parent) :
    QAbstractListModel(parent)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshContent()));
}

int PeerModel::rowCount(const QModelIndex&) const {
    return m_peers.size();
}

int PeerModel::columnCount(const QModelIndex&) const {
    return COL_COUNT;
}

QHash<int, QByteArray> PeerModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PM_IP]        = "ip";
    roles[PM_CLIENT]    = "size";
    roles[PM_FILE]      = "file";
    roles[PM_PROGRESS]  = "progress";
    roles[PM_SPEED]     = "speed";
    roles[PM_TOTAL]     = "total";
    return roles;
}

QVariant PeerModel::data(const QModelIndex& index, int role) const {
    QVariant result;

    if (!index.isValid())
        return result;

    switch(role) {
        case PM_IP:        return at(index).m_address;
        case PM_CLIENT:    return at(index).m_client;
        case PM_FILE:      return at(index).m_file;
        case PM_PROGRESS:  return QString::number(at(index).m_progress*100, 'f', 1)+"%";
        case PM_SPEED: return misc::friendlyUnit(speed(index))+tr("/s");
        case PM_TOTAL:     return misc::friendlyUnit(total(index));
        default:
        break;
    }

    return QVariant();
}

QVariant PeerModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal)
        return QAbstractListModel::headerData(section, orientation, role);

    if (role == Qt::TextAlignmentRole) {
        switch(section) {
            case PM_SPEED:
            case PM_TOTAL:
            return Qt::AlignRight;
        default:
            return Qt::AlignLeft;
        }
    }

    if (role == Qt::DisplayRole) {
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
    }

    return QVariant();
}

void PeerModel::populate(const QString& hash) {
    m_hash = hash;
    QED2KHandle handle = Session::instance()->getTransfer(hash);
    QList<PeerInfo> peers;
    if (handle.is_valid()) {
        peers.append(handle.get_peer_info());
        qSort(peers.begin(), peers.end());
    }

    if (!peers.isEmpty()) {
        // merge new list into previos
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
    } else {
        if (!m_peers.isEmpty()) {
            emit beginResetModel();
            m_peers.clear();
            emit endResetModel();
        }
    }

    timer->start(1500);
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

void PeerModel::refreshContent() {
    if (!m_hash.isEmpty()) populate(m_hash);
}
