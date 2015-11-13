#include <fstream>
#include <iostream>

#ifndef Q_MOC_RUN
#include <libed2k/bencode.hpp>
#include <libed2k/file.hpp>
#include <libed2k/md4_hash.hpp>
#include <libed2k/search.hpp>
#include <libed2k/error_code.hpp>
#include <libed2k/transfer_handle.hpp>
#include <libed2k/alert_types.hpp>
#include <libed2k/ip_filter.hpp>
#include <libed2k/util.hpp>
#include <libed2k/upnp.hpp>
#include <libed2k/natpmp.hpp>
#include <libed2k/file.hpp>
#endif

#include <QMessageBox>
#include <QDir>
#include <QDirIterator>
#include <QDesktopServices>
#include <QUrl>

#include "qed2ksession.h"
#include "misc.h"

FileType toFileType(const QString& filename) {
    return static_cast<FileType>(libed2k::GetED2KFileTypeID(filename.toUtf8().constData()));
}

QString toString(FileType type) {
    switch(type) {
        case FT_ANY:                return QObject::tr("Any");
        case FT_AUDIO:              return QObject::tr("Audio");
        case FT_VIDEO:              return QObject::tr("Video");
        case FT_IMAGE:              return QObject::tr("Picture");
        case FT_PROGRAM:            return QObject::tr("Program");
        case FT_DOCUMENT:           return QObject::tr("Document");
        case FT_ARCHIVE:            return QObject::tr("Archive");
        case FT_CDIMAGE:            return QObject::tr("CD image");
        case FT_EMULECOLLECTION:    return QObject::tr("Emule collection");
        default:                            return "";
    }
}

QED2KSession* QED2KSession::m_instance = NULL;
const qreal QED2KSession::MAX_RATIO = 9999.0;

QED2KSession* QED2KSession::instance() {
    if (!m_instance) m_instance = new QED2KSession;
    return m_instance;
}

void QED2KSession::drop() {
    delete m_instance;
}


/* Converts a QString hash into a  libed2k md4_hash */
static libed2k::md4_hash QStringToMD4(const QString& s)
{
    Q_ASSERT(s.length() == libed2k::md4_hash::hash_size*2);
    return libed2k::md4_hash::fromString(s.toStdString());
}

QString md4toQString(const libed2k::md4_hash& hash)
{
    return QString::fromStdString(hash.toString());
}

QED2KSearchResultEntry::QED2KSearchResultEntry() :
        m_nFilesize(0),
        m_nSources(0),
        m_nCompleteSources(0),
        m_nMediaBitrate(0),
        m_nMediaLength(0),
        m_type(FT_UNKNOWN)
{
}

//static
QED2KSearchResultEntry QED2KSearchResultEntry::load(const Preferences& pref) {
    QED2KSearchResultEntry res;
#ifdef Q_OS_MAC
    res.m_nFilesize = pref.value("Filesize", 0).toString().toULongLong();
#else
    res.m_nFilesize = pref.value("Filesize", 0).toULongLong();
#endif
    res.m_nSources  = pref.value("Sources", 0).toULongLong();
    res.m_nCompleteSources  = pref.value("CompleteSources", 0).toULongLong();
    res.m_nMediaBitrate     = pref.value("MediaBitrate", 0).toULongLong();
    res.m_nMediaLength      = pref.value("MediaLength", 0).toULongLong();
    res.m_hFile             = pref.value("File", QString()).toString();
    res.m_strFilename       = pref.value("Filename", QString()).toString();
    res.m_strMediaCodec     = pref.value("MediaCodec", QString()).toString();
    res.m_network_point.m_nIP = pref.value("IP", 0).toUInt();
    res.m_network_point.m_nPort = pref.value("Port", 0).toUInt();
    return res;
}

void QED2KSearchResultEntry::save(Preferences& pref) const {
#ifdef Q_OS_MAC
    pref.setValue("Filesize",       QString::number(m_nFilesize));
#else
    pref.setValue("Filesize",       m_nFilesize);
#endif
    pref.setValue("Sources",        m_nSources);
    pref.setValue("CompleteSources",m_nCompleteSources);
    pref.setValue("MediaBitrate",   m_nMediaBitrate);
    pref.setValue("MediaLength",    m_nMediaLength);
    pref.setValue("File",           m_hFile);
    pref.setValue("Filename",       m_strFilename);
    pref.setValue("MediaCodec",     m_strMediaCodec);
    pref.setValue("IP",             m_network_point.m_nIP);
    pref.setValue("Port",           m_network_point.m_nPort);
}

FileType QED2KSearchResultEntry::getType() {
    if (m_type == FT_UNKNOWN) {
        // EED2KFileType fileType
        m_type = toFileType(m_strFilename);
    }

    return m_type;
}

// static
QED2KSearchResultEntry QED2KSearchResultEntry::fromSharedFileEntry(const libed2k::shared_file_entry& sf)
{
    QED2KSearchResultEntry sre;

    sre.m_hFile = md4toQString(sf.m_hFile);
    sre.m_network_point = sf.m_network_point;

    try
    {
        for (size_t n = 0; n < sf.m_list.count(); n++)
        {
            boost::shared_ptr<libed2k::base_tag> ptag = sf.m_list[n];

            switch(ptag->getNameId())
            {

            case libed2k::FT_FILENAME:
                sre.m_strFilename = QString::fromUtf8(ptag->asString().c_str(), ptag->asString().size());
                break;
            case libed2k::FT_FILESIZE:
                sre.m_nFilesize += ptag->asInt();
                break;
            case libed2k::FT_FILESIZE_HI:
            	sre.m_nFilesize += (ptag->asInt() << 32);
            	break;
            case libed2k::FT_SOURCES:
                sre.m_nSources = ptag->asInt();
                break;
            case libed2k::FT_COMPLETE_SOURCES:
                sre.m_nCompleteSources = ptag->asInt();
                break;
            case libed2k::FT_MEDIA_BITRATE:
                sre.m_nMediaBitrate = ptag->asInt();
                break;
            case libed2k::FT_MEDIA_CODEC:
                sre.m_strMediaCodec = QString::fromUtf8(ptag->asString().c_str(), ptag->asString().size());
                break;
            case libed2k::FT_MEDIA_LENGTH:
                sre.m_nMediaLength = ptag->asInt();
                break;
            default:
                break;
            }
        }

        if (sre.m_nMediaLength == 0)
        {
            if (boost::shared_ptr<libed2k::base_tag> p = sf.m_list.getTagByName(libed2k::FT_ED2K_MEDIA_LENGTH))
            {
                if (p->getType() == libed2k::TAGTYPE_STRING)
                {
                    // TODO - possible need store string length
                }
                else
                {
                    sre.m_nMediaLength = p->asInt();
                }
            }
        }

        if (sre.m_nMediaBitrate == 0)
        {
            if (boost::shared_ptr<libed2k::base_tag> p = sf.m_list.getTagByName(libed2k::FT_ED2K_MEDIA_BITRATE))
            {
                sre.m_nMediaLength = p->asInt();
            }
        }

        sre.getType();

        // for users
        // m_nMediaLength  - low part of real size
        // m_nMediaBitrate - high part of real size
    }
    catch(libed2k::libed2k_exception& e)
    {
        qDebug("%s", e.what());
    }

    return (sre);
}

