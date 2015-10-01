#include "qed2kserver.h"
#include <fstream>
#include <QDebug>

QED2KServer::QED2KServer() : port(-1), clientId(0), filesCount(0), usersCount(0), maxUsers(0), lowidUsers(0) {
}

QED2KServer::QED2KServer(const QString& a, const QString& h, qint32 p) : alias(a), host(h), port(p),
    clientId(0), filesCount(0), usersCount(0), maxUsers(0), lowidUsers(0) {
}

//static
QED2KServer QED2KServer::fromServerMetEntry(const libed2k::server_met_entry& sme) {
    QED2KServer s;
    s.alias = QString::fromUtf8(sme.m_list.getStringTagByNameId(libed2k::FT_FILENAME).c_str());

    if (sme.m_network_point.m_nIP == 0)
        s.host = QString::fromStdString(sme.m_list.getStringTagByNameId(libed2k::ST_PREFERENCE));
    else
        s.host = QString::fromStdString(libed2k::int2ipstr(sme.m_network_point.m_nIP));

    s.port = sme.m_network_point.m_nPort;
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
