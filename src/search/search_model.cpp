#include "search_model.h"
#include "misc.h"
#include "res.h"

SearchModel::SearchModel(QObject *parent) :
    QAbstractTableModel(parent)
{}

int SearchModel::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const { return search_result.size(); }
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
                case FT_ANY: return QIcon(res::itemAny());
                case FT_AUDIO: return QIcon(res::itemMusic());
                case FT_VIDEO: return QIcon(res::itemVideo());
                case FT_IMAGE: return QIcon(res::itemPicture());
                case FT_ARCHIVE: return QIcon(res::itemArchive());
                case FT_PROGRAM: return QIcon(res::itemProgram());
                case FT_DOCUMENT: return QIcon(res::itemDocument());
                case FT_EMULECOLLECTION: return QIcon(res::itemEmuleCollection());
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
    if (rowCount() == 0)
        return;

    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    search_result.clear();
    endRemoveRows();
}

void SearchModel::addData(libed2k::net_identifier np, const QString& hash, const std::vector<QED2KSearchResultEntry>& entries, bool moreRes)
{
    if (entries.empty())
        return;

    Q_UNUSED(np);
    Q_UNUSED(hash);
    Q_UNUSED(moreRes);

    beginInsertRows(QModelIndex(), rowCount(), rowCount() + entries.size() - 1);

    for(std::vector<QED2KSearchResultEntry>::const_iterator itr = entries.begin(); itr != entries.end(); ++itr)
        search_result.append(*itr);

    endInsertRows();
}

const QED2KSearchResultEntry& SearchModel::at(const QModelIndex& indx) const {
    Q_ASSERT(indx.row() < rowCount());
    return search_result.at(indx.row());
}

QED2KSearchResultEntry& SearchModel::at(const QModelIndex& indx)
{
    Q_ASSERT(indx.row() < rowCount());
    return search_result[indx.row()];
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