bool QED2KSearchResultEntry::isCorrect() const
{
    return (m_hFile.size() == libed2k::MD4_HASH_SIZE*2 && !m_strFilename.isEmpty());
}

QED2KPeerOptions::QED2KPeerOptions(const libed2k::misc_options& mo, const libed2k::misc_options2& mo2)
{
    m_nAICHVersion      = static_cast<quint8>(mo.m_nAICHVersion);
    m_bUnicodeSupport   = mo.m_nUnicodeSupport;
    m_nUDPVer           = static_cast<quint8>(mo.m_nUDPVer);
    m_nDataCompVer      = static_cast<quint8>(mo.m_nDataCompVer);
    m_nSupportSecIdent  = static_cast<quint8>(mo.m_nSupportSecIdent);
    m_nSourceExchange1Ver = static_cast<quint8>(mo.m_nSourceExchange1Ver);
    m_nExtendedRequestsVer= static_cast<quint8>(mo.m_nExtendedRequestsVer);
    m_nAcceptCommentVer = static_cast<quint8>(mo.m_nAcceptCommentVer);
    m_bNoViewSharedFiles= mo.m_nNoViewSharedFiles;
    m_bMultiPacket      = mo.m_nMultiPacket;
    m_bSupportsPreview  = mo.m_nSupportsPreview;

    m_bSupportCaptcha   = mo2.support_captcha();
    m_bSourceExt2       = mo2.support_source_ext2();
    m_bExtMultipacket   = mo2.support_ext_multipacket();
    m_bLargeFiles       = mo2.support_large_files();
}

QString fromHash(const libed2k::md4_hash& h) {
    return misc::toQStringU(h.toString());
}

bool writeResumeData(const libed2k::save_resume_data_alert* p, const QString& path, bool onDisk, const QDateTime& ts)
{
    qDebug() << Q_FUNC_INFO;
    try {
        QED2KHandle h(p->m_handle);
        qDebug() << "save fast resume data for " << h.hash();

        if (h.is_valid() && p->resume_data) {
            QDir bkpDir(path);
            // Remove old fastresume file if it exists
            std::vector<char> out;
            libed2k::bencode(back_inserter(out), *p->resume_data);
            const QString filepath = bkpDir.absoluteFilePath(h.filename());
            libed2k::transfer_resume_data trd(p->m_handle.hash(), p->m_handle.name(), p->m_handle.size(), h.is_seed(), out);
            // append additional tag about has transfer disk file
            trd.m_fast_resume_data.add_tag(libed2k::make_typed_tag(onDisk, libed2k::CT_EMULE_RESERVED1, true));
            // append creation date
            trd.m_fast_resume_data.add_tag(libed2k::make_string_tag(ts.toString(Qt::ISODate).toUtf8().constData(), libed2k::CT_EMULE_RESERVED2, true));

            std::ofstream fs(filepath.toLocal8Bit(), std::ios_base::out | std::ios_base::binary);

            if (fs) {
                libed2k::archive::ed2k_oarchive oa(fs);
                oa << trd;
                return true;
            }
        }
    }
    catch(const libed2k::libed2k_exception& e) {
        qDebug() << "error on write resume data " << misc::toQStringU(e.what());
    }

    return false;
}

bool writeResumeDataOne(std::ofstream& fs, const libed2k::save_resume_data_alert* p)
{
    try
    {
        QED2KHandle h(p->m_handle);
        if (h.is_valid() && p->resume_data)
        {
            std::vector<char> out;
            libed2k::bencode(back_inserter(out), *p->resume_data);
            libed2k::transfer_resume_data trd(p->m_handle.hash(), p->m_handle.name(), p->m_handle.size(), p->m_handle.is_seed(), out);
            libed2k::archive::ed2k_oarchive oa(fs);
            oa << trd;
            return true;
        }
    }
    catch(const libed2k::libed2k_exception& e)
    {
        qDebug() << "error on write resume data " << misc::toQStringU(e.what());
        return false;
    }

    return false;
}

QED2KSession::QED2KSession() : m_upnp(0), m_natpmp(0) {   
    connect(&alertsTimer, SIGNAL(timeout()), this, SLOT(readAlerts()));
    m_speedMon.reset(new TransferSpeedMonitor(this));
    last_error_dt = QDateTime::currentDateTime().addSecs(-1);
}

void QED2KSession::start()
{
    qDebug() <<  Q_FUNC_INFO;
    Preferences pref;
    libed2k::session_settings settings;
    libed2k::fingerprint finger;

    // permanent user hash
    if (pref.userHash().isEmpty()){
        pref.setUserHash(misc::userHash());
    }

    // set zero to port for stop automatically listening        
    settings.user_agent  = libed2k::md4_hash::fromString(pref.userHash().toStdString());
    qDebug() << "user hash: " << pref.userHash();
    settings.listen_port = pref.listenPort();
    settings.m_collections_directory = misc::ED2KCollectionLocation().toUtf8().constData();
    settings.m_known_file = misc::emuleConfig("known.met").toUtf8().constData(); // always set known because user can close before all hashes will process
    settings.client_name  = pref.nick().toUtf8().constData();
    settings.mod_name = PRODUCT_NAME;
    settings.peer_connect_timeout = 100;
    settings.block_request_timeout = 60;
    settings.mod_major = VERSION_MAJOR;
    settings.mod_minor = VERSION_MINOR;
    settings.mod_build = VERSION_BUILD;

    m_session.reset(new libed2k::session(finger, NULL, settings));
    m_session->set_alert_mask(libed2k::alert::all_categories);
    m_session->set_alert_queue_size_limit(100000);

    // start listening on special interface and port and start server connection
    configureSession();
    alertsTimer.setInterval(1000);
    alertsTimer.start();
	m_speedMon->start();
}

