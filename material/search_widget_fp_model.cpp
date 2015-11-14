#include "search_widget_fp_model.h"
#include "search_model.h"
#include "qtlibed2k/qed2ksession.h"
#include "misc.h"

SWSortFilterProxyModel::SWSortFilterProxyModel(QObject* parent):
    QSortFilterProxyModel(parent), m_showOwn(false)
{
    //setSortRole(SearchModel::SortRole);
}

bool SWSortFilterProxyModel::lessThan(const QModelIndex& left, const QModelIndex& right) const {
    if (left.column() == SearchModel::DC_NAME) {
            return (misc::naturalCompare(sourceModel()->data(left, Qt::DisplayRole).toString(),
                                   sourceModel()->data(right, Qt::DisplayRole).toString(),
                                   Qt::CaseSensitive) < 0);
    }

    // HACK to source SearchModel in QML
    if (left.column() == SearchModel::DC_SOURCES) {
        SearchModel* sm = dynamic_cast<SearchModel*>(sourceModel());
        Q_ASSERT(sm != NULL);
        return sm->sources(left) < sm->sources(right);
    }

    return QSortFilterProxyModel::lessThan(left, right);
}

void SWSortFilterProxyModel::showOwn(bool f)
{
    if (m_showOwn != f) {
        m_showOwn = f;
        invalidateFilter();
    }
}

bool SWSortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    if (!m_showOwn) {
        Q_UNUSED(source_parent);
        QString hash = ((SearchModel*)sourceModel())->getHash(source_row);
        return !Session::instance()->getTransfer(hash).is_valid();
    }
    return true;
}

void SWSortFilterProxyModel::sortData() {
    QSortFilterProxyModel::sort(SearchModel::DC_SOURCES, Qt::DescendingOrder);
}
