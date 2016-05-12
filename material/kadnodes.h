#ifndef KADNODES_H
#define KADNODES_H

#include <QAbstractListModel>
#include "qtlibed2k/qed2ksession.h"

class KadNodes : public QAbstractListModel
{
    Q_OBJECT

    QList<KadNode>  nodes;
public:
    enum KADColumns {
        KC_HOST = Qt::UserRole + 1,
        KC_PORT,
        KC_ID,
        KC_DISTANCE,
        KC_END
    };

    explicit KadNodes(QObject *parent = 0);
    void setData(const QList<KadNode>& n);
    QHash<int, QByteArray> roleNames() const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    Q_INVOKABLE int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
};

#endif // KADNODES_H
