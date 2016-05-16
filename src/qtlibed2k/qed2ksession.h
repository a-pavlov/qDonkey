#ifndef __QED2KSESSION__
#define __QED2KSESSION__

#ifdef DISABLE_GUI
#include <QCoreApplication>
#else
#include <QApplication>
#include <QPalette>
#endif

#include <set>
#include <QPixmap>
#include <QPointer>
#include <QTimer>
#include <QHash>
#include <QStringList>
#include <QHash>
#include <QSet>
#include <QDateTime>
#include <QSharedPointer>
#include <QLinkedList>
#include <QUrl>

#ifndef Q_MOC_RUN
#include <libed2k/session.hpp>
#include <libed2k/session_status.hpp>
#include <libed2k/server_connection.hpp>
#include <libed2k/kademlia/kad_packet_struct.hpp>
#endif

#include "qed2kserver.h"
#include "qed2khandle.h"
#include "preferences.h"
#include "misc.h"
#include "transferspeedmonitor.h"


typedef libed2k::session_status SessionStatus;

struct ErrorCode {
    typedef boost::system::error_code LibCode;
    typedef std::string UICode;

    ErrorCode& operator=(const LibCode& ec) {
        m_libCode = ec;
        m_uiCode = "";
        return *this;
    }
    ErrorCode& operator=(const UICode& ec) {
        m_uiCode = ec;
        m_libCode = LibCode();
        return *this;
    }

    operator bool() const { return m_libCode || !m_uiCode.empty(); }
    operator LibCode&() { return m_libCode; }

    std::string message() const { return m_libCode ? m_libCode.message() : m_uiCode; }

    LibCode m_libCode;
    std::string m_uiCode;
};

enum FileType {
    FT_ANY              = 0,
    FT_AUDIO            = 1,    // ED2K protocol value (eserver 17.6+)
    FT_VIDEO            = 2,    // ED2K protocol value (eserver 17.6+)
    FT_IMAGE            = 3,    // ED2K protocol value (eserver 17.6+)
    FT_PROGRAM          = 4,    // ED2K protocol value (eserver 17.6+)
    FT_DOCUMENT         = 5,    // ED2K protocol value (eserver 17.6+)
    FT_ARCHIVE          = 6,    // ED2K protocol value (eserver 17.6+)
    FT_CDIMAGE          = 7,    // ED2K protocol value (eserver 17.6+)
    FT_EMULECOLLECTION  = 8,
    FT_UNKNOWN          = 9
};

FileType toFileType(const QString& filename);
QString toString(FileType type);

struct QED2KSearchResultEntry {
    quint64                 m_nFilesize;
    quint64                 m_nSources;
    quint64                 m_nCompleteSources;
    quint64                 m_nMediaBitrate;
    quint64                 m_nMediaLength;
	QString	                m_hFile;
	QString                 m_strFilename;
	QString	                m_strMediaCodec;
    FileType                m_type;
    libed2k::net_identifier m_network_point; 
	bool isCorrect() const;
	QED2KSearchResultEntry();
    static QED2KSearchResultEntry load(const Preferences& pref);
	static QED2KSearchResultEntry fromSharedFileEntry(const libed2k::shared_file_entry& sf);
    void save(Preferences& pref) const;
    FileType getType();
};

struct QED2KPeerOptions {
    quint8  m_nAICHVersion;
    bool    m_bUnicodeSupport;
    quint8  m_nUDPVer;
    quint8  m_nDataCompVer;
    quint8  m_nSupportSecIdent;
    quint8  m_nSourceExchange1Ver;
    quint8  m_nExtendedRequestsVer;
    quint8  m_nAcceptCommentVer;
    bool    m_bNoViewSharedFiles;
    bool    m_bMultiPacket;
    bool    m_bSupportsPreview;

    bool    m_bSupportCaptcha;
    bool    m_bSourceExt2;
    bool    m_bExtMultipacket;
    bool    m_bLargeFiles;
    QED2KPeerOptions(const libed2k::misc_options& mo, const libed2k::misc_options2& mo2);
};

/**
 * @brief The PersistentDataItem struct handles libed2k related transfer params
 * and additional birthday parameter for transfer
 */
struct PersistentDataItem {
    QSharedPointer<libed2k::add_transfer_params> m_params;
    QDateTime m_birthday;
    bool has_metadata() const { return !m_params.isNull(); }
};

typedef QHash<QString, PersistentDataItem> PersistentData;

QString fromHash(const libed2k::md4_hash&);

struct KadNode {
    QString host;
    quint16 port;
    QString kid;
    int distance;
    bool operator<(const KadNode& kn) const {
        return distance < kn.distance;
    }

    KadNode(const libed2k::kad_id& own, const libed2k::kad_state_entry& ke);
};

