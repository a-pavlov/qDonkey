#ifndef PEERMODEL_H
#define PEERMODEL_H

#include <QAbstractListModel>

class PeerModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Columns { IP, CLIENT, FILE, PROGRESS, SPEED, TOTAL, COL_COUNT };
    explicit PeerModel(QObject *parent = 0);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
signals:
    
public slots:
    
};

#endif // PEERMODEL_H
