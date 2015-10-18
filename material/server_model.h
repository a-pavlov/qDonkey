#ifndef __SERVER_MODEL__
#define __SERVER_MODEL__

#include <QAbstractListModel>
#include <QList>
#include <QHash>
#include <QByteArray>
#include "qed2kserver.h"

class ServerModel : public QAbstractListModel {
    Q_OBJECT
private:
    QList<QED2KServer>  servers;
public:
    enum ServerRoles {
        AliasRole   = Qt::UserRole + 1,
        HostRole,
        PortRole,
        UsersCountRole,
        FilesCountRole,
        DescriptionRole,
        StatusRole,
        SizeRole
    };


    ServerModel(QObject* parent = 0);
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void add(const QED2KServer&);
    Q_INVOKABLE void create(const QString& alias, const QString& host, int port);
    QModelIndex getIndex(const QString& alias, const QString& host, int port) const;

    Q_INVOKABLE void update(const QString& alias, const QString& host, int port);
public slots:
    void on_serverConnectionInitialized(QString alias, QString host, int port, quint32 client_id, quint32 tcp_flags, quint32 aux_port);
    void on_serverConnectionClosed(QString alias, QString host, int port, QString strError);
};

#endif //__SERVER_MODEL__