class FileDownloader;

class QED2KSession : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(QED2KSession)

private:
    QED2KSession();

    static QED2KSession* m_instance;
public:
    static const qreal MAX_RATIO;
    static QED2KSession* instance();
    static void drop();
    void start();
    virtual void stop();
    bool started() const;
    virtual ~QED2KSession();

    QED2KHandle getTransfer(const QString& hash) const;
    QLinkedList<QED2KHandle> getTransfers() const;
    QLinkedList<QED2KHandle> getActiveTransfers() const;
    virtual bool hasActiveTransfers() const; // override default behaviour by active transfers usage
    SessionStatus getSessionStatus() const;

    Q_INVOKABLE void deleteTransfer(const QString& hash, bool delete_files);

    void setDownloadRateLimit(long rate);
    void setUploadRateLimit(long rate);
    virtual void saveTempFastResumeData();
    virtual void saveFastResumeData();
    void startServerConnection(const QED2KServer& s);
    void stopServerConnection();
    Q_INVOKABLE bool isServerConnected() const;
    void makeTransferParametersAsync(const QString& filepath);
    void cancelTransferParameters(const QString& filepath);
    std::pair<libed2k::add_transfer_params, libed2k::error_code> makeTransferParameters(const QString& filepath) const;

    /** scan ed2k backup directory and load all files were matched name filter */
    void loadFastResumeData(const QString&);
    void enableUPnP(bool b);

    void deferPlayMedia(QED2KHandle h);
    Q_INVOKABLE void deferPlayMediaFile(const QString& hash);
    bool playMedia(QED2KHandle h);
    void playLink(const QString& strLink);
    Q_INVOKABLE bool loadDirectory(const QString& path);
    QDateTime hasBeenAdded(const QString& hash) const;
    qlonglong getETA(const QString& hash) const;
    QString status(const QString& hash) const;
    void playPendingMedia();
    libed2k::session* delegate() const;

    const libed2k::ip_filter& session_filter() const;

    Q_INVOKABLE void pauseTransfer(const QString& hash);
    Q_INVOKABLE void resumeTransfer(const QString& hash);
    Q_PROPERTY(QString currentMediaFile READ currentMediaFile WRITE setCurrentMediaFile NOTIFY currentMediaFileChanged)

    QString currentMediaFile() const;
    void setCurrentMediaFile(const QString&);

    libed2k::kad_state getKademliaState() const;
    Q_INVOKABLE void startKad();
    Q_INVOKABLE void stopKad();
    bool isKadStarted() const;
    void addNodesToKad(const QStringList&);
    void bootstrapKad(const QString& host, int port);
    Q_INVOKABLE QList<KadNode> kadState();
    Q_INVOKABLE bool hasInitialNodesFile();

    /**
     * @brief saveKadState
     * @return true if dht state contains some nodes
     */
    bool saveKadState();
    libed2k::entry loadKadState();
    Q_INVOKABLE bool hasPrevKadState() const;
    Q_INVOKABLE bool downloadEmuleKad();

    Q_INVOKABLE void toPP() { m_PropPending = true; }
    bool isPropPending() const { return m_PropPending; }
    Q_INVOKABLE void syncProperties();
private:
    QScopedPointer<libed2k::session> m_session;
    QHash<QString, QED2KHandle> m_fast_resume_transfers;   // contains fast resume data were loading
    QTimer alertsTimer;
    QTimer frdTimer;
    QSet<QED2KHandle> m_pending_medias;

    QStringList m_pendingFiles; // new files awaiting to calculate parameters and share
    QSet<QString> m_sharedFiles;    // earlier shared files(from metadata dir) and new shared files from inout directory
    QHash<QString, QDateTime> m_addTimes;   // creation dates for transfers
    QHash<QString, libed2k::transfer_resume_data> m_fastTransfers;  // transfers information from metadata directory
    QSet<QString>   m_currentSessionTransfers;  // transfers were added to session not as seed
    libed2k::server_connection_parameters m_sp;
    QString m_currentPath;
    QScopedPointer<TransferSpeedMonitor>    m_speedMon;
    QDateTime               last_error_dt;
    QUrl                    currentMF;
    FileDownloader*         m_fd;
    bool                    m_PropPending;
private slots:
    void readAlerts();
    void saveResume();
    void downloadEMuleKadCompleted(int rc, int system);
