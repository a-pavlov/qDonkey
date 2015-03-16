#ifndef __SEARCHMODEL_H
#define __SEARCHMODEL_H

#include <QAbstractTableModel>
#include <QIcon>
#include "qed2ksession.h"

class SearchModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Roles {
        SortRole = Qt::UserRole + 1
    };

    enum DisplayColumns {
        DC_NAME = 0,
        DC_FILESIZE,
        DC_SOURCES,
        DC_TYPE,
        DC_HASH,
        DC_MEDIA_BITRATE,
        DC_MEDIA_LENGTH,
        DC_MEDIA_CODEC,
        DC_END
    };

    explicit SearchModel(QObject *parent = 0);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    const QED2KSearchResultEntry& at(const QModelIndex& indx) const;
    QED2KSearchResultEntry& at(const QModelIndex& indx);

    QString filename(const QModelIndex&) const;
    quint64 size(const QModelIndex&) const;
    QString hash(const QModelIndex&) const;
    quint64 complete_sources(const QModelIndex&) const;
    quint64 sources(const QModelIndex&) const;
    FileType type(const QModelIndex&) const;
    quint64 media_length(const QModelIndex&) const;
    quint64 media_bitrate(const QModelIndex&) const;
    QString media_codec(const QModelIndex&) const;

    void clean();
    void removeIndex(int);
    void resetToIndex(int);
    int addDataTo(const QList<QED2KSearchResultEntry>&, int);
    void appendData(const QList<QED2KSearchResultEntry>&);
    void save();
    void load();
    int levels() const { return search_results.size(); }
private:                
    int currentIndex;
    QList<QList<QED2KSearchResultEntry> > search_results;
    QIcon  iconAny;
    QIcon  iconAudio;
    QIcon  iconVideo;
    QIcon  iconImage;
    QIcon  iconCD;
    QIcon  iconArchive;
    QIcon  iconProgram;
    QIcon  iconDoc;
    QIcon  iconEM;
signals:
    void countChanged(int);
};

#endif // __SEARCHMODEL_H
