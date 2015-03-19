#include "transfermodel_item.h"
#include "res.h"
#include "qtlibed2k/qed2ksession.h"
#include <QDebug>

TransferModelItem::TransferModelItem(const QED2KHandle& h) : m_handle(h)
{
    m_hash = h.hash();
    m_size = -1;
    if (m_filePath.isEmpty()) m_filePath = h.filepath();
    //m_addedTime = TorrentPersistentData::getAddedDate(h.hash());
    //m_seedTime = TorrentPersistentData::getSeedDate(h.hash());
    //m_label = TorrentPersistentData::getLabel(h.hash());
}

TransferModelItem::TransferModelItem(const QString& filePath, qint64 size, const QDateTime& created) {
    m_filePath = filePath;
    m_size = size;
    m_addedTime = created;
}

TransferModelItem::State TransferModelItem::state() const {

    if (!m_errorMessage.isEmpty()) {
        m_icon = QIcon(res::error());
        m_fgColor = QColor("red");
        return STATE_LOCALFILE;
    }

    // hashing in progress
    if (!m_handle.is_valid()) {
        m_icon = QIcon(res::fileOnDisk());
        m_fgColor = QColor("blue");
        return STATE_LOCALFILE;
    }

    // Pause or Queued
    if (m_handle.is_paused()) {
        m_icon = QIcon(res::paused());
        m_fgColor = QColor("red");
        return m_handle.is_seed() ? STATE_PAUSED_UP : STATE_PAUSED_DL;
    }

    /*if (m_handle.is_queued()) {
        if (m_torrent.state() != qt_queued_for_checking
          && m_torrent.state() != qt_checking_resume_data
          && m_torrent.state() != qt_checking_files) {
        m_icon = QIcon(":/Icons/skin/queued.png");
        m_fgColor = QColor("grey");
        return m_torrent.is_seed() ? STATE_QUEUED_UP : STATE_QUEUED_DL;
      }
    }

    */
    // Other states
    switch(m_handle.state()) {
        case QED2KHandle::downloading: {
            if (m_handle.download_payload_rate() > 0) {
                m_icon = QIcon(res::downloading());
                m_fgColor = QColor("green");
                return STATE_DOWNLOADING;
            } else {
                m_icon = QIcon(res::stalledDL());
                m_fgColor = QColor("grey");
                return STATE_STALLED_DL;
            }
        }
        case QED2KHandle::finished:
        case QED2KHandle::seeding:
            if (m_handle.upload_payload_rate() > 0) {
                m_icon = QIcon(res::uploading());
                m_fgColor = QColor("orange");
                return STATE_SEEDING;
            } else {
                m_icon = QIcon(res::stalledUP());
                m_fgColor = QColor("grey");
                return STATE_STALLED_UP;
            }
        case QED2KHandle::queued_for_checking:
        case QED2KHandle::checking_resume_data:
            m_icon = QIcon(res::checking());
            m_fgColor = QColor("grey");
            return STATE_CHECKING;
        default:
            m_icon = QIcon(res::error());
            m_fgColor = QColor("red");
            return STATE_INVALID;
    }

    return STATE_INVALID;
}

bool TransferModelItem::setData(int column, const QVariant &value, int role) {
    qDebug() << Q_FUNC_INFO << column << value;
    if (role != Qt::DisplayRole) return false;

    switch(column) {
        case TM_NAME: m_filePath = value.toString();
            return true;
        case TM_HASH: m_hash = value.toString();
            return true;
        case TM_ERROR: m_errorMessage = value.toString();
            return true;
        default:
        break;
    }

    return false;
}

QVariant TransferModelItem::data(int column, int role) const
{
    if (role == Qt::DecorationRole && column == TM_NAME) {
        return m_icon;
    }

    if (role == Qt::ForegroundRole) {
        return m_fgColor;
    }

    if (role != Qt::DisplayRole && role != Qt::UserRole) return QVariant();

    //if (!m_torrent.is_valid()) return QVariant();
    switch(column) {
    case TM_NAME:       return misc::fileName(m_filePath);
    case TM_SIZE:       return -1;
    case TM_PROGRESS:   return (!m_handle.is_valid())?1.:m_handle.progress();
    case TM_STATUS:     return state();
    case TM_SEEDS:      return (role == Qt::DisplayRole) ? m_handle.num_seeds() : m_handle.num_complete();
    case TM_PEERS:      return (role == Qt::DisplayRole) ? (m_handle.num_peers() - m_handle.num_seeds()) : m_handle.num_incomplete();
    case TM_DLSPEED:    return m_handle.download_payload_rate();
    case TM_UPSPEED:    return m_handle.upload_payload_rate();
    case TM_HASH:       return m_hash;
    case TM_RATIO:      return Session::instance()->getRealRatio(m_handle.hash());
    case TM_ADD_DATE:   return m_addedTime;
    case TM_SEED_DATE:  return m_seedTime;
    case TM_AMOUNT_DOWNLOADED: return static_cast<qlonglong>(m_handle.total_wanted_done());
    case TM_AMOUNT_LEFT: return static_cast<qlonglong>(m_handle.total_wanted() - m_handle.total_wanted_done());
    case TM_ERROR:      return m_errorMessage;
    case TM_TIME_ELAPSED: //return (role == Qt::DisplayRole) ? m_torrent.active_time() : m_torrent.seeding_time();
    default:
        return QVariant();
    }
}
