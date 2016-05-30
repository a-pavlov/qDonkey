#include "search_model.h"
#include "misc.h"

SearchModel::SearchModel(QObject *parent) :
    QAbstractListModel(parent), m_st(misc::ST_DEFAULT), m_moreResults(false) {
}

QHash<int, QByteArray> SearchModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DC_NAME]  = "name";
    roles[DC_FILESIZE] = "filesize";
    roles[DC_FILESIZE_NUM] = "filesize_num";
    roles[DC_SOURCES] = "sources";
    roles[DC_SOURCES_NUM]="sources_num";
    roles[DC_TYPE] = "type";
    roles[DC_HASH] = "hash";
    roles[DC_MEDIA_BITRATE] = "bitrate";
    roles[DC_MEDIA_LENGTH] = "media_length";
    roles[DC_MEDIA_CODEC] = "media_codec";
    roles[DC_PREVIEWABLE] = "previewable";
    return roles;
}

int SearchModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const {
    return search_results.size();
}
int SearchModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const { return DC_END; }

QVariant SearchModel::data(const QModelIndex& index, int role) const {
    QVariant result;

    if (!index.isValid()) return result;

    switch(role) {
        case DC_NAME:        return filename(index);
        case DC_FILESIZE:    return misc::friendlyUnit(size(index), m_st);
        case DC_FILESIZE_NUM:return size(index);
        case DC_SOURCES: {
            quint64 nSources = sources(index);
            quint64 nCompleteSources = complete_sources(index);

            QString strSrc = (nSources > 0) ?
            (QString::number(100 * nCompleteSources / nSources)) : "0";

            strSrc += "%(";
            strSrc += QString::number(nSources);
            strSrc += ")";
            return strSrc;
        }
        case DC_SOURCES_NUM:return sources(index);
        case DC_TYPE:       return toString(type(index));
        case DC_HASH:       return hash(index);
        case DC_MEDIA_BITRATE: {
            quint64 nMediaBitrate = media_bitrate(index);
            QString bitRate = QString::number(nMediaBitrate);

            if (bitRate.length() && nMediaBitrate > 0) {
                bitRate += tr(" kBit/s");
            }

            return bitRate;
        }
        case DC_MEDIA_LENGTH: return media_length(index) > 0 ? misc::userFriendlyDuration(media_length(index), 1) : "";
        case DC_MEDIA_CODEC:  return media_codec(index);
        case DC_PREVIEWABLE: return misc::isPreviewable(misc::file_extension(filename(index)));
        case DC_END:
            Q_ASSERT(false);
            break;
        default:
            break;
        }
    /*

    switch(role)
    {
        case NativeRole: {
            switch(index.column()) {
                case DC_NAME: return filename(index);
                case DC_FILESIZE: return size(index);
                case DC_SOURCES: return sources(index);
                case DC_TYPE: return type(index);
                case DC_HASH: return hash(index);
                case DC_MEDIA_BITRATE: return media_bitrate(index);
                case DC_MEDIA_LENGTH: return media_length(index);
                default: break;
            }
            break;
        }
        case Qt::DisplayRole:  {
            switch(index.column()) {
                case DC_NAME:        return filename(index);
                case DC_FILESIZE:    return misc::friendlyUnit(size(index), m_st);
                case DC_SOURCES:
                {
                    quint64 nSources = sources(index);
                    quint64 nCompleteSources = complete_sources(index);

                    QString strSrc = (nSources > 0) ?
                        (QString::number(100 * nCompleteSources / nSources)) : "0";

                    strSrc += "%(";
                    strSrc += QString::number(nSources);
                    strSrc += ")";
                    return strSrc;
                }
                case DC_TYPE:       return toString(type(index));
                case DC_HASH:       return hash(index);
                case DC_MEDIA_BITRATE: {

                    quint64 nMediaBitrate = media_bitrate(index);
                    QString bitRate = QString::number(nMediaBitrate);

                    if (bitRate.length() && nMediaBitrate > 0) {
                        bitRate += tr(" kBit/s");
                    }

                    return bitRate;
                }
                case DC_MEDIA_LENGTH: return media_length(index) > 0 ? misc::userFriendlyDuration(media_length(index), 1) : "";
                case DC_MEDIA_CODEC:  return media_codec(index);
                case DC_END:
                    Q_ASSERT(false);
                    break;
                default:
                    break;
            }
            break;
        }
        case Qt::DecorationRole: {
            if (index.column() == DC_NAME) {
                switch(type(index)) {
                case FT_ANY: return iconAny;
                case FT_AUDIO: return iconAudio;
                case FT_VIDEO: return iconVideo;
                case FT_IMAGE: return iconImage;
                case FT_CDIMAGE: return iconCD;
                case FT_ARCHIVE: return iconArchive;
                case FT_PROGRAM: return iconProgram;
                case FT_DOCUMENT: return iconDoc;
                case FT_EMULECOLLECTION: return iconEM;
                default:
                    break;
                }
            }
            break;
        }
        case SortRole:{
            switch(index.column()) {
            case DC_NAME:       return filename(index);
            case DC_FILESIZE:   return size(index);
            case DC_SOURCES:    return sources(index);
            case DC_TYPE:  {
                return toString(type(index));
            }
            case DC_HASH:           return hash(index);
            case DC_MEDIA_BITRATE:  return media_bitrate(index);
            case DC_MEDIA_LENGTH:   return media_length(index);
            case DC_MEDIA_CODEC:    return media_codec(index);
            case DC_END:
                Q_ASSERT(false);
                break;
            default:
                break;
            }
            break;
        }
        case Qt::FontRole: {
            break;
        }
        case Qt::ForegroundRole: {
            QColor color(Qt::black);
            QString h = hash(index);
            Q_ASSERT(misc::isMD4Hash(h));
            if (Session::instance()->getTransfer(h).is_valid()) color = Qt::red;
            return color;
        }
        default:
            break;
    }
*/
    return result;
}

