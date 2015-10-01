#ifndef __QED2KSERVER__
#define __QED2KSERVER__

#include <QString>
#include <QPair>
#include <QList>

#ifndef Q_MOC_RUN
#include <libed2k/file.hpp>
#endif

struct QED2KServer {
    QString alias;
    QString host;
    qint32  port;
    quint32 clientId;
    QString serverIp;
    quint32 filesCount;
    quint32 usersCount;
    quint32 maxUsers;
    quint32 lowidUsers;
    QString name;
    QString description;

    QED2KServer();
    QED2KServer(const QString& a, const QString& h, qint32 p);
    static QED2KServer fromServerMetEntry(const libed2k::server_met_entry&);

    bool operator<(const QED2KServer& s) const { return alias < s.alias; }
    bool operator==(const QED2KServer& s) const { return alias == s.alias; }
};

QList<QED2KServer> fromServersMet(const QString& filename);

#endif
