#ifndef Q_MOC_RUN
#include <libed2k/constants.hpp>
#endif

#include "qed2khandle.h"
#include "misc.h"


PeerInfo::PeerInfo() : m_speed_down(0), m_total_down(0),
    m_speed_up(0), m_total_up(0),
    m_progress(0.f), m_hack(false) {}

QED2KHandle::QED2KHandle() {}

QED2KHandle::QED2KHandle(const libed2k::transfer_handle& h): m_delegate(h) {
}

bool QED2KHandle::operator==(const QED2KHandle& t) const
{
    const QED2KHandle* pt = dynamic_cast<const QED2KHandle*>(&t);
    return (pt && pt->m_delegate == m_delegate);
}

bool QED2KHandle::operator<(const QED2KHandle& t) const
{
    const QED2KHandle* pt = dynamic_cast<const QED2KHandle*>(&t);
    return (pt && pt->m_delegate < m_delegate);
}

QString QED2KHandle::hash() const { return misc::toQString(m_delegate.hash()); }
QString QED2KHandle::name() const { return misc::toQStringU(m_delegate.name()); }

QString QED2KHandle::save_path() const {
    return misc::toQStringU(m_delegate.save_path()).replace("\\", "/"); // why replace ?
}

QString QED2KHandle::firstFileSavePath() const { return save_path(); }
QString QED2KHandle::creation_date() const { return QString(); }
QString QED2KHandle::comment() const { return QString(); }
QString QED2KHandle::next_announce() const { return QString(); }
TransferStatus QED2KHandle::status() const { return m_delegate.status(); }
QED2KHandle::TransferState QED2KHandle::state() const {
    switch (m_delegate.state()) {
        case libed2k::transfer_status::queued_for_checking: return queued_for_checking;
        case libed2k::transfer_status::checking_files: return checking_files;
        case libed2k::transfer_status::downloading_metadata: return downloading_metadata;
        case libed2k::transfer_status::downloading: return downloading;
        case libed2k::transfer_status::finished: return seeding;
        case libed2k::transfer_status::seeding: return seeding;
        case libed2k::transfer_status::allocating: return allocating;
        case libed2k::transfer_status::checking_resume_data: return checking_resume_data;
        default:
            Q_ASSERT(false);
    }

    return end;
}

int QED2KHandle::upload_limit() const { return m_delegate.upload_limit(); }
int QED2KHandle::download_limit() const { return m_delegate.download_limit(); }
bool QED2KHandle::is_valid() const { return m_delegate.is_valid(); }
bool QED2KHandle::is_seed() const { return m_delegate.is_seed(); }
bool QED2KHandle::is_paused() const { return m_delegate.is_paused(); }
bool QED2KHandle::is_sequential_download() const { return m_delegate.is_sequential_download(); }
void QED2KHandle::piece_availability(std::vector<int>& avail) const { m_delegate.piece_availability(avail); }
std::vector<int> QED2KHandle::piece_priorities() const { return m_delegate.piece_priorities(); }
TransferSize QED2KHandle::total_size() const { return m_delegate.size(); }
TransferSize QED2KHandle::piece_length() const { return libed2k::PIECE_SIZE; }
bool QED2KHandle::extremity_pieces_first() const {
    const QString ext = misc::file_extension(filename());

    if (!misc::isPreviewable(ext)) return false; // No media file

    const QList<int> extremities = file_extremity_pieces_at();
    const std::vector<int> piece_priorities = m_delegate.piece_priorities();
    foreach (int e, extremities) if (piece_priorities[e] != 7) return false;
    return true;
}

float QED2KHandle::progress() const {
    TransferStatus st = status();
    if (!st.total_wanted) return 0.;
    if (st.total_wanted_done == st.total_wanted)  return 1.;
    float progress = (float) st.total_wanted_done / (float) st.total_wanted;
    return progress;
}

QString QED2KHandle::filepath() const {  return misc::toQStringU(libed2k::combine_path(m_delegate.save_path(), m_delegate.name()));}
QString QED2KHandle::filename() const { return misc::toQStringU(m_delegate.name()); }
TransferSize QED2KHandle::filesize() const { return m_delegate.size(); }

