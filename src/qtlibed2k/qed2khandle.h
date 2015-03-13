#ifndef __QED2kHANDLE__
#define __QED2kHANDLE__

#include <libed2k/transfer_handle.hpp>
#include <QString>

typedef libed2k::size_type TransferSize;
typedef libed2k::transfer_status TransferStatus;
typedef libed2k::transfer_status::state_t TransferState;
typedef libed2k::peer_info PeerInfo;

class QED2KHandle {
public:
    QED2KHandle();
    explicit QED2KHandle(const libed2k::transfer_handle& h);
    const libed2k::transfer_handle& delegate() const { return m_delegate; }

    bool operator==(const QED2KHandle& t) const;
    bool operator<(const QED2KHandle& t) const;

    QString hash() const;
    QString name() const;

    QString save_path() const;
    QString firstFileSavePath() const;
    QString creation_date() const;
    QString comment() const;
    QString next_announce() const;
    TransferStatus status() const;
    TransferState state() const;
    int upload_limit() const;
    int download_limit() const;
    bool is_valid() const;
    bool is_seed() const;
    bool is_paused() const;
    bool is_sequential_download() const;

    bool is_downloading() const {
        return state() == libed2k::transfer_status::downloading;
    }

    bool is_queued() const {
        return state() == libed2k::transfer_status::queued_for_checking;
    }

    bool is_checking_files() const {
        return state() == libed2k::transfer_status::checking_files;
    }


    void piece_availability(std::vector<int>& avail) const;
    std::vector<int> piece_priorities() const;
    TransferSize total_size() const;
    TransferSize piece_length() const;
    bool extremity_pieces_first() const;
    float progress() const;
    QString filepath() const;
    QString filename() const;
    TransferSize filesize() const;
    std::vector<int> file_extremity_pieces_at(unsigned int index) const;
    QStringList absolute_files_path() const;
    void get_peer_info(std::vector<PeerInfo>& peers) const;

    void pause() const;
    void resume() const;
    void move_storage(const QString& path) const;
    void rename_file(int index, const QString& new_name) const;
    void prioritize_extremity_pieces(bool p) const;
    void prioritize_extremity_pieces(bool p, unsigned int index) const;

    void set_sequential_download(bool sd) const;
    void save_resume_data() const;
    bool need_save_resume_data() const;
    void set_upload_mode(bool b) const;
    void set_eager_mode(bool b) const;

private:
    libed2k::transfer_handle m_delegate;
};


#endif //__QED2kHANDLE__