void SearchModel::clean() {
    beginResetModel();
    search_results.clear();
    endResetModel();
    emit IsEmptyChanged(search_results.isEmpty());
}


void SearchModel::appendData(const QList<QED2KSearchResultEntry>& entries) {
    if (entries.isEmpty()) return;
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + entries.size() - 1);
    search_results.append(entries);
    qDebug() << "results count " << search_results.size();
    endInsertRows();
    emit IsEmptyChanged(search_results.isEmpty());
}

bool SearchModel::IsEmpty() const
{
    return search_results.isEmpty();
}

const QED2KSearchResultEntry& SearchModel::at(const QModelIndex& indx) const {
    Q_ASSERT(indx.row() < rowCount());
    return search_results.at(indx.row());
}

QED2KSearchResultEntry& SearchModel::at(const QModelIndex& indx)
{
    Q_ASSERT(indx.row() < rowCount());
    return search_results[indx.row()];
}

void SearchModel::setSizeType(misc::SizeType st) {
    m_st = st;
    dataChanged(index(0, DC_FILESIZE), index(rowCount(), DC_FILESIZE));
}

QString SearchModel::filename(const QModelIndex& indx) const {
    return at(indx).m_strFilename;
}

quint64 SearchModel::size(const QModelIndex& indx) const {
    return at(indx).m_nFilesize;
}

QString SearchModel::hash(const QModelIndex& indx) const {
    return at(indx).m_hFile;
}

FileType SearchModel::type(const QModelIndex& indx) const {
    return at(indx).m_type;
}

quint64 SearchModel::complete_sources(const QModelIndex& indx) const {
    return at(indx).m_nCompleteSources;
}

quint64 SearchModel::sources(const QModelIndex& indx) const {
    return at(indx).m_nSources;
}

quint64 SearchModel::media_bitrate(const QModelIndex& indx) const {
    return at(indx).m_nMediaBitrate;
}

quint64 SearchModel::media_length(const QModelIndex& indx) const {
    return at(indx).m_nMediaLength;
}

QString SearchModel::media_codec(const QModelIndex& indx) const {
    return at(indx).m_strMediaCodec;
}

QString SearchModel::getHash(int row) const {
    Q_ASSERT(row < rowCount());
    return search_results[row].m_hFile;
}

void SearchModel::on_searchResult(const libed2k::net_identifier& np, const QString& hash,
    const QList<QED2KSearchResultEntry>& vRes, bool bMoreResult) {
    Q_UNUSED(np);
    Q_UNUSED(hash);
    m_moreResults = bMoreResult;
    appendData(vRes);
}