QList<int> QED2KHandle::file_extremity_pieces_at() const {
    int last_piece = m_delegate.num_pieces() - 1;
    int penult_piece = std::max(last_piece - 1, 0);

    QList<int> res;
    return (res << 0 << penult_piece << last_piece);
}

QList<PeerInfo> QED2KHandle::get_peer_info() const {
    std::vector<libed2k::peer_info> inf;
    m_delegate.get_peer_info(inf);
    QList<PeerInfo> res;
    for(std::vector<libed2k::peer_info>::const_iterator peer = inf.begin(); peer != inf.end(); ++peer) {
        PeerInfo info;
        libed2k::error_code ec;
        QString peer_ip = misc::toQString(peer->ip.address().to_string(ec));
        info.m_address = peer_ip + ":" + QString::number(peer->ip.port());
        if(ec) continue;

        if (peer->payload_down_speed == 0 && peer->payload_up_speed == 0) continue;

        info.m_hash         = hash();
        info.m_client       = misc::toQStringU(peer->client);
        info.m_file         = filename();
        info.m_total_down   = peer->total_download;
        info.m_speed_down   = peer->payload_down_speed;
        info.m_total_up     = peer->total_upload;
        info.m_speed_up     = peer->payload_up_speed;
        info.m_progress     = peer->progress;
        switch(peer->source)
        {
            case libed2k::peer_info::tracker:
                info.m_source = PeerInfo::PST_SERVER;
                break;
            case libed2k::peer_info::dht:
                info.m_source = PeerInfo::PST_KAD;
                break;
            case libed2k::peer_info::incoming:
                info.m_source = PeerInfo::PST_INCOMING;
                break;
            default:
                info.m_source = PeerInfo::PST_UNKNOWN;
                break;
        }

        res.push_back(info);
    }

    return res;
}

void QED2KHandle::pause() const { m_delegate.pause(); }
void QED2KHandle::resume() const { m_delegate.resume(); }
void QED2KHandle::move_storage(const QString& new_path) const {
    if (QDir(save_path()) == QDir(new_path))
        return;
    // Actually move the storage
    m_delegate.move_storage(new_path.toUtf8().constData());
}
void QED2KHandle::rename_file(int index, const QString& new_name) const {
    m_delegate.rename_file(new_name.toUtf8().constData());
}

void QED2KHandle::prioritize_extremity_pieces(bool p) const {
    int prio = p ? 7 : 1;
    const QList<int> extremities = file_extremity_pieces_at();
    foreach (int e, extremities)
        m_delegate.set_piece_priority(e, prio);
}

void QED2KHandle::set_sequential_download(bool sd) const { m_delegate.set_sequential_download(sd); }
void QED2KHandle::save_resume_data() const { m_delegate.save_resume_data(); }
bool QED2KHandle::need_save_resume_data() const { return m_delegate.need_save_resume_data(); }
void QED2KHandle::set_upload_mode(bool b) const { m_delegate.set_upload_mode(b); }
void QED2KHandle::set_eager_mode(bool b) const { m_delegate.set_eager_mode(b); }
libed2k::size_type QED2KHandle::all_time_upload() const { return status().all_time_upload; }
libed2k::size_type QED2KHandle::all_time_download() const { return status().all_time_download; }
libed2k::size_type QED2KHandle::total_done() const { return status().total_done; }
libed2k::size_type QED2KHandle::total_wanted_done() const { return status().total_wanted_done; }
libed2k::size_type QED2KHandle::total_wanted() const { return status().total_wanted; }
int QED2KHandle::num_seeds() const { return status().num_seeds; }
int QED2KHandle::num_peers() const { return status().num_peers; }
int QED2KHandle::num_complete() const { return status().num_complete; }
int QED2KHandle::num_incomplete() const { return status().num_incomplete; }
int QED2KHandle::num_connections() const { return status().num_connections; }
int QED2KHandle::connections_limit() const { return status().connections_limit; }
qlonglong QED2KHandle::active_time() const { return status().active_time; }
qlonglong QED2KHandle::seeding_time() const { return status().seeding_time; }
