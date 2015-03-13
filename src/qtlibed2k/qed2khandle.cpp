#include <libed2k/constants.hpp>

#include "qed2khandle.h"
#include "misc.h"


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
TransferState QED2KHandle::state() const
{
    return m_delegate.state();
    /*
    TransferState ts;

    switch (m_delegate.state())
    {
        case libed2k::transfer_status::queued_for_checking:
            ts = qt_queued_for_checking;
            break;
        case libed2k::transfer_status::checking_files:
            ts = qt_checking_files;
            break;
        case libed2k::transfer_status::downloading_metadata:
            ts = qt_downloading_metadata;
            break;
        case libed2k::transfer_status::downloading:
            ts = qt_downloading;
            break;
        case libed2k::transfer_status::finished:
            ts = qt_seeding;
            break;
        case libed2k::transfer_status::seeding:
            ts = qt_seeding;
            break;
        case libed2k::transfer_status::allocating:
            ts = qt_allocating;
            break;
        case libed2k::transfer_status::checking_resume_data:
            ts = qt_checking_resume_data;
            break;
        default:
            ts = qt_unhandled_state;
    }

    return (ts);
    */
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

    const std::vector<int> extremities = file_extremity_pieces_at(0);
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

std::vector<int> QED2KHandle::file_extremity_pieces_at(unsigned int index) const {
    Q_ASSERT(index == 0);
    int last_piece = m_delegate.num_pieces() - 1;
    int penult_piece = std::max(last_piece - 1, 0);

    std::vector<int> res;
    res.push_back(0);
    res.push_back(penult_piece);
    res.push_back(last_piece);
    return res;
}

QStringList QED2KHandle::absolute_files_path() const {
    QStringList res;
    res << filepath();
    return res;
}
void QED2KHandle::get_peer_info(std::vector<PeerInfo>& infos) const 
{
    std::vector<libed2k::peer_info> ed_infos;
    m_delegate.get_peer_info(ed_infos);
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
    prioritize_extremity_pieces(p, 0);
}
void QED2KHandle::prioritize_extremity_pieces(bool p, unsigned int index) const {
    Q_ASSERT(index == 0);

    int prio = p ? 7 : 1;
    const std::vector<int> extremities = file_extremity_pieces_at(index);
    foreach (int e, extremities)
        m_delegate.set_piece_priority(e, prio);
}

void QED2KHandle::set_sequential_download(bool sd) const { m_delegate.set_sequential_download(sd); }
void QED2KHandle::save_resume_data() const { m_delegate.save_resume_data(); }
bool QED2KHandle::need_save_resume_data() const { return m_delegate.need_save_resume_data(); }
void QED2KHandle::set_upload_mode(bool b) const { m_delegate.set_upload_mode(b); }
void QED2KHandle::set_eager_mode(bool b) const { m_delegate.set_eager_mode(b); }
