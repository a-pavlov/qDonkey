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
    case Qt::DisplayRole: return s.fingerprint.alias;
    case AliasRole: return s.fingerprint.alias;
    case HostRole: return s.fingerprint.host;
    case PortRole: return s.fingerprint.port;
    case UsersCountRole: return s.usersCount;
    case FilesCountRole: return s.filesCount;
    case DescriptionRole: return s.description;
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
