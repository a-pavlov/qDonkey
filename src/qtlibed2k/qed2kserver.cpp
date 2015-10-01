#include "qed2kserver.h"
#include <fstream>
#include <QDebug>

QED2KServerFingerprint::QED2KServerFingerprint() {
    port = -1;
}

QED2KServerFingerprint::QED2KServerFingerprint(const QString& a, const QString& h, int p) :
    alias(a), host(h), port(p) {}


bool QED2KServerFingerprint::isDefined() const {
    return !alias.isEmpty() && !host.isEmpty() && port != -1;
}

QED2KServer::QED2KServer(const QED2KServerFingerprint& fp) : fingerprint(fp) {
}

//static
QED2KServer QED2KServer::fromServerMetEntry(const libed2k::server_met_entry& sme) {
    QED2KServerFingerprint fp;
    fp.alias = QString::fromUtf8(sme.m_list.getStringTagByNameId(libed2k::FT_FILENAME).c_str());

    if (sme.m_network_point.m_nIP == 0)
        fp.host = QString::fromStdString(sme.m_list.getStringTagByNameId(libed2k::ST_PREFERENCE));
    else
        fp.host = QString::fromStdString(libed2k::int2ipstr(sme.m_network_point.m_nIP));

    fp.port    = sme.m_network_point.m_nPort;

    QED2KServer s(fp);

    s.clientId   = 0;
    s.filesCount = sme.m_list.getIntTagByName("files");
    s.usersCount = sme.m_list.getIntTagByName("users");
    s.maxUsers   = sme.m_list.getIntTagByNameId(libed2k::ST_MAXUSERS);
    s.lowidUsers = sme.m_list.getIntTagByNameId(libed2k::ST_LOWIDUSERS);
    s.description= QString::fromStdString(sme.m_list.getStringTagByNameId(libed2k::ST_DESCRIPTION));
    return s;
}

QList<QED2KServer> fromServersMet(const QString& filename) {
    libed2k::error_code ec;
    libed2k::server_met smet;
    Q_ASSERT(!filename.isEmpty());

    try {
        std::ifstream fs(filename.toLocal8Bit(), std::ios_base::in | std::ios_base::binary);
        if (fs) {
            libed2k::archive::ed2k_iarchive ia(fs);
            ia >> smet;
            qDebug() << "loaded " << smet.m_servers.m_collection.size() << " servers";
            ec = libed2k::errors::no_error;
        }
    } catch(libed2k::libed2k_exception& e) {
        ec = e.error();
    }

    QList<QED2KServer> res;

    foreach(const libed2k::server_met_entry entry, smet.m_servers.m_collection) {
        res << QED2KServer::fromServerMetEntry(entry);
    }

    return res;
}
