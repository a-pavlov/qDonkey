#include "transferlist_delegate.h"
#include <QModelIndex>
#include <QByteArray>
#include <QStyleOptionViewItem>
#include <QStyleOptionViewItemV2>
#include <QApplication>
#include <QPainter>
#include <QDateTime>
#include "misc.h"
#include "transfermodel_item.h"
#include "qtlibed2k/qed2ksession.h"

#ifdef Q_WS_WIN
  #include <QPlastiqueStyle>
#endif

TransferListDelegate::TransferListDelegate(QObject *parent) : QItemDelegate(parent) {}

TransferListDelegate::~TransferListDelegate() {}

void TransferListDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QStyleOptionViewItemV2 opt = QItemDelegate::setOptions(index, option);
    painter->save();
    switch(index.column()) {
        case TransferModelItem::TM_AMOUNT_DOWNLOADED:
        case TransferModelItem::TM_AMOUNT_LEFT:
        case TransferModelItem::TM_SIZE: {
            QItemDelegate::drawBackground(painter, opt, index);
            opt.displayAlignment = Qt::AlignRight;
            QItemDelegate::drawDisplay(painter, opt, option.rect, misc::friendlyUnit(index.data().toLongLong()));
            break;
        }
    case TransferModelItem::TM_SEEDS:
    case TransferModelItem::TM_PEERS: {
        QString display = QString::number(index.data().toLongLong());
        if (index.data(Qt::UserRole).toLongLong() > 0) {
            // Scrape was successful, we have total values
            display += " ("+QString::number(index.data(Qt::UserRole).toLongLong())+")";
        }

        QItemDelegate::drawBackground(painter, opt, index);
        opt.displayAlignment = Qt::AlignRight;
        QItemDelegate::drawDisplay(painter, opt, opt.rect, display);
        break;
        }
    case TransferModelItem::TM_STATUS: {
        const int state = index.data().toInt();
        QString display;
        switch(state) {
            case TransferModelItem::STATE_DOWNLOADING:
                display = tr("Downloading");
                break;
            case TransferModelItem::STATE_PAUSED_DL:
            case TransferModelItem::STATE_PAUSED_UP:
                display = tr("Paused");
                break;
            case TransferModelItem::STATE_QUEUED:
                display = tr("Queued", "i.e. torrent is queued");
                break;
            case TransferModelItem::STATE_SEEDING:
                display = tr("Seeding", "Torrent is complete and in upload-only mode");
                break;
            case TransferModelItem::STATE_STALLED_UP:
            case TransferModelItem::STATE_STALLED_DL:
                display = tr("Stalled", "Torrent is waiting for download to begin");
                break;
            default:
                display = "";
            }
            QItemDelegate::drawBackground(painter, opt, index);
            QItemDelegate::drawDisplay(painter, opt, opt.rect, display);
            break;
        }
    case TransferModelItem::TM_UPSPEED:
    case TransferModelItem::TM_DLSPEED:{
            QItemDelegate::drawBackground(painter, opt, index);
            const qulonglong speed = index.data().toULongLong();
            opt.displayAlignment = Qt::AlignRight;
            QItemDelegate::drawDisplay(painter, opt, opt.rect, misc::friendlyUnit(speed)+tr("/s", "/second (.i.e per second)"));
            break;
        }
    case TransferModelItem::TM_TIME_ELAPSED: {
            QItemDelegate::drawBackground(painter, opt, index);
            QString txt = misc::userFriendlyDuration(index.data().toLongLong());
            qlonglong seeding_time = index.data(Qt::UserRole).toLongLong();
            if (seeding_time > 0)
            txt += " ("+tr("Seeded for %1", "e.g. Seeded for 3m10s").arg(misc::userFriendlyDuration(seeding_time))+")";
            QItemDelegate::drawDisplay(painter, opt, opt.rect, txt);
            break;
        }
    case TransferModelItem::TM_ADD_DATE:
        QItemDelegate::drawBackground(painter, opt, index);
        QItemDelegate::drawDisplay(painter, opt, opt.rect, index.data().toDateTime().toLocalTime().toString(Qt::DefaultLocaleShortDate));
        break;
    case TransferModelItem::TM_ETA:
        QItemDelegate::drawBackground(painter, opt, index);
        QItemDelegate::drawDisplay(painter, opt, option.rect, misc::userFriendlyDuration(index.data().toLongLong()));
        break;
    case TransferModelItem::TM_RATIO: {
            QItemDelegate::drawBackground(painter, opt, index);
            opt.displayAlignment = Qt::AlignRight;
            const qreal ratio = index.data().toDouble();
            if (ratio > Session::MAX_RATIO)
                QItemDelegate::drawDisplay(painter, opt, opt.rect, QString::fromUtf8("∞"));
            else
                QItemDelegate::drawDisplay(painter, opt, opt.rect, QString::number(ratio, 'f', 2));
            break;
        }
    case TransferModelItem::TM_PROGRESS: {
            QStyleOptionProgressBarV2 newopt;
            qreal progress = index.data().toDouble()*100.;
            // We don't want to display 100% unless
            // the torrent is really complete
            if (progress > 99.94 && progress < 100.) progress = 99.9;
            newopt.rect = opt.rect;
            newopt.text = QString::number(progress, 'f', 1)+"%";
            newopt.progress = (int)progress;
            newopt.maximum = 100;
            newopt.minimum = 0;
            newopt.state |= QStyle::State_Enabled;
            newopt.textVisible = true;
    #ifndef Q_WS_WIN
            QApplication::style()->drawControl(QStyle::CE_ProgressBar, &newopt, painter);
    #else
            // XXX: To avoid having the progress text on the right of the bar
            QPlastiqueStyle st;
            st.drawControl(QStyle::CE_ProgressBar, &newopt, painter, 0);
    #endif
            break;
        }
    default:
        QItemDelegate::paint(painter, option, index);
    }

    painter->restore();
}

QWidget* TransferListDelegate::createEditor(QWidget*, const QStyleOptionViewItem &, const QModelIndex &) const {
  // No editor here
  return 0;
}
