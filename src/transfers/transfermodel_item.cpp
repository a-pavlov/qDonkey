#include "transfermodel_item.h"
#include "res.h"
#include "qtlibed2k/qed2ksession.h"
#include <QDebug>

TransferModelItem::TransferModelItem(const QED2KHandle& h) : m_handle(h) {
    Q_ASSERT(h.is_valid());
    m_hash = h.hash();
    m_ft = toFileType(h.filename());
}

TransferModelItem::State TransferModelItem::state() const {
    if (m_handle.is_paused()) return m_handle.is_seed()?STATE_PAUSED_UP:STATE_PAUSED_DL;

    switch(m_handle.state()) {
        case QED2KHandle::downloading: return (m_handle.download_payload_rate() > 0)?STATE_DOWNLOADING:STATE_STALLED_DL;
        case QED2KHandle::finished:
        case QED2KHandle::seeding:
            return (m_handle.upload_payload_rate() > 0)?STATE_SEEDING:STATE_STALLED_UP;
        case QED2KHandle::queued_for_checking:
        case QED2KHandle::checking_resume_data:
            return STATE_CHECKING;
    default:
        break;
    }

    return STATE_INVALID;
}

bool TransferModelItem::setData(int column, const QVariant &value, int role) {
    qDebug() << Q_FUNC_INFO << column << value;
    if (role != Qt::DisplayRole) return false;

    switch(column) {
        case TM_HASH: m_hash = value.toString();
            return true;
        default:
        break;
    }

    return false;
}

QVariant TransferModelItem::data(int column, int role) const {
    if (role == Qt::DecorationRole && column == TM_NAME) {
        switch(state()) {
            case STATE_DOWNLOADING: return QIcon(res::downloading());
            case STATE_STALLED_DL:  return QIcon(res::stalledDL());
            case STATE_STALLED_UP:  return QIcon(res::stalledUP());
            case STATE_SEEDING:     return QIcon(res::uploading());
            case STATE_PAUSED_UP:
            case STATE_PAUSED_DL:   return QIcon(res::paused());
            case STATE_CHECKING:
            case STATE_QUEUED:      return QIcon(res::checking());
            case STATE_INVALID:     return QIcon(res::error());
            default:
                return QVariant();
        }
    }

    if (role == Qt::ForegroundRole) {
        switch(state()) {
        case STATE_DOWNLOADING: return QColor("green");
        case STATE_STALLED_DL:
        case STATE_STALLED_UP:  return QColor("grey");
        case STATE_SEEDING:     return QColor("orange");
        case STATE_PAUSED_UP:
        case STATE_PAUSED_DL:   return QColor("red");
        case STATE_CHECKING:
        case STATE_QUEUED:      return QColor("grey");
        case STATE_INVALID:     return QColor("red");
        default:
            return QVariant();
        }
    }

    if (role != Qt::DisplayRole && role != Qt::UserRole) return QVariant();

    //if (!m_torrent.is_valid()) return QVariant();
    switch(column) {
        case TM_NAME:       return m_handle.filename();
        case TM_SIZE:       return m_handle.filesize();
        case TM_TYPE:       return toString(m_ft);
        case TM_PROGRESS:   return (!m_handle.is_valid())?1.:m_handle.progress();
        case TM_STATUS:     return state();
        case TM_SEEDS:      return (role == Qt::DisplayRole) ? m_handle.num_seeds() : m_handle.num_complete();
        case TM_PEERS:      return (role == Qt::DisplayRole) ? (m_handle.num_peers() - m_handle.num_seeds()) : m_handle.num_incomplete();
        case TM_DLSPEED:    return m_handle.download_payload_rate();
        case TM_UPSPEED:    return m_handle.upload_payload_rate();
        case TM_HASH:       return m_hash;
        case TM_ETA:        return (m_handle.is_seed() || m_handle.is_paused())?MAX_ETA:Session::instance()->getETA(m_handle.hash());
        case TM_RATIO:      return Session::instance()->getRealRatio(m_handle.hash());
        case TM_ADD_DATE:   return Session::instance()->hasBeenAdded(m_handle.hash());
        case TM_AMOUNT_DOWNLOADED: return static_cast<qlonglong>(m_handle.total_wanted_done());
        case TM_AMOUNT_LEFT: return static_cast<qlonglong>(m_handle.total_wanted() - m_handle.total_wanted_done());
        case TM_TIME_ELAPSED: return (role == Qt::DisplayRole) ? m_handle.active_time() : m_handle.seeding_time();
        default:
            return QVariant();
    }
}
