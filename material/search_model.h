#ifndef __SEARCHMODEL_H
#define __SEARCHMODEL_H

#include <QAbstractListModel>
#include <QIcon>
#include "qed2ksession.h"

class SearchModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum DisplayColumns {
        DC_NAME = Qt::UserRole + 1,
        DC_FILESIZE,
        DC_FILESIZE_NUM,
        DC_SOURCES,
        DC_SOURCES_NUM,
        DC_TYPE,
        DC_HASH,
        DC_MEDIA_BITRATE,
        DC_MEDIA_LENGTH,
        DC_MEDIA_CODEC,
        DC_PREVIEWABLE,
        DC_ORIGIN,
        DC_END
    };

    explicit SearchModel(QObject *parent = 0);
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    const QED2KSearchResultEntry& at(const QModelIndex& indx) const;
    QED2KSearchResultEntry& at(const QModelIndex& indx);

    void setSizeType(misc::SizeType);

    quint16 origin(const QModelIndex&) const;
    QString filename(const QModelIndex&) const;
    quint64 size(const QModelIndex&) const;
    QString hash(const QModelIndex&) const;
    quint64 complete_sources(const QModelIndex&) const;
    quint64 sources(const QModelIndex&) const;
    FileType type(const QModelIndex&) const;
    quint64 media_length(const QModelIndex&) const;
    quint64 media_bitrate(const QModelIndex&) const;
    QString media_codec(const QModelIndex&) const;
    QString getHash(int row) const;

    Q_INVOKABLE void clean();
    void appendData(const QList<QED2KSearchResultEntry>&);
    Q_PROPERTY(bool IsEmpty READ IsEmpty NOTIFY IsEmptyChanged)
    bool IsEmpty() const;
private:
    misc::SizeType m_st;
    QList<QED2KSearchResultEntry> search_results;
    bool m_moreResults;    
signals:
    void countChanged(int);
    void IsEmptyChanged(bool);
public slots:
    void on_searchResult(const libed2k::net_identifier& np, const QString& hash,
                      const QList<QED2KSearchResultEntry>& vRes, bool bMoreResult);
};

#endif // __SEARCHMODEL_H
