#include "kadnodes.h"

KadNodes::KadNodes(QObject *parent/* = 0*/) : QAbstractListModel(parent) {
}


void KadNodes::setData(const QList<KadNode>& n) {
    beginResetModel();
    nodes = n;
    endResetModel();
}

QHash<int, QByteArray> KadNodes::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[KC_HOST]  = "host";
    roles[KC_PORT] = "port";
    roles[KC_ID] = "id";
    roles[KC_DISTANCE]="distance";
    return roles;
}

int KadNodes::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const {
    return KC_END;
}

int KadNodes::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const {
    return nodes.size();
}

QVariant KadNodes::data(const QModelIndex& index, int role) const {
    QVariant result;

    if (!index.isValid()) return result;

    switch(role) {
        case KC_HOST:   return nodes.at(index.row()).host;
        case KC_PORT:   return nodes.at(index.row()).port;
        case KC_ID:     return nodes.at(index.row()).kid;
        case KC_DISTANCE: return nodes.at(index.row()).distance;
    default:
        return result;
    }

    return result;
}
