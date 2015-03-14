#include "search_widget_fp_model.h"
#include "search_widget_delegate.h"

SWSortFilterProxyModel::SWSortFilterProxyModel(QObject* parent):
    QSortFilterProxyModel(parent), m_showOwn(true)
{
}

bool SWSortFilterProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
    QString name1 = sourceModel()->data(
        sourceModel()->index(left.row(), SWDelegate::SW_NAME)).toString();
    QString name2 = sourceModel()->data(
        sourceModel()->index(right.row(), SWDelegate::SW_NAME)).toString();
    bool torr1 = misc::isTorrentLink(name1);
    bool torr2 = misc::isTorrentLink(name2);

    if (torr1 && !torr2) return sortOrder() == Qt::DescendingOrder;
    else if (!torr1 && torr2) return sortOrder() == Qt::AscendingOrder;

    return QSortFilterProxyModel::lessThan(left, right);
}

void SWSortFilterProxyModel::showOwn(bool f)
{
    m_showOwn = f;
    invalidateFilter();
}

bool SWSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    if (!m_showOwn) {
        QString hash = sourceModel()->index(
            source_row, SWDelegate::SW_ID, source_parent).data().toString();
        //return !inSession(hash);
        return false;
    }
    return true;
}
