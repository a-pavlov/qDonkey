#ifndef TRANSFERMODEL_ITEM_H
#define TRANSFERMODEL_ITEM_H

#include <QObject>
#include <QDateTime>
#include <QIcon>
#include <QColor>

#include "qtlibed2k/qed2khandle.h"

class TransferModelItem : public QObject {
    Q_OBJECT
public:
    enum State {
        STATE_DOWNLOADING,
        STATE_QUEUED,
        STATE_STALLED_DL,
        STATE_STALLED_UP,
        STATE_SEEDING,
        STATE_PAUSED_UP,
        STATE_PAUSED_DL,
        STATE_CHECKING,
        STATE_INVALID
    };

    enum Column {
        TM_NAME,
        TM_SIZE,
        TM_TYPE,
        TM_PROGRESS,
        TM_STATUS,
        TM_SEEDS,
        TM_PEERS,
        TM_DLSPEED,
        TM_UPSPEED,
        TM_HASH,
        TM_RATIO,
        TM_ADD_DATE,
        TM_AMOUNT_DOWNLOADED,
        TM_AMOUNT_LEFT,
        TM_TIME_ELAPSED,
        TM_END
    };

public:
    TransferModelItem(const QED2KHandle& h);
    inline int columnCount() const { return TM_END; }
    QVariant data(int column, int role = Qt::DisplayRole) const;
    bool setData(int column, const QVariant &value, int role = Qt::DisplayRole);
    inline QString hash() const { return m_hash; }
    QED2KHandle handle() const { return m_handle; }
    void setHandle(QED2KHandle h) { m_handle = h; }

private:
    State state() const;

private:
    QED2KHandle m_handle;
    QDateTime m_addedTime;
    QDateTime m_seedTime;
    QString m_hash;
};

#endif
