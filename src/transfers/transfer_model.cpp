#include "transfer_model.h"
#include "preferences.h"
#include "qtlibed2k/qed2ksession.h"

#include <QDirIterator>
#include <QFileInfo>

TransferModel::TransferModel(QObject *parent) : QAbstractListModel(parent),
    m_refreshInterval(3000) {

    // Listen for Transfer changes
    connect(Session::instance(), SIGNAL(transferAdded(QED2KHandle)), SLOT(addTransfer(QED2KHandle)));
    connect(Session::instance(), SIGNAL(transferDeleted(QString)), SLOT(removeTransfer(QString)));
    connect(Session::instance(), SIGNAL(transferFinished(QED2KHandle)), SLOT(handleTransferUpdate(QED2KHandle)));
    connect(Session::instance(), SIGNAL(transferResumed(QED2KHandle)), SLOT(handleTransferUpdate(QED2KHandle)));
    connect(Session::instance(), SIGNAL(transferRestored(QED2KHandle)), SLOT(addTransfer(QED2KHandle)));
    connect(Session::instance(), SIGNAL(transferPaused(QED2KHandle)), SLOT(handleTransferUpdate(QED2KHandle)));
    connect(Session::instance(), SIGNAL(transferShared(QED2KHandle)), SLOT(addTransfer(QED2KHandle)));
    connect(Session::instance(), SIGNAL(fileError(QED2KHandle, QString)),
            SLOT(handleTransferUpdate(QED2KHandle)));
    connect(Session::instance(), SIGNAL(transferParametersReady(libed2k::add_transfer_params,libed2k::error_code)),
            SLOT(addTransferParameters(libed2k::add_transfer_params,libed2k::error_code)));
}

TransferModel::~TransferModel() {
  qDebug() << Q_FUNC_INFO << "ENTER";
  qDeleteAll(m_transfers);
  m_transfers.clear();
  qDebug() << Q_FUNC_INFO << "EXIT";
}

QVariant TransferModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch(section) {
                case TransferModelItem::TM_NAME: return tr("Name", "i.e: Transfer name");
                case TransferModelItem::TM_SIZE: return tr("Size", "i.e: Transfer size");
                case TransferModelItem::TM_TYPE: return tr("Type", "i.e. file type");
                case TransferModelItem::TM_PROGRESS: return tr("Done", "% Done");
                case TransferModelItem::TM_STATUS: return tr("Status", "Transfer status (e.g. downloading, seeding, paused)");
                case TransferModelItem::TM_SEEDS: return tr("Seeds", "i.e. full sources (often untranslated)");
                case TransferModelItem::TM_PEERS: return tr("Peers", "i.e. partial sources (often untranslated)");
                case TransferModelItem::TM_DLSPEED: return tr("Down Speed", "i.e: Download speed");
                case TransferModelItem::TM_UPSPEED: return tr("Up Speed", "i.e: Upload speed");
                case TransferModelItem::TM_HASH: return tr("Hash", "i.e transfer hash");
                case TransferModelItem::TM_RATIO: return tr("Ratio", "Share ratio");
                case TransferModelItem::TM_ADD_DATE: return tr("Added On", "Transfer was added to transfer list on 01/01/2010 08:00");
                case TransferModelItem::TM_AMOUNT_DOWNLOADED: return tr("Amount downloaded", "Amount of data downloaded (e.g. in MB)");
                case TransferModelItem::TM_AMOUNT_LEFT: return tr("Amount left", "Amount of data left to download (e.g. in MB)");
                case TransferModelItem::TM_TIME_ELAPSED: return tr("Time Active", "Time (duration) the Transfer is active (not paused)");
            default:
            return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole) {
        switch(section) {
            case TransferModelItem::TM_SIZE:
            case TransferModelItem::TM_SEEDS:
            case TransferModelItem::TM_PEERS:
            case TransferModelItem::TM_DLSPEED:
            case TransferModelItem::TM_UPSPEED:
            case TransferModelItem::TM_RATIO:
            case TransferModelItem::TM_AMOUNT_DOWNLOADED:
            case TransferModelItem::TM_AMOUNT_LEFT:
                return Qt::AlignRight;
            case TransferModelItem::TM_PROGRESS:
                return Qt::AlignHCenter;
            default:
                return Qt::AlignLeft;
            }
        }
    }

    return QVariant();
}

