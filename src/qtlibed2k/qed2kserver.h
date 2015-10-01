#ifndef __QED2KSERVER__
#define __QED2KSERVER__

#include <QString>
#include <QPair>
#include <QList>

#ifndef Q_MOC_RUN
#include <libed2k/file.hpp>
#endif

struct QED2KServerFingerprint {
    QString alias;
    QString host;
    int port;

    QED2KServerFingerprint();
    QED2KServerFingerprint(const QString& a, const QString& h, int p);

    bool isDefined() const;

    bool operator<(const QED2KServerFingerprint& sf) const {
        return qMakePair(alias, qMakePair(host, port)) < qMakePair(sf.alias, qMakePair(sf.host, sf.port));
    }
};

struct QED2KServer {
    QED2KServerFingerprint fingerprint;
    quint32 clientId;
    QString serverIp;
    quint32 filesCount;
    quint32 usersCount;
    quint32 maxUsers;
    quint32 lowidUsers;
    QString name;
    QString description;

    QED2KServer(const QED2KServerFingerprint&);
    static QED2KServer fromServerMetEntry(const libed2k::server_met_entry&);

    bool operator<(const QED2KServer& s) const {
        return fingerprint < s.fingerprint;
    }
};

QList<QED2KServer> fromServersMet(const QString& filename);

#endif