public slots:
	void configureSession();
    virtual QPair<QED2KHandle, ErrorCode> addLink(QString strLink, bool resumed = false);
    Q_INVOKABLE bool loadLink(const QString& link, bool resumed);
    virtual void addTransferFromFile(const QString& filename, bool resumed = false);
    virtual QED2KHandle addTransfer(const libed2k::add_transfer_params&);
    Q_INVOKABLE bool addTransfer(const QString& hash, const QString& filename, qlonglong size, int sources, bool preview);
    QString postTransfer(const libed2k::add_transfer_params&);  // async add transfer and return hash from atp

	/**
	  * number parameters were ignored on zero value
	 */
    Q_INVOKABLE void searchFiles(const QString& strQuery,
	                quint64 nMinSize,
	                quint64 nMaxSize,
	                unsigned int nSources,
                    unsigned int nCompleteSources,
	                QString strFileType,
	                QString strFileExt,
	                QString strMediaCodec,
	                quint32 nMediaLength,
	                quint32 nMediaBitrate);

	/**
	  * set found file hash here
	 */
    Q_INVOKABLE void searchRelatedFiles(QString strHash);
	/**
	  * you can search more results only when previous call return succuss
	 */
    Q_INVOKABLE void searchMoreResults();
	/**
	  * stops current search
	 */
    Q_INVOKABLE void cancelSearch();

    /**
      * open on completed transfer`
    */
    Q_INVOKABLE bool openTransfer(QString hash);

    libed2k::peer_connection_handle getPeer(const libed2k::net_identifier& np);
    libed2k::peer_connection_handle findPeer(const libed2k::net_identifier& np);
    QString makeLink(const QString& filename, long filesize, const QString& filehash);
    qreal getRealRatio(const QString &hash) const;
signals:
    /**
     * servers related signals
     */
    void serverNameResolved(QString alias, QString strName);
    void serverConnectionInitialized(QString alias, QString host, int port, quint32 client_id, quint32 tcp_flags, quint32 aux_port);
    void serverConnectionClosed(QString alias, QString host, int port, QString strError);
    void serverStatus(QString alias, int nFiles, int nUsers);
    void serverMessage(QString alias, QString strMessage);
    void serverIdentity(QString alias, QString strName, QString strDescription);

    // TODO - add server identifier
    void searchResult(const libed2k::net_identifier& np, const QString& hash, 
                      const QList<QED2KSearchResultEntry>& vRes, bool bMoreResult);

    /**
     * @brief searchFinished - simplified signal emitted after search completed
     */
    void searchFinished(int resultsCount, bool bMoreResult);


    /**
     * peers related signals
     */
    void peerConnected(const libed2k::net_identifier& np,
                       const QString& hash,
                       bool bActive);

    void peerDisconnected(const libed2k::net_identifier& np, const QString& hash, const libed2k::error_code ec);
    void peerSharedFiles(const libed2k::net_identifier& np, const QString& hash,
                         const std::vector<QED2KSearchResultEntry>& vRes);

    void peerIsModSharedFiles(const libed2k::net_identifier& np, const QString& hash, const QString& dir_hash,
                              const QList<QED2KSearchResultEntry>& vRes);
    /**
      * requested peers shared directories
     */
    void peerSharedDirectories(const libed2k::net_identifier& np, const QString& hash, const QStringList& strList);

    /**
      * requested peers files from sprecified directory
     */
    void peerSharedDirectoryFiles(const libed2k::net_identifier& np, const QString& hash,
                                  const QString& strDirectory, const QList<QED2KSearchResultEntry>& vRes);

    /**
      * incoming message signals
     */
    void peerMessage(const libed2k::net_identifier& np, const QString& hash, const QString& strMessage);
    void peerCaptchaRequest(const libed2k::net_identifier& np, const QString& hash, const QPixmap& pm);
    void peerCaptchaResult(const libed2k::net_identifier& np, const QString& hash, quint8 nResult);

    /**
      * peer rejected our query for files and possibly directories
     */
    void peerSharedFilesAccessDenied(const libed2k::net_identifier& np, const QString& hash);


    void transferParametersReady(const libed2k::add_transfer_params&, const libed2k::error_code&);

    /**
     * @brief transferAdded - completely new transfer was added to session
     */
    void transferAdded(QED2KHandle);
    void transferPaused(QED2KHandle);
    void transferResumed(QED2KHandle);
    void transferDeleted(QString);
    /**
     * @brief transferFinished - new transfer finished download
     */
    void transferFinished(QED2KHandle);
    /**
     * @brief transferShared - new transfer created from local file
     */
    void transferShared(QED2KHandle);
    void transferSavePathChanged(QED2KHandle);
    void transferStorageMoved(QED2KHandle);

    /**
     * @brief transferRestored - transfer was restored from fast resume data
     */
    void transferRestored(QED2KHandle);

    void fileError(QED2KHandle, QString);
    void fileIOError(QString filename, QString message);

    void resetInputDirectory(const QString& path);

    void currentMediaFileChanged(QString);
    void downloadKadCompleted(int rc, int system);
};

typedef QED2KSession Session;

#endif //__QED2KSESSION__
