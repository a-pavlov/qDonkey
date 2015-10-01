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

    enum ServerStatus {
        ServerConnected,
        ServerConnecting,
        ServerDisconnected
    };

    ServerModel(QObject* parent = 0);
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    void add(const QED2KServer&);
    Q_INVOKABLE void create(const QString& alias, const QString& host, int port);
    QModelIndex getIndex(const QString& alias) const;

    Q_INVOKABLE void update(const QString& alias);

};

#endif //__SERVER_MODEL__
