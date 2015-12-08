#include "server_model.h"
#include "qed2ksession.h"
#include "preferences.h"
#include <QDebug>

void save_server(const QED2KServer& server, Preferences& pref) {
    pref.setValue("Alias", server.alias);
    pref.setValue("Host", server.host);
    pref.setValue("Port", server.port);
    pref.setValue("IP", server.serverIp);
    pref.setValue("Name", server.name);
    pref.setValue("Descr", server.description);
}

QED2KServer load_server(Preferences& pref) {
    QED2KServer server;
    server.alias = pref.value("Alias", "").toString();
    server.host = pref.value("Host", "").toString();
    server.port = pref.value("Port", 0).toInt();
    server.serverIp = pref.value("IP", "").toString();
    server.name = pref.value("Name", "").toString();
    server.description = pref.value("Descr", "").toString();
    return server;
}

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
        if (servers.at(i) == QED2KServer(alias, host, port)) return index(i);
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

void ServerModel::remove(const QString& alias, const QString& host, int port) {
    QModelIndex index = getIndex(alias, host, port);
    if (index.isValid()) {
        beginRemoveRows(QModelIndex(), index.row(), index.row());
        servers.removeAt(index.row());
        endRemoveRows();
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

void ServerModel::load() {
    Preferences pref;
    if (pref.getServersPresence()) {
        int size = pref.beginReadArray("Servers");
        for(int i = 0; i < size; ++i) {
            pref.setArrayIndex(i);
            servers << load_server(pref);
        }

        pref.endArray();
    } else {
        add(QED2KServer("is74", "emule.is74.ru", 4661));
        add(QED2KServer("TV Underground", "176.103.48.36", 4184));
        add(QED2KServer("eMule Security No1", "91.200.42.46" ,1176));
        add(QED2KServer("eMule Security No2", "91.200.42.47", 3883));
        add(QED2KServer("eMule Security No3", "91.200.42.119", 9939));
        add(QED2KServer("eMule Security No4", "77.120.115.66", 5041));
        add(QED2KServer("eMule oVPN.to Anonymous ed2k-Server", "213.163.71.135", 4242));
        add(QED2KServer("emule lover QQ Qun 212431292", "27.152.28.252", 4242));
        add(QED2KServer("PEERATES.NET", "195.154.83.5" ,7111));
        add(QED2KServer("PeerBooter", "212.83.184.152", 7111));
        add(QED2KServer("!! www.Sharing-Devils.org No.1 ...", "195.154.109.229" ,4232));

        foreach(const QED2KServer s, fromServersMet("./server.met")) {
            add(s);
        }
    }
}

void ServerModel::save() const {
    Preferences pref;
    pref.setServersPresence(true);
    pref.beginWriteArray("Servers", servers.size());
    int index = 0;
    foreach(const QED2KServer& s, servers) {
        pref.setArrayIndex(index++);
        save_server(s, pref);
    }

    pref.endArray();

}