void QED2KSession::stop()
{
    alertsTimer.stop();
    m_session->pause();
    saveFastResumeData();
}

bool QED2KSession::started() const { return !m_session.isNull(); }

QED2KSession::~QED2KSession()
{
    if (started())
    {
        stop();
    }
}


QED2KHandle QED2KSession::getTransfer(const QString &hash) const {
    return QED2KHandle(m_session->find_transfer(libed2k::md4_hash::fromString(hash.toStdString())));
}

QLinkedList<QED2KHandle> QED2KSession::getTransfers() const {
    std::vector<libed2k::transfer_handle> handles = m_session->get_transfers();
    QLinkedList<QED2KHandle> transfers;

    for (std::vector<libed2k::transfer_handle>::iterator i = handles.begin();
         i != handles.end(); ++i)
        transfers.push_back(QED2KHandle(*i));

    return transfers;
}

QLinkedList<QED2KHandle> QED2KSession::getActiveTransfers() const
{
    std::vector<libed2k::transfer_handle> handles = m_session->get_active_transfers();
    QLinkedList<QED2KHandle> transfers;


    for (std::vector<libed2k::transfer_handle>::iterator i = handles.begin();
         i != handles.end(); ++i)
        transfers.push_back(QED2KHandle(*i));

    return transfers;
}

bool QED2KSession::hasActiveTransfers() const
{
    QLinkedList<QED2KHandle> torrents = getActiveTransfers();
    QLinkedList<QED2KHandle>::iterator torrentIT;
    for (torrentIT = torrents.begin(); torrentIT != torrents.end(); torrentIT++) {
        const QED2KHandle h(*torrentIT);
        if (h.is_valid() && !h.is_seed() && !h.is_paused() )
            return true;
    }
    return false;
}

SessionStatus QED2KSession::getSessionStatus() const { return m_session->status(); }

void QED2KSession::deleteTransfer(const QString& hash, bool delete_files) {
    const QED2KHandle t = getTransfer(hash);
    if (!t.is_valid()) return;

    QFileInfo info(t.filepath());
    QString metaFilepath = QDir(misc::metadataDirectory(info.absolutePath())).absoluteFilePath(t.filename());
    qDebug() << "metadata file " << metaFilepath;

    if (QFile::remove(metaFilepath)) {
        qDebug() << "fast resume data for " << hash << " erased ";
    }

    m_session->remove_transfer(
        t.delegate(),
        delete_files ? libed2k::session::delete_files : libed2k::session::none);
}

void QED2KSession::setDownloadRateLimit(long rate) {
    libed2k::session_settings settings = m_session->settings();
    settings.download_rate_limit = rate;
    m_session->set_settings(settings);
}

void QED2KSession::setUploadRateLimit(long rate) {
    libed2k::session_settings settings = m_session->settings();
    settings.upload_rate_limit = rate;
    m_session->set_settings(settings);
}

void QED2KSession::configureSession() {
    qDebug() << Q_FUNC_INFO;
    Preferences pref;
    const unsigned short old_listenPort = m_session->settings().listen_port;
    const unsigned short new_listenPort = pref.listenPort();
    const int up_limit = pref.upSpeedLimited()?pref.upSpeed():-1;
    const int dl_limit = pref.dlSpeedLimited()?pref.dlSpeed():-1;

    // set common settings before for announce correct nick on server
    libed2k::session_settings s = m_session->settings();
    s.client_name = pref.nick().toUtf8().constData();
    s.m_show_shared_catalogs = false; //pref.isShowSharedDirectories();
    s.m_show_shared_files = false; //pref.isShowSharedFiles();
    s.download_rate_limit = dl_limit <= 0 ? -1 : dl_limit*1024;
    s.upload_rate_limit = up_limit <= 0 ? -1 : up_limit*1024;
    m_session->set_settings(s);

    if (new_listenPort != old_listenPort) m_session->listen_on(new_listenPort);    
    enableUPnP(false);
}


QPair<QED2KHandle, ErrorCode> QED2KSession::addLink(QString strLink, bool resumed /* = false */)
{
    Preferences pref;
    qDebug("Load ED2K link: %s", strLink.toUtf8().constData());

    libed2k::emule_collection_entry ece =
        libed2k::emule_collection::fromLink(libed2k::url_decode(strLink.toUtf8().constData()));
    QED2KHandle h;
    ErrorCode ec;

    if (ece.defined())
    {
        qDebug("Link is correct, add transfer");
        QString filepath = QDir(pref.inputDir()).filePath(
            QString::fromUtf8(ece.m_filename.c_str(), ece.m_filename.size()));
        libed2k::add_transfer_params atp;
        atp.file_hash = ece.m_filehash;
        atp.file_path = filepath.toUtf8().constData();
        atp.file_size = ece.m_filesize;
        atp.duplicate_is_error = true;

        try
        {
            h = addTransfer(atp);
            if (!resumed) h.pause();
        }
        catch(libed2k::libed2k_exception e)
        {
            ec = e.error();
        }
    }
    else
        ec = "Incorrect link";

    return qMakePair(h, ec);
}

