#ifndef TRANSFERDETAILSMODEL_H
#define TRANSFERDETAILSMODEL_H

#include <QSortFilterProxyModel>

class TransferDetailsModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    TransferDetailsModel(QObject* parent = 0);
    Q_INVOKABLE void setHash(const QString&);
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
    QString hash;
};

#endif // TRANSFERDETAILSMODEL_H
