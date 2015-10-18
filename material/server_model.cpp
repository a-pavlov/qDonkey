#include "server_model.h"
#include "qed2ksession.h"
#include <QDebug>

ServerModel::ServerModel(QObject* parent) : QAbstractListModel(parent) {

}

QHash<int, QByteArray> ServerModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[AliasRole]    = "alias";
    roles[HostRole]     = "host";
    roles[PortRole]     = "port";
    roles[UsersCountRole] = "userscount";
    roles[FilesCountRole] = "filescount";
    roles[DescriptionRole] = "description";
    roles[StatusRole]   = "status";
    return roles;
}

int ServerModel::rowCount(const QModelIndex&) const {
    return servers.size();
}

QVariant ServerModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return QVariant();
    Q_ASSERT(index.row() < servers.size());
    const QED2KServer& s = servers.at(index.row());
    switch(role) {
    case Qt::DisplayRole: return s.alias;
    case AliasRole: return s.alias;
    case HostRole: return s.host;
    case PortRole: return s.port;
    case UsersCountRole: return s.usersCount;
    case FilesCountRole: return s.filesCount;
    case DescriptionRole: return s.description;
    case StatusRole: return s.status;
    default:
        break;
    }

    return QVariant();
}

void ServerModel::add(const QED2KServer& s) {
    foreach(const QED2KServer& srv, servers) {
        if (srv == s) {
            return;
        }
    }

    beginInsertRows(QModelIndex(), servers.size(), servers.size());
    servers << s;
    endInsertRows();
}

void ServerModel::create(const QString& alias, const QString& host, int port) {
    QED2KServer fp(alias, host, port);
    add(fp);
}

QModelIndex ServerModel::getIndex(const QString& alias, const QString& host, int port) const {
    for(int i = 0; i < servers.size(); ++i) {
        if (servers.at(i).alias == QED2KServer(alias, host, port)) return index(i);
    }

    return QModelIndex();
}

void ServerModel::update(const QString& alias, const QString& host, int port) {
    QModelIndex index = getIndex(alias, host, port);
    if (index.isValid()) {
        Q_ASSERT(index.row() < servers.size());
        switch(servers.at(index.row()).status) {
            case QED2KServer::ServerConnected:
            case QED2KServer::ServerConnecting:
                qDebug() << "disconnect " << alias;
                Session::instance()->stopServerConnection();
                break;
            case QED2KServer::ServerDisconnected:
                qDebug() << "connect " << alias;
                Session::instance()->startServerConnection(servers.at(index.row()));
                servers[index.row()].status = QED2KServer::ServerConnecting;
                emit dataChanged(index, index);
                break;
            default:
                break;
        }
    }
}

void ServerModel::on_serverConnectionInitialized(QString alias, QString host, int port, quint32 client_id, quint32 tcp_flags, quint32 aux_port) {
    Q_UNUSED(client_id);
    Q_UNUSED(tcp_flags);
    Q_UNUSED(aux_port);
    QModelIndex index = getIndex(alias, host, port);
    if (index.isValid()) {
        Q_ASSERT(index.row() < servers.size());
        servers[index.row()].status = QED2KServer::ServerConnected;
        emit dataChanged(index, index);
    }
}

void ServerModel::on_serverConnectionClosed(QString alias, QString host, int port, QString strError) {
    Q_UNUSED(strError);
    QModelIndex index = getIndex(alias, host, port);
    if (index.isValid()) {
        Q_ASSERT(index.row() < servers.size());
        servers[index.row()].status = QED2KServer::ServerDisconnected;
        emit dataChanged(index, index);
    }
}

