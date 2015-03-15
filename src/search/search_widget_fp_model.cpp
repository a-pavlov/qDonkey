#include "search_widget_fp_model.h"
#include "search_model.h"
#include "qtlibed2k/qed2ksession.h"

SWSortFilterProxyModel::SWSortFilterProxyModel(QObject* parent):
    QSortFilterProxyModel(parent), m_showOwn(true)
{
}

bool SWSortFilterProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const {
    return QSortFilterProxyModel::lessThan(left, right);
}

void SWSortFilterProxyModel::showOwn(bool f)
{
    m_showOwn = f;
    invalidateFilter();
}

bool SWSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    if (!m_showOwn) {
        QString hash = sourceModel()->index(source_row, SearchModel::DC_HASH, source_parent).data().toString();
        return Session::instance()->getTransfer(hash).is_valid();
    }
    return true;
}
