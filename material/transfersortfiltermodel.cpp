#include "transfersortfiltermodel.h"
#include "transfermodel_item.h"
#include "transfer_model.h"
#include "misc.h"


TransferSortFilterModel::TransferSortFilterModel(QObject* parent): QSortFilterProxyModel(parent)
{

}

bool TransferSortFilterModel::lessThan(const QModelIndex& left, const QModelIndex& right) const {
    // stupid hack :(
    TransferModel* tm = dynamic_cast<TransferModel*>(sourceModel());
    Q_ASSERT(tm != NULL);
    return misc::naturalCompare(tm->data(left).toString(), tm->data(right).toString(), Qt::CaseSensitive) < 0;
}

