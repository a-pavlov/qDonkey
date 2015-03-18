#include "transfermodel_item.h"
#include <QDebug>

TransferModelItem::TransferModelItem(const QED2KHandle& h) : m_handle(h)
{
    m_hash = h.hash();
    m_size = -1;
    if (m_name.isEmpty()) m_name = h.name();
    //m_addedTime = TorrentPersistentData::getAddedDate(h.hash());
    //m_seedTime = TorrentPersistentData::getSeedDate(h.hash());
    //m_label = TorrentPersistentData::getLabel(h.hash());
}

TransferModelItem::TransferModelItem(const QString& name, qint64 size, const QDateTime& created) {
    m_name = name;
    m_size = size;
    m_addedTime = created;
}

TransferModelItem::State TransferModelItem::state() const
{
    /*
  try {
    // Pause or Queued
    if (m_torrent.is_paused()) {
      m_icon = QIcon(":/Icons/skin/paused.png");
      m_fgColor = QColor("red");
      return m_torrent.is_seed() ? STATE_PAUSED_UP : STATE_PAUSED_DL;
    }
    if (m_torrent.is_queued()) {
      if (m_torrent.state() != qt_queued_for_checking
          && m_torrent.state() != qt_checking_resume_data
          && m_torrent.state() != qt_checking_files) {
        m_icon = QIcon(":/Icons/skin/queued.png");
        m_fgColor = QColor("grey");
        return m_torrent.is_seed() ? STATE_QUEUED_UP : STATE_QUEUED_DL;
      }
    }
    // Other states
    switch(m_torrent.state()) {
    case qt_allocating:
    case qt_downloading_metadata:
    case qt_downloading: {
      if (m_torrent.download_payload_rate() > 0) {
        m_icon = QIcon(":/Icons/skin/downloading.png");
        m_fgColor = QColor("green");
        return STATE_DOWNLOADING;
      } else {
        m_icon = QIcon(":/Icons/skin/stalledDL.png");
        m_fgColor = QColor("grey");
        return STATE_STALLED_DL;
      }
    }
    case qt_finished:
    case qt_seeding:
      if (m_torrent.upload_payload_rate() > 0) {
        m_icon = QIcon(":/Icons/skin/uploading.png");
        m_fgColor = QColor("orange");
        return STATE_SEEDING;
      } else {
        m_icon = QIcon(":/Icons/skin/stalledUP.png");
        m_fgColor = QColor("grey");
        return STATE_STALLED_UP;
      }
    case qt_queued_for_checking:
    case qt_checking_resume_data:
    case qt_checking_files:
      m_icon = QIcon(":/Icons/skin/checking.png");
      m_fgColor = QColor("grey");
      return m_torrent.is_seed() ? STATE_CHECKING_UP : STATE_CHECKING_DL;
    default:
      m_icon = QIcon(":/Icons/skin/error.png");
      m_fgColor = QColor("red");
      return STATE_INVALID;
    }
  } catch(libtorrent::invalid_handle&)
    {
    m_icon = QIcon(":/Icons/skin/error.png");
    m_fgColor = QColor("red");
    return STATE_INVALID;
  }
  catch(libed2k::libed2k_exception&)
    {
        m_icon = QIcon(":/Icons/skin/error.png");
        m_fgColor = QColor("red");
        return STATE_INVALID;
    }
    */

    return STATE_INVALID;
}

bool TransferModelItem::setData(int column, const QVariant &value, int role) {
    qDebug() << Q_FUNC_INFO << column << value;
    if (role != Qt::DisplayRole) return false;

    switch(column) {
        case TM_NAME: m_name = value.toString();
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
    case TM_NAME:
        return m_name;
    case TM_SIZE:
        return -1;
    case TM_PROGRESS:
        return m_handle.progress();
    case TM_STATUS:
        return state();
    case TM_SEEDS: {
        //return (role == Qt::DisplayRole) ? m_handle.num_seeds() : m_handle.num_complete();
        }
    case TM_PEERS: {
        //return (role == Qt::DisplayRole) ? (m_torrent.num_peers()-m_torrent.num_seeds()) : m_torrent.num_incomplete();
        }
    case TM_DLSPEED:
        //return m_torrent.download_payload_rate();
    case TM_UPSPEED:
        //return m_torrent.upload_payload_rate();
    case TM_RATIO:
        //return Session::instance()->getRealRatio(m_torrent.hash());
    case TM_ADD_DATE:
        return m_addedTime;
    case TM_SEED_DATE:
        return m_seedTime;
    case TM_AMOUNT_DOWNLOADED:
        //return static_cast<qlonglong>(m_torrent.total_wanted_done());
    case TM_AMOUNT_LEFT:
        //return static_cast<qlonglong>(m_torrent.total_wanted() - m_torrent.total_wanted_done());
    case TM_TIME_ELAPSED:
        //return (role == Qt::DisplayRole) ? m_torrent.active_time() : m_torrent.seeding_time();
    default:
        return QVariant();
    }
}
