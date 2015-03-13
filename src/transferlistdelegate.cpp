#include "transferlistdelegate.h"
#include <QModelIndex>
#include <QByteArray>
#include <QStyleOptionViewItem>
#include <QStyleOptionViewItemV2>
#include <QApplication>
#include <QPainter>
#include <QDateTime>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QAbstractItemView>
#include "misc.h"
#include "torrentmodel.h"
#include "transport/session.h"
#include "transport/transfer.h"
#include "utils.h"

#ifdef Q_WS_WIN
  #include <QPlastiqueStyle>
#endif

TransferListDelegate::TransferListDelegate(QObject *parent) : QItemDelegate(parent)
{}

TransferListDelegate::~TransferListDelegate()
{}

void TransferListDelegate::drawProgressBar(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
    qreal progress = index.data().toDouble();
    
    // prevent drawing out of bounds of progress bar
    if (progress < 0)
        progress = 0;
    else if (progress > 1.0)
        progress = 1.0;

    qreal vertCenter = option.rect.height() / 2.0;
    qreal innerRadius = 5.0;
    qreal outerRadius = innerRadius + 1.0;
    qreal hPadding = 4.0;

    // NOTE!!! Adding 0.5 to X and Y of an original QRect did the trick:
    // shapes become more attractive

    QRectF rect(option.rect.x() + hPadding + 0.5,
               option.rect.y() + vertCenter - outerRadius + 0.5,
               option.rect.width() - hPadding * 2.0,
               outerRadius * 2.0);

    QRectF rect2(rect.left() + 1.0,
                rect.top() + 1.0,
                (rect.width() - 2.0) * progress,
                rect.height() - 2.0);

    drawBackground(painter, option, index);

    // enable antialiasing to have good looking shapes
    painter->setRenderHint(QPainter::Antialiasing);

    // set colors according to the status and progress
    const TorrentModelItem::State state = index.model()->index(index.row(), TorrentModelItem::TR_STATUS).data().value<TorrentModelItem::State>();

    switch(state)
    {
        case TorrentModelItem::States::DOWNLOADING:
        {
            // draw border
            painter->setPen(QColor("#719ABA"));
            painter->setBrush(Qt::white);
            painter->drawRoundedRect(rect, outerRadius, outerRadius);

            // draw progress
            painter->setPen(Qt::transparent);
            painter->setBrush(QColor("#9ED2F9"));
            painter->drawRoundedRect(rect2, innerRadius, innerRadius);
            break;
        }
        case TorrentModelItem::States::SEEDING:
        case TorrentModelItem::States::STALLED_UP:
        {
            // draw border
            painter->setPen(QColor("#5FBD5D"));
            painter->setBrush(Qt::white);
            painter->drawRoundedRect(rect, outerRadius, outerRadius);

            // draw progress
            painter->setPen(Qt::transparent);
            painter->setBrush(QColor("#76E972"));
            painter->drawRoundedRect(rect2, innerRadius, innerRadius);
            break;
        }
        default:
        {
            // draw border
            painter->setPen(QColor("#777777"));
            painter->setBrush(Qt::white);
            painter->drawRoundedRect(rect, outerRadius, outerRadius);

            // draw progress
            painter->setPen(Qt::transparent);
            painter->setBrush(QColor("#AAAAAA"));
            painter->drawRoundedRect(rect2, innerRadius, innerRadius);
            break;
        }
    }

    QRectF textRect(rect.left(), rect.top() + 1, rect.width(), rect.height());
    QFont font = painter->font();
    font.setFamily("Arial");
    font.setPointSize(8);
    painter->setFont(font);
    painter->setPen(Qt::black);
    painter->drawText(textRect, QString("%1%").arg(static_cast<int>(progress * 100.0)), QTextOption(Qt::AlignCenter));
}

void TransferListDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QStyleOptionViewItemV2 opt = QItemDelegate::setOptions(index, option);
    painter->save();