QVariant TransferModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    try {
        if (index.row() >= 0 && index.row() < rowCount() && index.column() >= 0 && index.column() < columnCount())
            return m_transfers[index.row()]->data(index.column(), role);
    } catch(libed2k::libed2k_exception&) {}
    return QVariant();
}

bool TransferModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    qDebug() << Q_FUNC_INFO << value;
    if (!index.isValid() || role != Qt::DisplayRole) return false;

    try {
        if (index.row() >= 0 && index.row() < rowCount() && index.column() >= 0 && index.column() < columnCount()) {
            bool change = m_transfers[index.row()]->setData(index.column(), value, role);
            if (change) notifyTransferChanged(index.row());
            return change;
        }
    } catch(libed2k::libed2k_exception&) {}
    return false;
}

int TransferModel::transferRow(const QString &hash) const {
    QList<TransferModelItem*>::const_iterator it;
    int row = 0;
    for (it = m_transfers.constBegin(); it != m_transfers.constEnd(); it++) {
        if ((*it)->hash() == hash) return row;
        ++row;
    }

    return -1;
}

void TransferModel::addTransfer(const QED2KHandle& h) {
    qDebug() << Q_FUNC_INFO;

    if (h.state() == QED2KHandle::checking_resume_data) {
        // we don't know yet whether this transfer finished or not
        qDebug() << "add unchecked transfer " << h.hash();
        m_uncheckedTransfers << h;
        return;
    }

    if (transferRow(h.hash()) == -1) {
        qDebug() << "add transfer to list " << h.hash();
        beginInsertTransfer(m_transfers.size());
        TransferModelItem *item = new TransferModelItem(h);
        m_transfers << item;
        emit transferAdded(item);
        endInsertTransfer();
    }
}

void TransferModel::removeTransfer(const QString &hash) {
    const int row = transferRow(hash);
    qDebug() << Q_FUNC_INFO << hash << row;
    if (row >= 0) {
        beginRemoveTransfer(row);
        m_transfers.removeAt(row);
        endRemoveTransfer();
    }
}

void TransferModel::beginInsertTransfer(int row) {
    beginInsertRows(QModelIndex(), row, row);
}

void TransferModel::endInsertTransfer() {
    endInsertRows();
}

void TransferModel::beginRemoveTransfer(int row) {
    beginRemoveRows(QModelIndex(), row, row);
}

void TransferModel::endRemoveTransfer() {
    endRemoveRows();
}

void TransferModel::processUncheckedTransfers() {
    for(QList<QED2KHandle>::iterator i = m_uncheckedTransfers.begin();
        i != m_uncheckedTransfers.end();) {
        QED2KHandle::TransferState state = i->state();

        if (state != QED2KHandle::checking_resume_data) {
            // now we know whether transfer finished or not
            // do not show finished transfers
            if (state != QED2KHandle::seeding) addTransfer(*i);
            i = m_uncheckedTransfers.erase(i);
        }
        else
            ++i;
    }
}

void TransferModel::handleTransferUpdate(const QED2KHandle& h) {
    const int row = transferRow(h.hash());
    if (row != -1) {
        if (!m_transfers.at(row)->handle().is_valid()) m_transfers[row]->setHandle(h);
        notifyTransferChanged(row);
    }
}

void TransferModel::notifyTransferChanged(int row) {
    emit dataChanged(index(row, 0), index(row, columnCount()-1));
}

void TransferModel::setRefreshInterval(int refreshInterval) {
    if (m_refreshInterval != refreshInterval) {
        m_refreshInterval = refreshInterval;
        m_refreshTimer.stop();
        m_refreshTimer.start(m_refreshInterval);
    }
}

void TransferModel::forceModelRefresh() {
    processUncheckedTransfers();
    emit dataChanged(index(0, 0), index(rowCount()-1, columnCount()-1));
}

Qt::ItemFlags TransferModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return 0;
    // Explicitely mark as editable
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

QString TransferModel::transferHash(int row) const {
  if (row >= 0 && row < rowCount()) return m_transfers.at(row)->hash();
  return QString();
}

void TransferModel::handleTransferAboutToBeRemoved(const QED2KHandle &h, bool) {
  const int row = transferRow(h.hash());
  if (row >= 0)
      emit transferAboutToBeRemoved(m_transfers.at(row));
}
