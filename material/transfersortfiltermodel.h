#ifndef TRANSFERSORTFILTERMODEL_H
#define TRANSFERSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class TransferSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TransferSortFilterModel(QObject* parent = 0);
    virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
};

#endif // TRANSFERSORTFILTERMODEL_H