//    painter->setRenderHint(QPainter::Antialiasing);

    switch(index.column())
    {
        case TorrentModelItem::TR_AMOUNT_DOWNLOADED:
        case TorrentModelItem::TR_AMOUNT_LEFT:
        case TorrentModelItem::TR_SIZE:
        {
            QItemDelegate::drawBackground(painter, opt, index);
            opt.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
            QItemDelegate::drawDisplay(painter, opt, opt.rect, misc::friendlyUnit(index.data().toLongLong()));
            break;
        }
        case TorrentModelItem::TR_ETA:
        {
            QItemDelegate::drawBackground(painter, opt, index);
            QItemDelegate::drawDisplay(painter, opt, opt.rect, misc::userFriendlyDuration(index.data().toLongLong()));
            break;
        }
        case TorrentModelItem::TR_SEEDS:
        case TorrentModelItem::TR_PEERS:
        {
            QString display = QString::number(index.data().toLongLong());

            if (index.data(Qt::UserRole).toLongLong() > 0)
            {
              // Scrape was successful, we have total values
              display += " ("+QString::number(index.data(Qt::UserRole).toLongLong())+")";
            }

            QItemDelegate::drawBackground(painter, opt, index);
            opt.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
            QItemDelegate::drawDisplay(painter, opt, opt.rect, display);
            break;
        }
        case TorrentModelItem::TR_STATUS:
        {
            const QString &state = TorrentModelItem::States::toString(index.data().value<TorrentModelItem::State>());
            QItemDelegate::drawBackground(painter, opt, index);
            QItemDelegate::drawDisplay(painter, opt, opt.rect, state);
            break;
        }
        case TorrentModelItem::TR_UPSPEED:
        case TorrentModelItem::TR_DLSPEED:
        {
            QItemDelegate::drawBackground(painter, opt, index);
            const qulonglong speed = index.data().toULongLong();
            opt.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
            QItemDelegate::drawDisplay(painter, opt, opt.rect, misc::friendlyUnit(speed)+tr("/s", "/second (.i.e per second)"));
            break;
        }
        case TorrentModelItem::TR_UPLIMIT:
        case TorrentModelItem::TR_DLLIMIT:
        {
            QItemDelegate::drawBackground(painter, opt, index);
            const qlonglong limit = index.data().toLongLong();
            opt.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;

            if (limit > 0)
                QItemDelegate::drawDisplay(painter, opt, opt.rect, QString::number(limit/1024., 'f', 1) + " " + tr("KiB/s", "KiB/second (.i.e per second)"));
            else
                QItemDelegate::drawDisplay(painter, opt, opt.rect, QString::fromUtf8("∞"));
            break;
        }
        case TorrentModelItem::TR_TIME_ELAPSED:
        {
            QItemDelegate::drawBackground(painter, opt, index);
            QString txt = misc::userFriendlyDuration(index.data().toLongLong());
            qlonglong seeding_time = index.data(Qt::UserRole).toLongLong();

            if (seeding_time > 0)
                txt += " ("+tr("Seeded for %1", "e.g. Seeded for 3m10s").arg(misc::userFriendlyDuration(seeding_time))+")";

            QItemDelegate::drawDisplay(painter, opt, opt.rect, txt);
            break;
        }
        case TorrentModelItem::TR_ADD_DATE:
        case TorrentModelItem::TR_SEED_DATE:
        {
            QItemDelegate::drawBackground(painter, opt, index);
            QItemDelegate::drawDisplay(painter, opt, opt.rect, index.data().toDateTime().toLocalTime().toString(Qt::DefaultLocaleShortDate));
            break;
        }
        case TorrentModelItem::TR_RATIO:
        {
            QItemDelegate::drawBackground(painter, opt, index);
            opt.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
            const qreal ratio = index.data().toDouble();

            if (ratio > Session::MAX_RATIO)
                QItemDelegate::drawDisplay(painter, opt, opt.rect, QString::fromUtf8("∞"));
            else
                QItemDelegate::drawDisplay(painter, opt, opt.rect, QString::number(ratio, 'f', 2));
            break;
        }
        case TorrentModelItem::TR_PRIORITY:
        {
            const int priority = index.data().toInt();

            if (priority >= 0)
            {
                opt.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
                QItemDelegate::paint(painter, opt, index);
            }
            else
            {
                QItemDelegate::drawBackground(painter, opt, index);
                opt.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
                QItemDelegate::drawDisplay(painter, opt, opt.rect, "*");
            }
            break;
          }
        case TorrentModelItem::TR_PROGRESS:
        {
            drawProgressBar(painter, option, index);
            break;
        }
        case TorrentModelItem::TR_TYPE:
        {
            const QString &type = Transfer::Types::toString(index.data().value<Transfer::Type>());
            QItemDelegate::drawBackground(painter, opt, index);
            opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
            QItemDelegate::drawDisplay(painter, opt, opt.rect, type);
            break;
        }
        case TorrentModelItem::TR_FILE_TYPE:
        {
            const QString &fileType = utils::toString(index.data().value<utils::FileType>());
            QItemDelegate::drawBackground(painter, opt, index);
            opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
            QItemDelegate::drawDisplay(painter, opt, opt.rect, fileType);
            break;
        }
        case TorrentModelItem::TR_NAME:
        {   
            const QString &fileName = index.data().toString();
            const QAbstractItemModel *model = index.model();
            QIcon stateIcon(TorrentModelItem::States::toIcon(model->index(index.row(), TorrentModelItem::TR_STATUS).data().value<TorrentModelItem::States::State>()));
            QIcon typeIcon(utils::toIcon(model->index(index.row(), TorrentModelItem::TR_FILE_TYPE).data().value<utils::FileType>()));
            QItemDelegate::drawBackground(painter, opt, index);
            int indent = 4;
            QRect stateRect(opt.rect.left() + indent, opt.rect.top() + 1, 20, 20);
            QRect typeRect(stateRect.right() + indent, stateRect.top(), 20, 20);
            QRect textRect(opt.rect);
            textRect.setLeft(typeRect.right() + indent);
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->drawPixmap(stateRect, stateIcon.pixmap(20, 20));
            painter->drawPixmap(typeRect, typeIcon.pixmap(20, 20));
            opt.rect.setLeft(opt.rect.left() + 20);
            opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
            QItemDelegate::drawDisplay(painter, opt, textRect, fileName);
            break;
        }
        default:
            QItemDelegate::paint(painter, opt, index);
    }

    painter->restore();
}

QWidget* TransferListDelegate::createEditor(QWidget*, const QStyleOptionViewItem &, const QModelIndex &) const
{
    // No editor here
    return 0;
}

QSize TransferListDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QSize(QItemDelegate::sizeHint(option, index).width(), 24);
}