void QED2KSession::addTransferFromFile(const QString& filename, bool resumed /* = false*/)
{
    if (QFile::exists(filename))  {
        ErrorCode ec;
        Preferences pref;
        libed2k::emule_collection ecoll = libed2k::emule_collection::fromFile(
            filename.toLocal8Bit().constData());

        foreach(const libed2k::emule_collection_entry& ece, ecoll.m_files) {
            QString filepath = QDir(pref.inputDir()).filePath(
                QString::fromUtf8(ece.m_filename.c_str(), ece.m_filename.size()));
            qDebug() << "add transfer " << filepath;
            libed2k::add_transfer_params atp;
            atp.file_hash = ece.m_filehash;
            atp.file_path = filepath.toUtf8().constData();
            atp.file_size = ece.m_filesize;
            atp.duplicate_is_error = true;

            try {
                QED2KHandle h = addTransfer(atp);
                if (!resumed) h.pause();
            } catch(libed2k::libed2k_exception e) {
                ec = e.error();
            }
        }
    }
}

QED2KHandle QED2KSession::addTransfer(const libed2k::add_transfer_params& atp){
    return QED2KHandle(delegate()->add_transfer(atp));
}

bool QED2KSession::addTransfer(const QString& hash, const QString& filename, qlonglong size, int sources) {
    using namespace libed2k;
    qDebug() << "download file " << filename << " with hash " << hash << " size " << size;
    EED2KFileType fileType = GetED2KFileTypeID(filename.toStdString());
    if (fileType == ED2KFT_EMULECOLLECTION) {
    //    filename.replace('\\', '-');
    //    filename.replace('/', '-');
    }

    // TODO - fix preferences
    QString filepath = QDir(Preferences().inputDir()).filePath(filename);
    libed2k::add_transfer_params params;
    params.file_hash = libed2k::md4_hash::fromString(hash.toStdString());
    params.file_path = filepath.toUtf8().constData();
    params.file_size = size;
    params.seed_mode = false;
    params.num_complete_sources = sources;
    QED2KHandle h = addTransfer(params);
    return h.is_valid();
}

QString QED2KSession::postTransfer(const libed2k::add_transfer_params& atp)
{
    {
        // do not create file on windows with last point because of Qt truncate it point!
        bool touch = true;
#ifdef Q_OS_WIN
        touch = (!atp.file_path.empty() && (atp.file_path.at(atp.file_path.size() - 1) != '.'));
#endif
        QFile f(misc::toQStringU(atp.file_path));
        // file not exists, need touch and transfer are not exists
        if (!f.exists() && touch && !QED2KHandle(delegate()->find_transfer(atp.file_hash)).is_valid())
        {
            f.open(QIODevice::WriteOnly);
        }
    }

    delegate()->post_transfer(atp);
    return misc::toQString(atp.file_hash);
}

libed2k::session* QED2KSession::delegate() const { return m_session.data(); }

const libed2k::ip_filter& QED2KSession::session_filter() const { return delegate()->get_ip_filter(); }

