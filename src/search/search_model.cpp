#include "search_model.h"
#include "misc.h"
#include "res.h"

SearchModel::SearchModel(QObject *parent) :
    QAbstractTableModel(parent), currentIndex(-1) {
    iconAny = QIcon(res::itemAny());
    iconAudio = QIcon(res::itemMusic());
    iconVideo = QIcon(res::itemVideo());
    iconImage = QIcon(res::itemPicture());
    iconCD = QIcon(res::itemCDImage());
    iconArchive = QIcon(res::itemArchive());
    iconProgram = QIcon(res::itemProgram());
    iconDoc = QIcon(res::itemDocument());
    iconEM = QIcon(res::itemEmuleCollection());
}

int SearchModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const {
    if (currentIndex != -1) return search_results.at(currentIndex).size();
    return 0;
}
int SearchModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const { return DC_END; }

QVariant SearchModel::data(const QModelIndex& index, int role) const {
    QVariant result;

    if (!index.isValid())
        return result;

    switch(role)
    {
        case Qt::DisplayRole:
        {
            switch(index.column()) {
                case DC_NAME:        return filename(index);
                case DC_FILESIZE:    return misc::friendlyUnit(size(index), misc::ST_DEFAULT);
                case DC_SOURCES:
                {
                    quint64 nSources = sources(index);
                    quint64 nCompleteSources = complete_sources(index);

                    QString strSrc = (nSources > 0) ?
                        (QString::number(100 * nCompleteSources / nSources)) : "0";

                    strSrc += "%(";
                    strSrc += QString::number(nCompleteSources);
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
        case Qt::DecorationRole:
        {
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
                case FT_EMULECOLLECTION: iconEM;
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
                //QString strHash = hash(index);
                //if (misc::isMD4Hash(strHash) && Session::instance()->getTransfer(strHash).is_valid())
                //        color = Qt::red;
            break;
        }
        default:
            break;
    }

    return result;
}

QVariant SearchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QAbstractTableModel::headerData(section, orientation, role);

    switch(section) {
        case DC_NAME:        return tr("Filename");
        case DC_FILESIZE:    return tr("Size");
        case DC_SOURCES:     return tr("Sources");
        case DC_TYPE:        return tr("Type");
        case DC_HASH:        return tr("ID");
        case DC_MEDIA_BITRATE: return tr("Bitrate");
        case DC_MEDIA_LENGTH: return tr("Duration");
        case DC_MEDIA_CODEC: return tr("Codec");
        case DC_END: Q_ASSERT(false);
            break;
        default:
            break;
    }

    return QVariant();
}

void SearchModel::clean() {
    search_results.clear();
    currentIndex = -1;
    emit countChanged(search_results.size());
}

void SearchModel::removeIndex(int index) {
    Q_ASSERT(index < search_results.size());
    search_results.removeAt(index);
    currentIndex = search_results.size() - 1;
    emit countChanged(search_results.size());
}

int SearchModel::addDataTo(const QList<QED2KSearchResultEntry>& entries, int index)
{
    Q_ASSERT(index < search_results.size());

    if (entries.empty())
        return 0;

    beginInsertRows(QModelIndex(), rowCount(), rowCount() + entries.size() - 1);
    search_results[index].append(entries);
    endInsertRows();
    return search_results.at(index).size();
}

void SearchModel::resetToIndex(int index) {
    Q_ASSERT(index < search_results.size());
    beginResetModel();
    currentIndex = index;
    endResetModel();
}

void SearchModel::appendData(const QList<QED2KSearchResultEntry>& entries) {
    search_results.append(entries);
    emit countChanged(search_results.size());
}

const QED2KSearchResultEntry& SearchModel::at(const QModelIndex& indx) const {
    Q_ASSERT(indx.row() < rowCount());
    return search_results.at(currentIndex).at(indx.row());
}

QED2KSearchResultEntry& SearchModel::at(const QModelIndex& indx)
{
    Q_ASSERT(indx.row() < rowCount());
    return search_results[currentIndex][indx.row()];
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
