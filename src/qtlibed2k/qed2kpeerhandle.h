#ifndef QED2KPEERHANDLE_H
#define QED2KPEERHANDLE_H

#ifndef Q_MOC_RUN
#include <libed2k/session.hpp>
#endif

class QED2KPeerHandle {
public:
    QED2KPeerHandle(const libed2k::peer_connection_handle& pch);
    ~QED2KPeerHandle();

    void    sendMessageToPeer(const QString& strMessage);
    bool    isAllowedSharedFilesView();
    void    requestDirs();
    void    requestFiles(QString dirName);
    void    requestDirFiles(QString dirHash);
    QString getUserName();

    libed2k::md4_hash getHash();

    libed2k::peer_connection_options getConnectionOptions();

private:
    libed2k::peer_connection_handle m_delegate;
};

#endif // QED2KPEERHANDLE_H
