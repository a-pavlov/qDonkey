#include "transferdetailsmodel.h"
#include "transfermodel_item.h"
#include "transfer_model.h"

TransferDetailsModel::TransferDetailsModel(QObject* parent /*=0*/): QSortFilterProxyModel(parent)
{

}

bool TransferDetailsModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const {
    return hash == dynamic_cast<TransferModel*>(sourceModel())->transferHash(source_row);
}

void TransferDetailsModel::setHash(const QString& h) {
    hash = h;
    invalidateFilter();
}
