#ifndef __SW_FP_MODEL__
#define __SW_FP_MODEL__

#include <QSortFilterProxyModel>

class SWSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    SWSortFilterProxyModel(QObject* parent = 0);
    virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
    void showOwn(bool f);
    Q_INVOKABLE void sortData();
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;

private:
    bool m_showOwn;
};

#endif //__SW_FP_MODEL__