void QED2KSession::searchFiles(const QString& strQuery,
        quint64 nMinSize,
        quint64 nMaxSize,
        unsigned int nSources,
        unsigned int nCompleteSources,
        QString strFileType,
        QString strFileExt,
        QString strMediaCodec,
        quint32 nMediaLength,
        quint32 nMediaBitrate)
{
    try
    {
        libed2k::search_request sr = libed2k::generateSearchRequest(nMinSize, nMaxSize, nSources, nCompleteSources,
            strFileType.toUtf8().constData(),
            strFileExt.toUtf8().constData(),
            strMediaCodec.toUtf8().constData(),
            nMediaLength,
            nMediaBitrate,
            strQuery.toUtf8().constData());

        m_session->post_search_request(sr);
    }
    catch(libed2k::libed2k_exception& e)
    {
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void QED2KSession::pauseTransfer(const QString& hash) {
    QED2KHandle h = getTransfer(hash);
    if (h.is_valid()) h.pause();
}

void QED2KSession::resumeTransfer(const QString& hash) {
    QED2KHandle h = getTransfer(hash);
    if (h.is_valid()) h.resume();
}

void QED2KSession::searchRelatedFiles(QString strHash)
{
    libed2k::search_request sr = (generateSearchRequest(QStringToMD4(strHash)));
    m_session->post_search_request(sr);
}

void QED2KSession::searchMoreResults()
{
    m_session->post_search_more_result_request();
}

void QED2KSession::cancelSearch()
{
    m_session->post_cancel_search();
}

void QED2KSession::openTransfer(QString hash) {
    QED2KHandle h = getTransfer(hash);
    if (h.is_valid() && h.is_seed()) {
        QUrl u1 = QUrl::fromLocalFile(h.filepath());
        QUrl u2(h.filepath());

        qDebug() << u1 << " 2 " << u2;
        //if (u1.isValid()) qDebug() << "u1 valid";
        //if (u2.isValid()) qDebug() << "u2 valid";

        //if (QDesktopServices::openUrl(u1))
        //{
        //    qDebug() << u1 << " opened";
        //}

        if (QDesktopServices::openUrl(u2))
        {
            qDebug() << u2 << " opened";

        }
    }
}

libed2k::peer_connection_handle QED2KSession::getPeer(const libed2k::net_identifier& np)
{
    libed2k::peer_connection_handle pch = m_session->find_peer_connection(np);

    if (pch.empty())
    {
        pch = m_session->add_peer_connection(np);
    }
    return pch;
}

libed2k::peer_connection_handle QED2KSession::findPeer(const libed2k::net_identifier& np)
{
    libed2k::peer_connection_handle pch = m_session->find_peer_connection(np);

    return pch;
}

void QED2KSession::readAlerts()
{
    std::auto_ptr<libed2k::alert> a = m_session->pop_alert();

    while (a.get())
    {
        if (libed2k::server_name_resolved_alert* p =
            dynamic_cast<libed2k::server_name_resolved_alert*>(a.get()))
        {
            emit serverNameResolved(misc::toQStringU(p->name), QString::fromUtf8(p->endpoint.c_str(), p->endpoint.size()));
        }
        if (libed2k::server_connection_initialized_alert* p =
            dynamic_cast<libed2k::server_connection_initialized_alert*>(a.get()))
        {
            emit serverConnectionInitialized(misc::toQStringU(p->name), misc::toQString(p->host), p->port, p->client_id, p->tcp_flags, p->aux_port);
            qDebug() << "server initialized: " << QString::fromStdString(p->name) << " " << QString::fromStdString(p->host) << " " << p->port;
        }
        else if (libed2k::server_status_alert* p = dynamic_cast<libed2k::server_status_alert*>(a.get()))
        {
            emit serverStatus(misc::toQStringU(p->name), p->files_count, p->users_count);
        }
        else if (libed2k::server_identity_alert* p = dynamic_cast<libed2k::server_identity_alert*>(a.get()))
        {
            emit serverIdentity(misc::toQStringU(p->name), QString::fromUtf8(p->server_name.c_str(), p->server_name.size()),
                                QString::fromUtf8(p->server_descr.c_str(), p->server_descr.size()));
        }
        else if (libed2k::server_message_alert* p = dynamic_cast<libed2k::server_message_alert*>(a.get()))
        {
            emit serverMessage(misc::toQStringU(p->name), QString::fromUtf8(p->server_message.c_str(), p->server_message.size()));
        }
        else if (libed2k::server_connection_closed* p =
                 dynamic_cast<libed2k::server_connection_closed*>(a.get()))
        {
            emit serverConnectionClosed(misc::toQStringU(p->name), misc::toQString(p->host), p->port, QString::fromLocal8Bit(p->m_error.message().c_str()));
        }
        else if (libed2k::shared_files_alert* p = dynamic_cast<libed2k::shared_files_alert*>(a.get()))
        {
            QList<QED2KSearchResultEntry> vRes;
            vRes.reserve(p->m_files.m_collection.size());
            bool bMoreResult = p->m_more;

            for (size_t n = 0; n < p->m_files.m_collection.size(); ++n)
            {
                QED2KSearchResultEntry sre = QED2KSearchResultEntry::fromSharedFileEntry(p->m_files.m_collection[n]);
                if (sre.isCorrect()) vRes.push_back(sre);
            }

            // emit special signal for derived class
            if (libed2k::shared_directory_files_alert* p2 =
                dynamic_cast<libed2k::shared_directory_files_alert*>(p))
            {
                emit peerSharedDirectoryFiles(
                    p2->m_np, md4toQString(p2->m_hash),
                    QString::fromUtf8(p2->m_strDirectory.c_str(), p2->m_strDirectory.size()), vRes);
            }
            else if (libed2k::ismod_shared_directory_files_alert* p2 =
                     dynamic_cast<libed2k::ismod_shared_directory_files_alert*>(p))
            {
                emit peerIsModSharedFiles(p2->m_np, md4toQString(p2->m_hash), md4toQString(p2->m_dir_hash), vRes);
            }
            else
            {
                emit searchResult(p->m_np, md4toQString(p->m_hash), vRes, bMoreResult);
                emit searchFinished(vRes.size(), bMoreResult);
            }
        }
        else if (libed2k::listen_failed_alert* p = dynamic_cast<libed2k::listen_failed_alert*>(a.get()))
        {
            Q_UNUSED(p)
            // TODO - process signal - it means we have different client on same port
        }
        else if (libed2k::peer_connected_alert* p = dynamic_cast<libed2k::peer_connected_alert*>(a.get()))
        {
            emit peerConnected(p->m_np, md4toQString(p->m_hash), p->m_active);
        }
        else if (libed2k::peer_disconnected_alert* p = dynamic_cast<libed2k::peer_disconnected_alert*>(a.get()))
        {
            emit peerDisconnected(p->m_np, md4toQString(p->m_hash), p->m_ec);
        }
        else if (libed2k::peer_message_alert* p = dynamic_cast<libed2k::peer_message_alert*>(a.get()))
        {
            emit peerMessage(p->m_np, md4toQString(p->m_hash),
                             QString::fromUtf8(p->m_strMessage.c_str(), p->m_strMessage.size()));
        }
        else if (libed2k::peer_captcha_request_alert* p =
                 dynamic_cast<libed2k::peer_captcha_request_alert*>(a.get()))
        {
            QPixmap pm;
            if (!p->m_captcha.empty()) pm.loadFromData((const uchar*)&p->m_captcha[0], p->m_captcha.size()); // avoid windows rtl error
            emit peerCaptchaRequest(p->m_np, md4toQString(p->m_hash), pm);
        }
        else if (libed2k::peer_captcha_result_alert* p =
                 dynamic_cast<libed2k::peer_captcha_result_alert*>(a.get()))
        {
            emit peerCaptchaResult(p->m_np, md4toQString(p->m_hash), p->m_nResult);
        }
        else if (libed2k::shared_files_access_denied* p =
                 dynamic_cast<libed2k::shared_files_access_denied*>(a.get()))
        {
            emit peerSharedFilesAccessDenied(p->m_np, md4toQString(p->m_hash));
        }
        else if (libed2k::shared_directories_alert* p = dynamic_cast<libed2k::shared_directories_alert*>(a.get()))
        {
            QStringList qstrl;

            for (size_t n = 0; n < p->m_dirs.size(); ++n)
            {
                qstrl.append(QString::fromUtf8(p->m_dirs[n].c_str(), p->m_dirs[n].size()));
            }

            emit peerSharedDirectories(p->m_np, md4toQString(p->m_hash), qstrl);
        }
        else if (libed2k::added_transfer_alert* p =
                 dynamic_cast<libed2k::added_transfer_alert*>(a.get()))
        {
            QED2KHandle h(p->m_handle);
            if (!h.is_seed()) m_currentSessionTransfers.insert(h.hash());

            if (!m_addTimes.contains(h.hash())) {
                m_addTimes.insert(h.hash(), QDateTime::currentDateTime());
            }

            if (m_sharedFiles.contains(h.hash())) {
                emit transferShared(h);
            } else {
                emit transferAdded(QED2KHandle(h));
            }
        }
        else if (libed2k::paused_transfer_alert* p =
                 dynamic_cast<libed2k::paused_transfer_alert*>(a.get()))
        {
            emit transferPaused(QED2KHandle(p->m_handle));
        }
        else if (libed2k::resumed_transfer_alert* p =
                 dynamic_cast<libed2k::resumed_transfer_alert*>(a.get()))
        {
            emit transferResumed(QED2KHandle(p->m_handle));
        }
        else if (libed2k::deleted_transfer_alert* p =
                 dynamic_cast<libed2k::deleted_transfer_alert*>(a.get()))
        {            
            emit transferDeleted(QString::fromStdString(p->m_hash.toString()));
        }
        else if (libed2k::finished_transfer_alert* p =
                 dynamic_cast<libed2k::finished_transfer_alert*>(a.get()))
        {
            QED2KHandle h(p->m_handle);

            if (m_sharedFiles.contains(h.hash())) {
                emit transferRestored(h);
            } else {
                emit transferFinished(h);
            }
        }
        else if (libed2k::save_resume_data_alert* p = dynamic_cast<libed2k::save_resume_data_alert*>(a.get()))
        {
            Preferences pref;
            QED2KHandle h(p->m_handle);
            QDateTime t;
            writeResumeData(p, misc::metadataDirectory(pref.inputDir()), QFile::exists(h.filepath()), m_addTimes.value(h.hash(), t));
        }
        else if (libed2k::transfer_params_alert* p = dynamic_cast<libed2k::transfer_params_alert*>(a.get()))
        {
            QFileInfo info(misc::toQStringU(p->m_atp.file_path));
            if (info.absoluteDir() == QDir(m_currentPath)) {
                emit transferParametersReady(p->m_atp, p->m_ec);
                if (!p->m_ec) m_sharedFiles << addTransfer(p->m_atp).hash();
                else qDebug() << "transfer params error: " << misc::toQStringU(p->m_ec.message());
            } else {
                qDebug() << "ignore add transfer parameters for old directory: " << misc::toQStringU(p->m_atp.file_path);
            }
        }
        else if (libed2k::file_renamed_alert* p = dynamic_cast<libed2k::file_renamed_alert*>(a.get()))
        {
            emit transferSavePathChanged(QED2KHandle(p->m_handle));
        }
        else if (libed2k::storage_moved_alert* p = dynamic_cast<libed2k::storage_moved_alert*>(a.get()))
        {
            emit transferStorageMoved(QED2KHandle(p->m_handle));
        }
        else if (libed2k::file_error_alert* p = dynamic_cast<libed2k::file_error_alert*>(a.get()))
        {
            QED2KHandle h(p->m_handle);
            if (h.is_valid()) {
                emit fileError(h, QString::fromLocal8Bit(p->error.message().c_str(), p->error.message().size()));
                QDateTime cdt = QDateTime::currentDateTime();
                if (last_error_dt.secsTo(cdt) > 2) {
                    emit fileIOError(h.filename(), QString::fromLocal8Bit(p->error.message().c_str(), p->error.message().size()));
                    last_error_dt = cdt;
                }
                h.pause();
            }
        }
        else if (libed2k::portmap_error_alert* p = dynamic_cast<libed2k::portmap_error_alert*>(a.get()))
        {
            qDebug("UPnP Failure, msg: %s", p->message().c_str());
            //addConsoleMessage(tr("UPnP/NAT-PMP: Port mapping failure, message: %1")
            //                  .arg(misc::toQString(p->message())), "red");
        }
        else if (libed2k::portmap_alert* p = dynamic_cast<libed2k::portmap_alert*>(a.get()))
        {
            qDebug("UPnP Success, msg: %s", p->message().c_str());
            //addConsoleMessage(tr("UPnP/NAT-PMP: Port mapping successful, message: %1")
            //                  .arg(misc::toQString(p->message())), "blue");
        }

        a = m_session->pop_alert();
    }

    // deferred transfers loading and files sharing
    if (!m_fastTransfers.empty()) {
        QList<QString> keys = m_fastTransfers.keys();
        int counter = 10;

        while(!keys.empty() && counter != 0 ) {
            QString key = keys.takeFirst();
            libed2k::transfer_resume_data trd = m_fastTransfers.take(key);
            libed2k::add_transfer_params atp;
            QString filepath = QDir(m_currentPath).filePath(misc::toQStringU(trd.m_filename.m_collection));
            boost::shared_ptr<libed2k::base_tag> onDisk = trd.m_fast_resume_data.getTagByNameId(libed2k::CT_EMULE_RESERVED1);

            Q_ASSERT(onDisk);

            if (QFile::exists(filepath) || !onDisk->asBool()) {
                atp.seed_mode = false;
                // make full path for transfer startup
                atp.file_path = filepath.toUtf8().constData();
                atp.file_size = trd.m_filesize;
                atp.file_hash = trd.m_hash;

                if (trd.m_fast_resume_data.count() > 0) {
                    atp.resume_data = const_cast<std::vector<char>* >(
                        &trd.m_fast_resume_data.getTagByNameId(libed2k::FT_FAST_RESUME_DATA)->asBlob());
                }

                QED2KHandle h = addTransfer(atp);
                m_sharedFiles << h.hash();
                boost::shared_ptr<libed2k::base_tag> addDt = trd.m_fast_resume_data.getTagByNameId(libed2k::CT_EMULE_RESERVED2);

                if (addDt) {
                    m_addTimes.insert(h.hash(), QDateTime::fromString(misc::toQStringU(addDt->asString()), Qt::ISODate));
                }
            } else {
                qDebug() << "fast resume data file" << key << " lost target file, remove resume data";
                if (QFile::remove(QDir(misc::metadataDirectory(m_currentPath)).filePath(key))) {
                    qDebug() << "fast resume data file erased";
                }
            }

            --counter;
        }
    } else {
        int counter = 4;
        while(!m_pendingFiles.empty() && counter != 0) {
            makeTransferParametersAsync(m_pendingFiles.takeFirst());
            --counter;
        }
    }
}

// Called periodically
void QED2KSession::saveTempFastResumeData()
{
    std::vector<libed2k::transfer_handle> transfers =  m_session->get_transfers();

    for (std::vector<libed2k::transfer_handle>::iterator th_itr = transfers.begin();
         th_itr != transfers.end(); ++th_itr)
    {
        QED2KHandle h = QED2KHandle(*th_itr);

        try
        {
            if (h.is_valid() &&
                    h.state() != QED2KHandle::checking_files &&
                    h.state() != QED2KHandle::queued_for_checking &&
                    h.need_save_resume_data())
            {
                qDebug("Saving fastresume data for %s", qPrintable(h.name()));
                h.save_resume_data();
            }
        }
        catch(std::exception&)
        {}
    }
}

void QED2KSession::saveFastResumeData()
{
    qDebug("Saving fast resume data...");
    Preferences pref;
    int part_num = 0;
    int num_resume_data = 0;
    // Pause session
    delegate()->pause();
    std::vector<libed2k::transfer_handle> transfers =  delegate()->get_transfers();

    for (std::vector<libed2k::transfer_handle>::iterator th_itr = transfers.begin(); th_itr != transfers.end(); th_itr++) {
        QED2KHandle h = QED2KHandle(*th_itr);
        if (!h.is_valid()) {
            qDebug() << "transfer invalid or hasn't metadata";
            continue;
        }

        try {

            if (h.state() == QED2KHandle::checking_files || h.state() == QED2KHandle::queued_for_checking) {
                qDebug() << "skip transfer " << h.hash() << " because of checking files or queued for checking";
                continue;
            }

            if (h.need_save_resume_data()) {
                if(!h.is_seed())
                    ++part_num;
                h.save_resume_data();
                ++num_resume_data;
            }
        }
        catch(libed2k::libed2k_exception& e) {
            qDebug() << "exception on save fast resume " << misc::toQStringU(e.what());
        }
    }

    while (num_resume_data > 0) {
        libed2k::alert const* a = delegate()->wait_for_alert(libed2k::seconds(30));

        if (a == 0) {
            qDebug("On save fast resume data we got empty alert - alert wasn't generated");
            break;
        }

        if (libed2k::save_resume_data_failed_alert const* rda = dynamic_cast<libed2k::save_resume_data_failed_alert const*>(a)) {
            qDebug() << "save resume data failed alert " << misc::toQStringU(rda->message().c_str());
            --num_resume_data;

            try {
                if (rda->m_handle.is_valid())
                    delegate()->remove_transfer(rda->m_handle);
            }
            catch(const libed2k::libed2k_exception& e) {
                qDebug() << "exception on remove transfer after save " << misc::toQStringU(e.what());
            }
        } else if (libed2k::save_resume_data_alert const* rd = dynamic_cast<libed2k::save_resume_data_alert const*>(a)) {
            --num_resume_data;
            QED2KHandle h(rd->m_handle);
            QDateTime t;
            writeResumeData(rd, misc::metadataDirectory(pref.inputDir()), QFile::exists(h.filepath()), m_addTimes.value(h.hash(), t));

            try
            {
                delegate()->remove_transfer(rd->m_handle);
            }
            catch(const libed2k::libed2k_exception& e)
            {
                qDebug() << "exception on remove transfer after save " << misc::toQStringU(e.what());
            }
        }

        delegate()->pop_alert();
    }
}

void QED2KSession::loadFastResumeData(const QString& path) {
    qDebug() << Q_FUNC_INFO << path;

    // we need files 32 length(MD4_HASH_SIZE*2) name and extension fastresume
    QStringList filter;
    //filter << "????????????????????????????????.fastresume";

    QDir fastresumeDir(path);
    const QStringList files = fastresumeDir.entryList(filter, QDir::Files, QDir::Unsorted);

    foreach (const QString &file, files) {
        qDebug("Trying to load fastresume data: %s", qPrintable(file));
        const QString file_abspath = fastresumeDir.absoluteFilePath(file);
        // extract hash from name
        //libed2k::md4_hash hash = libed2k::md4_hash::fromString(
        //    file.toStdString().substr(0, libed2k::MD4_HASH_SIZE*2));

        try {
            std::ifstream fs(file_abspath.toLocal8Bit(), std::ios_base::in | std::ios_base::binary);

            if (fs)
            {
                libed2k::transfer_resume_data trd;
                libed2k::archive::ed2k_iarchive ia(fs);
                ia >> trd;

                // add transfer
                PersistentDataItem item;
                item.m_params = QSharedPointer<libed2k::add_transfer_params>(new libed2k::add_transfer_params);
                item.m_params->seed_mode = false;
                item.m_params->file_path = trd.m_filename.m_collection;
                item.m_params->file_size = trd.m_filesize;
                item.m_params->file_hash = trd.m_hash;

                if (trd.m_fast_resume_data.count() > 0)
                {
                    item.m_params->resume_data = const_cast<std::vector<char>* >(
                        &trd.m_fast_resume_data.getTagByNameId(libed2k::FT_FAST_RESUME_DATA)->asBlob());
                }

                boost::shared_ptr<libed2k::base_tag> resumeTag = trd.m_fast_resume_data.getTagByNameId(libed2k::FT_FAST_RESUME_DATA);

                if (resumeTag) {
                    item.m_params->resume_data = const_cast<std::vector<char>* >(&resumeTag->asBlob());
                }

                boost::shared_ptr<libed2k::base_tag> birthdayTag = trd.m_fast_resume_data.getTagByNameId(libed2k::CT_EMULE_RESERVED1);

                if (birthdayTag) {
                    item.m_birthday = QDateTime::fromString(misc::toQStringU(birthdayTag->asString()), Qt::ISODate);
                } else {
                    item.m_birthday = QDateTime::currentDateTime();
                }

                /*
                //QFileInfo qfi(QString::fromUtf8(trd.m_filepath.m_collection.c_str()));
                // add transfer only when file still exists
                if (qfi.exists() && qfi.isFile())
                {
                    //QED2KHandle h(delegate()->add_transfer(params));
                    //m_fast_resume_transfers.insert(h.hash(), h);
                }
                else
                {
                    qDebug() << "file not exists: " << qfi.fileName();
                    // check file should exists!
                }
                */
            }
        }
        catch(const libed2k::libed2k_exception& e) {
            qDebug() << "incorrect fast resume file format detected: " << misc::toQStringU(e.what());
            QFile::remove(file_abspath);
        }
    }
}

void QED2KSession::enableUPnP(bool b)
{
    Preferences pref;
    if (b) {
        if (!m_upnp) {
            qDebug("Enabling UPnP / NAT-PMP");
            m_upnp = delegate()->start_upnp();
            m_natpmp = delegate()->start_natpmp();
        }
    } else {
        if (m_upnp) {
            qDebug("Disabling UPnP / NAT-PMP");
            delegate()->stop_upnp();
            delegate()->stop_natpmp();
            m_upnp = 0;
            m_natpmp = 0;
        }
    }
}

void QED2KSession::deferPlayMedia(QED2KHandle h) {
    if (h.is_valid() && !playMedia(h)) {
        qDebug() << "Defer playing file: " << h.filename();
        h.set_sequential_download(false);
        h.prioritize_extremity_pieces(true);
        h.set_eager_mode(true);
        m_pending_medias.insert(h);
    }
}

void QED2KSession::playLink(const QString& strLink) {
    deferPlayMedia(addLink(strLink).first);
}

bool QED2KSession::playMedia(QED2KHandle h) {
    if (h.is_valid() && misc::isPreviewable(misc::file_extension(h.filename())))
    {
        TransferBitfield pieces = h.pieces();
        const QList<int> extremity_pieces = h.file_extremity_pieces_at();

        // check we have all boundary pieces for the file
        foreach (int p, extremity_pieces) if (!pieces[p]) return false;

        h.set_sequential_download(true);
        return QDesktopServices::openUrl(QUrl::fromLocalFile(h.filepath()));
    }

    return false;
}

void QED2KSession::playPendingMedia() {
    for (QSet<QED2KHandle>::iterator i = m_pending_medias.begin(); i != m_pending_medias.end();) {
        if (!i->is_valid() || playMedia(*i))
            i = m_pending_medias.erase(i);
        else
            ++i;
    }
}

void QED2KSession::startServerConnection(const QED2KServer& s) {
    libed2k::server_connection_parameters sp;
    sp.name = s.alias.toUtf8().constData();
    sp.host = s.host.toUtf8().constData();
    sp.port = s.port;
    sp.set_operations_timeout(30);
    sp.set_keep_alive_timeout(100);
    sp.set_reconnect_timeout(100);
    sp.set_announce_timeout(20);
    sp.announce_items_per_call_limit = 60;
    delegate()->server_connect(sp);
}

void QED2KSession::stopServerConnection() { delegate()->server_disconnect(); }

bool QED2KSession::isServerConnected() const
{
    return (delegate()->server_connection_established());
}

void QED2KSession::makeTransferParametersAsync(const QString& filepath)
{
    delegate()->make_transfer_parameters(filepath.toUtf8().constData());
}

void QED2KSession::cancelTransferParameters(const QString& filepath)
{
    delegate()->cancel_transfer_parameters(filepath.toUtf8().constData());
}

std::pair<libed2k::add_transfer_params, libed2k::error_code> QED2KSession::makeTransferParameters(const QString& filepath) const
{
    bool cancel = false;
    return libed2k::file2atp()(filepath.toUtf8().constData(), cancel);
}

QString QED2KSession::makeLink(const QString &filename, long filesize, const QString &filehash)
{
    return misc::toQStringU(libed2k::emule_collection::toLink(filename.toUtf8().constData(), filesize,
                                                         libed2k::md4_hash::fromString(filehash.toStdString()), true));
}

qreal QED2KSession::getRealRatio(const QString &hash) const
{
    QED2KHandle h = getTransfer(hash);
    if (!h.is_valid()) {
        return 0.;
    }

    libed2k::size_type all_time_upload = h.all_time_upload();
    libed2k::size_type all_time_download = h.all_time_download();
    if (all_time_download == 0 && h.is_seed()) {
        // Purely seeded transfer
        all_time_download = h.total_done();
    }

    if (all_time_download == 0) {
        if (all_time_upload == 0)
            return 0;
        return MAX_RATIO+1;
    }

    qreal ratio = all_time_upload / (float) all_time_download;
    Q_ASSERT(ratio >= 0.);
    if (ratio > MAX_RATIO)
        ratio = MAX_RATIO;
    return ratio;
}

void QED2KSession::loadDirectory(const QString& path) {
    qDebug() << Q_FUNC_INFO << path;
    if (QDir(m_currentPath) == QDir(path)) return;
    emit resetInputDirectory(path);

    QLinkedList<QED2KHandle> transfers = Session::instance()->getTransfers();
    foreach(QED2KHandle h, transfers) {
        if (h.is_seed()) Session::instance()->deleteTransfer(h.hash(), false);
    }

    m_pendingFiles.clear();
    m_sharedFiles.clear();
    m_fastTransfers.clear();

    m_currentPath = path;
    QStringList filter;
    //filter << "????????????????????????????????.fastresume";

    QDir fastresumeDir(misc::metadataDirectory(path));
    const QStringList files = fastresumeDir.entryList(filter, QDir::Files, QDir::Unsorted);

    foreach (const QString &file, files) {
        qDebug("Trying to load fastresume data: %s", qPrintable(file));
        const QString file_abspath = fastresumeDir.absoluteFilePath(file);
        // extract hash from name
        //libed2k::md4_hash hash = libed2k::md4_hash::fromString(
        //    file.toStdString().substr(0, libed2k::MD4_HASH_SIZE*2));

        try {
            std::ifstream fs(file_abspath.toLocal8Bit(), std::ios_base::in | std::ios_base::binary);

            if (fs) {
                libed2k::transfer_resume_data trd;
                libed2k::archive::ed2k_iarchive ia(fs);
                ia >> trd;
                //item.m_params->seed_mode = false;
                //item.m_params->file_path = trd.m_filepath.m_collection;
                //item.m_params->file_size = trd.m_filesize;
                //item.m_params->file_hash = trd.m_hash;
                m_fastTransfers.insert(misc::toQStringU(trd.m_filename.m_collection), trd);
            }
        }
        catch(const libed2k::libed2k_exception& e) {
            qDebug() << "incorrect fast resume file format detected: " << misc::toQStringU(e.what());
            QFile::remove(file_abspath);
        }
    }

    QDirIterator dirIt(path, QDir::NoDotAndDotDot | QDir::Files);
    while(dirIt.hasNext()) {
        QString filepath = dirIt.next();
        QFileInfo info = dirIt.fileInfo();
        if (!m_fastTransfers.contains(info.fileName())) {
            m_pendingFiles << filepath;
        }
    }
}

QDateTime QED2KSession::hasBeenAdded(const QString& hash) const {
    QDateTime t;
    return m_addTimes.value(hash, t);
}

qlonglong QED2KSession::getETA(const QString& hash) const {
    return m_speedMon->getETA(hash);
}

QString QED2KSession::status(const QString& hash) const {
    return m_currentSessionTransfers.contains(hash)?"N":"R";
}
