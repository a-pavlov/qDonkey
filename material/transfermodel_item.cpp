#include "transfermodel_item.h"
#include "qtlibed2k/qed2ksession.h"
#include <QDebug>
#include <QtQml>

TransferModelItem::TransferModelItem(const QED2KHandle& h, const QString& status) : m_handle(h), m_status(status) {
    Q_ASSERT(h.is_valid());
    m_hash = h.hash();
    m_ft = toFileType(h.filename());
}

TransferModelItemEnum::State TransferModelItem::state() const {
    if (m_handle.is_paused()) return m_handle.is_seed()?TransferModelItemEnum::STATE_PAUSED_UP:TransferModelItemEnum::STATE_PAUSED_DL;

    switch(m_handle.state()) {
        case QED2KHandle::downloading: return (m_handle.download_payload_rate() > 0)?TransferModelItemEnum::STATE_DOWNLOADING:TransferModelItemEnum::STATE_STALLED_DL;
        case QED2KHandle::finished:
        case QED2KHandle::seeding:
            return (m_handle.upload_payload_rate() > 0)?TransferModelItemEnum::STATE_SEEDING:TransferModelItemEnum::STATE_STALLED_UP;
        case QED2KHandle::queued_for_checking:
        case QED2KHandle::checking_resume_data:
            return TransferModelItemEnum::STATE_CHECKING;
    default:
        break;
    }

    return TransferModelItemEnum::STATE_INVALID;
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
    Q_UNUSED(column);
    //if (!m_torrent.is_valid()) return QVariant();
    switch(role) {
        case TM_NAME:       return m_handle.filename();
        case TM_SIZE:       return misc::friendlyUnit(m_handle.filesize(), misc::ST_DEFAULT);
        case TM_TYPE:       return toString(m_ft);
        case TM_PROGRESS:   return (!m_handle.is_valid())?100.:m_handle.progress()*100;
        case TM_STATUS:     return state();
        case TM_SEEDS:      return m_handle.num_seeds();
        case TM_PEERS:      return (m_handle.num_peers() - m_handle.num_seeds());
        case TM_DLSPEED:    return misc::friendlyUnit(m_handle.download_payload_rate()) + tr("/s");
        case TM_UPSPEED:    return misc::friendlyUnit(m_handle.upload_payload_rate()) + tr("/s");
        case TM_HASH:       return m_hash;
        case TM_ETA:        return misc::userFriendlyDuration((m_handle.is_seed() || m_handle.is_paused())?MAX_ETA:Session::instance()->getETA(m_handle.hash()));
        case TM_RATIO:      return Session::instance()->getRealRatio(m_handle.hash());
        case TM_ADD_DATE:   return Session::instance()->hasBeenAdded(m_handle.hash());
        case TM_AMOUNT_DOWNLOADED: return static_cast<qlonglong>(m_handle.total_wanted_done());
        case TM_AMOUNT_LEFT: return static_cast<qlonglong>(m_handle.total_wanted() - m_handle.total_wanted_done());
        case TM_TIME_ELAPSED: return m_handle.active_time(); // : m_handle.seeding_time();
        case TM_AGE:        return m_status;
        default:
            return QVariant();
    }

    return QVariant();
}

//static
void TransferModelItemEnum::qmlRegister() {
    qmlRegisterType<TransferModelItemEnum>("TransferModelItemEnum", 1, 0, "TransferModelItem");
}
