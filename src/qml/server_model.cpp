#include "server_model.h"


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
    case StatusRole: return ServerDisconnected; // call session
    default:
        break;
    }

    return QVariant();
}

void ServerModel::add(const QED2KServer& s) {
    beginInsertRows(QModelIndex(), servers.size(), servers.size());
    servers << s;
    endInsertRows();
}

void ServerModel::create(const QString& alias, const QString& host, int port) {
    QED2KServer fp(alias, host, port);
    add(fp);
}

QModelIndex ServerModel::getIndex(const QString& alias) const {
    for(int i = 0; i < servers.size(); ++i) {
        if (servers.at(i).alias == alias) return index(i);
    }

    return QModelIndex();
}

void ServerModel::update(const QString& alias) {
    QModelIndex index = getIndex(alias);
    if (index.isValid()) emit dataChanged(index, index);
}

