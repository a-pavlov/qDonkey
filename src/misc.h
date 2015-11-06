/*
 * Bittorrent Client using Qt4 and libtorrent.
 * Copyright (C) 2006  Christophe Dumez
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * In addition, as a special exception, the copyright holders give permission to
 * link this program with the OpenSSL project's "OpenSSL" library (or with
 * modified versions of it that use the same license as the "OpenSSL" library),
 * and distribute the linked executables. You must obey the GNU General Public
 * License in all respects for all of the code used other than "OpenSSL".  If you
 * modify file(s), you may extend this exception to your version of the file(s),
 * but you are not obligated to do so. If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * Contact : chris@qbittorrent.org
 */

#ifndef MISC_H
#define MISC_H

#ifndef Q_MOC_RUN
#include <libed2k/md4_hash.hpp>
#include <libed2k/error_code.hpp>
#include <libed2k/file.hpp>
#endif
#include <sstream>
#include <QString>
#include <QStringList>
#include <QThread>
#include <ctime>
#include <QPoint>
#include <QFile>
#include <QDir>
#ifndef DISABLE_GUI
#include <QIcon>
#endif

#define PRODUCT_NAME "qDonkey"
#define COMPANY_NAME "donkeyhome"

const qlonglong MAX_ETA = 8640000;

const int ed2k_max_download = 10000;
const int ed2k_max_upload   = 8000;

typedef QMap<QString, QList<QString> > shared_map;

/*  Miscellaneaous functions that can be useful */
class misc : public QObject{
  Q_OBJECT

public:
    enum SizeType 
    {
        ST_DEFAULT,
        ST_KB,
        ST_MB,
        ST_GB
    };
private:
  misc(); // Forbidden

public:
  static const QString metadataFilename;
  static QString productName() {
      return QString::fromUtf8(PRODUCT_NAME);
  }

  static QString versionString(){
      return tr("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD);
  }

  static inline QString toQString(const std::string &str) {
    return QString::fromLocal8Bit(str.c_str());
  }

  static inline QString toQString(const char* str) {
    return QString::fromLocal8Bit(str);
  }

  static inline QString toQStringU(const std::string& str) {
    return QString::fromUtf8(str.c_str());
  }

  static inline QString toQStringU(const char* str) {
      return QString::fromUtf8(str);
  }

  static inline QString toQString(const libed2k::md4_hash& hash)
  {
      return (QString::fromStdString(hash.toString()));
  }

  static inline bool isMD4Hash(const QString& s)
  {
      return (s.length() == 32);
  }

  static inline bool isSHA1Hash(const QString& s)
  {
      return (s.length() == 40);
  }


  static inline QString file_extension(const QString &filename) {
    QString extension;
    int point_index = filename.lastIndexOf(".");
    if (point_index >= 0) {
      extension = filename.mid(point_index+1);
    }
    return extension;
  }

#ifndef DISABLE_GUI
  static void shutdownComputer(bool sleep=false);
#endif

  static QString parseHtmlLinks(const QString &raw_text);

  static quint64 computePathSize(QString path);

  static QString fixFileNames(QString path);

  static QString updateLabelInSavePath(QString defaultSavePath, QString save_path, const QString &old_label, const QString &new_label);

  static bool sameFiles(const QString &path1, const QString &path2);
  static bool isUrl(const QString &s);
  static QString toValidFileSystemName(QString filename);
  static bool isValidFileSystemName(const QString& filename);

  /* Ported from Qt4 to drop dependency on QtGui */
  static QString QDesktopServicesTempInetLocation();
  static QString QDesktopServicesDataLocation();
  static QString QDesktopServicesCacheLocation();
  static QString QDesktopServicesDownloadLocation();
  /* End of Qt4 code */

#ifndef DISABLE_GUI
  // Get screen center
  static QPoint screenCenter(QWidget *win);
#endif
  static QString ED2KBackupLocation();
  static QString ED2KMetaLocation(const QString&);
  static QString ED2KCollectionLocation();
  static QString XCatalogCacheLocation();
  static QString cacheLocation();
  //static long long freeDiskSpaceOnPath(QString path);
  // return best userfriendly storage unit (B, KiB, MiB, GiB, TiB)
  // use Binary prefix standards from IEC 60027-2
  // see http://en.wikipedia.org/wiki/Kilobyte
  // value must be given in bytes
  static QString friendlyUnit(qreal val, SizeType type = ST_DEFAULT);
  static QString friendlyUnit(qreal val, bool is_speed, bool webui);
  static QString accurateDoubleToString(const double &n, const int &precision, bool localized);
  static bool isPreviewable(QString extension);
  static QString branchPath(QString file_path, bool uses_slashes=false);
  static QString fileName(QString file_path);
  // Replace ~ in path
  static QString expandPath(QString path);
  // Take a number of seconds and return an user-friendly
  // time duration like "1d 2h 10m".
  static QString userFriendlyDuration(qlonglong seconds, int type = 0);
  static QString getUserIDString();
  static QString getUserName();

  // Convert functions
  static QStringList toStringList(const QList<bool> &l);
  static QList<int> intListfromStringList(const QStringList &l);
  static QList<bool> boolListfromStringList(const QStringList &l);

  static QDir uniquePath(const QDir& path, const QList<QDir>& paths);
  static QStringList cmd2list(const QString&);
  static QString userHash();

  /**
    * eMule migration functions
    * helpers for quiet migration
    * migration functions must return empty values or 0!
   */
  static QStringList getFileLines(const QString& filename, const char* codec = NULL);
  static QString emuleConfig(const QString& filename);
  static QString emuleConfigFilename();

#ifdef Q_OS_WIN32
  static QStringList emuleSharedFiles();
  static QStringList emuleSharedDirs();
  static QString emuleKeyFile();
#endif

  static int     migrationPort(int port);
  static QString migrateValue(const QString& value, const QString& def, const char* codepage = NULL);
  static shared_map migrationShareds();
  static QStringList migrationSharedFiles();
  static libed2k::server_met file2smet(const QString&, libed2k::error_code&);
  static bool naturalSort(QString left, QString right, bool& result);
  static QString metadataDirectory(const QString& path);
  static bool prepareInputDirectory(const QString& path);
  static void normalizePath(QString&);

  static int naturalCompare(const QString &s1, const QString &s2,  Qt::CaseSensitivity cs);
};

//  Trick to get a portable sleep() function
class SleeperThread : public QThread {
public:
  static void msleep(unsigned long msecs)
  {
    QThread::msleep(msecs);
  }
};

class CleanupEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit CleanupEventFilter(QObject *parent = 0);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
signals:
public slots:
};

#endif
