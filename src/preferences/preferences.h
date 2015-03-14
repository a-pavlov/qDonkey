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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QCryptographicHash>
#include <QPair>
#include <QDir>
#include <QTime>
#include <QList>
#include <QDebug>

#ifndef DISABLE_GUI
#include <QApplication>
#else
#include <QCoreApplication>
#endif

#include "misc.h"
#include "qinisettings.h"

#define QBT_REALM "Web UI Access"
enum scheduler_days { EVERY_DAY, WEEK_DAYS, WEEK_ENDS, MON, TUE, WED, THU, FRI, SAT, SUN };
enum maxRatioAction {PAUSE_ACTION, REMOVE_ACTION};
namespace Proxy {
enum ProxyType {HTTP=1, SOCKS5=2, HTTP_PW=3, SOCKS5_PW=4, SOCKS4=5};
}
namespace TrayIcon {
enum Style { NORMAL = 0, MONO_DARK, MONO_LIGHT };
}
namespace DNS {
enum Service { DYNDNS, NOIP, NONE = -1 };
}

class Preferences : public QIniSettings {
  Q_DISABLE_COPY(Preferences)

public:
  Preferences() : QIniSettings(COMPANY_NAME,  PRODUCT_NAME){}

public:

  /**
    * this method executes migrate - write options from orignal eMule to our options
   */
  void migrate()
  {
      if (isMigrationStage())
      {
          saveSharedDirs(misc::migrationShareds());
          setListenPort(misc::migrationPort(4662));
          // do not migrate nick since we use emule.com
          //setNick(misc::migrateValue("Nick", misc::getUserName(), "UTF-8"));
          QString incomingDir = misc::migrateValue("IncomingDir", misc::QDesktopServicesDownloadLocation());
          setSavePathTorrent(incomingDir);
          setSavePathMule(incomingDir);
          qDebug() << "mule save path " << getSavePathMule() << " incoming dir " << incomingDir;
          QString maxDownload = misc::migrateValue("MaxDownload", QString::number(ed2k_max_download), "UTF-8");
          QString maxUpload = misc::migrateValue("MaxUpload", QString::number(ed2k_max_upload), "UTF-8");

          // zero will interpret as default limit
          setED2KDownloadLimit((maxDownload.toLong()==0)?ed2k_max_download:maxDownload.toLong());
          setED2KUploadLimit((maxUpload.toLong()==0)?ed2k_max_upload:maxUpload.toLong());
          // migrate old ipfilter.dat file
          if (!QFile::copy(misc::emuleConfig("ipfilter.dat"), misc::ED2KMetaLocation("ipfilter.dat")))
          {
              qDebug() << "unable to copy " << misc::emuleConfig("ipfilter.dat");
          }

          sync();
      }
  }

  QString getSavePath() const {
      return QString();
  }

  // General options
  QString getLocale() const {
    return value(QString::fromUtf8("Preferences/General/Locale"), "").toString();
  }

  void setLocale(const QString &locale) {
    setValue(QString::fromUtf8("Preferences/General/Locale"), locale);
  }

  bool useProgramNotification() const {
    return value(QString::fromUtf8("Preferences/General/ProgramNotification"), true).toBool();
  }

  void useProgramNotification(bool use) {
    setValue(QString::fromUtf8("Preferences/General/ProgramNotification"), use);
  }

  bool deleteTorrentFilesAsDefault() const {
    return value(QString::fromUtf8("Preferences/General/DeleteTorrentsFilesAsDefault"), false).toBool();
  }

  void setDeleteTorrentFilesAsDefault(bool del) {
    setValue(QString::fromUtf8("Preferences/General/DeleteTorrentsFilesAsDefault"), del);
  }

  void setConfirmOnExit(bool confirm) {
    setValue(QString::fromUtf8("Preferences/General/ExitConfirm"), confirm);
  }

  bool confirmOnExit() const {
    return value(QString::fromUtf8("Preferences/General/ExitConfirm"), true).toBool();
  }

  void showSpeedInTitleBar(bool show) {
    setValue(QString::fromUtf8("Preferences/General/SpeedInTitleBar"), show);
  }

  bool speedInTitleBar() const {
    return value(QString::fromUtf8("Preferences/General/SpeedInTitleBar"), false).toBool();
  }

  bool useAlternatingRowColors() const {
    return value(QString::fromUtf8("Preferences/General/AlternatingRowColors"), true).toBool();
  }

  void setAlternatingRowColors(bool b) {
    setValue("Preferences/General/AlternatingRowColors", b);
  }

  bool systrayIntegration() const {
#ifdef Q_WS_MAC
    return false;
#else
    return value(QString::fromUtf8("Preferences/General/SystrayEnabled"), true).toBool();
#endif
  }

  void setSystrayIntegration(bool enabled) {
    setValue(QString::fromUtf8("Preferences/General/SystrayEnabled"), enabled);
  }

  void setToolbarDisplayed(bool displayed) {
    setValue(QString::fromUtf8("Preferences/General/ToolbarDisplayed"), displayed);
  }

  bool isToolbarDisplayed() const {
    return value(QString::fromUtf8("Preferences/General/ToolbarDisplayed"), true).toBool();
  }

  bool minimizeToTray() const {
    return value(QString::fromUtf8("Preferences/General/MinimizeToTray"), false).toBool();
  }

  void setMinimizeToTray(bool b) {
    setValue("Preferences/General/MinimizeToTray", b);
  }

  bool closeToTray() const {
    return value(QString::fromUtf8("Preferences/General/CloseToTray"), false).toBool();
  }

  void setCloseToTray(bool b) {
    setValue("Preferences/General/CloseToTray", b);
  }

  bool startMinimized() const {
    return value(QString::fromUtf8("Preferences/General/StartMinimized"), false).toBool();
  }

  void setStartMinimized(bool b) {
    setValue("Preferences/General/StartMinimized", b);
  }

  bool isSlashScreenDisabled() const {
    return value(QString::fromUtf8("Preferences/General/NoSplashScreen"), false).toBool();
  }

  void setSplashScreenDisabled(bool b) {
    setValue("Preferences/General/NoSplashScreen", b);
  }

  // Preventing from system suspend while active torrents are presented.
  bool preventFromSuspend() const {
    return value(QString::fromUtf8("Preferences/General/PreventFromSuspend"), false).toBool();
  }

  void setPreventFromSuspend(bool b) {
    setValue("Preferences/General/PreventFromSuspend", b);
  }

  bool checkForUpdates() const {
      return value(QString::fromUtf8("Preferences/General/CheckForUpdates"), true).toBool();
  }

  void setCheckForUpdates(bool b){
      setValue(QString::fromUtf8("Preferences/General/CheckForUpdates"), b);
  }

  // Downloads
  QString getSavePathTorrent() const {
    QString save_path = value(QString::fromUtf8("Preferences/Downloads/SavePath")).toString();
    if (!save_path.isEmpty())
      return save_path;
    return misc::QDesktopServicesDownloadLocation();
  }

  void setSavePathTorrent(const QString &save_path) {
    setValue(QString::fromUtf8("Preferences/Downloads/SavePath"), save_path);
  }

  bool isTempPathEnabledTorrent() const {
    return value(QString::fromUtf8("Preferences/Downloads/TempPathEnabled"), true).toBool();
  }

  void setTempPathEnabledTorrent(bool enabled) {
    setValue(QString::fromUtf8("Preferences/Downloads/TempPathEnabled"), enabled);
  }

  QString getTempPathTorrent() const {
    const QString temp = QDir(getSavePathTorrent()).absoluteFilePath("temp");
    return value(QString::fromUtf8("Preferences/Downloads/TempPath"), temp).toString();
  }  

  void setTempPathTorrent(const QString &path) {
    setValue(QString::fromUtf8("Preferences/Downloads/TempPath"), path);
  }

  // Downloads mule section
  QString getSavePathMule() const {
    QString save_path = value(QString::fromUtf8("Preferences/Downloads/SavePathMule")).toString();
#if defined(Q_WS_WIN) || defined(Q_OS_OS2)
    save_path.replace("/", "\\");
#endif
    if (!save_path.isEmpty()) return save_path;
    return misc::QDesktopServicesDownloadLocation();
  }

  void setSavePathMule(const QString &save_path) { setValue(QString::fromUtf8("Preferences/Downloads/SavePathMule"), save_path); }
  bool isTempPathEnabledMule() const { return value(QString::fromUtf8("Preferences/Downloads/TempPathEnabledMule"), true).toBool(); }
  void setTempPathEnabledMule(bool enabled) { setValue(QString::fromUtf8("Preferences/Downloads/TempPathEnabledMule"), enabled); }
  QString getTempPathMule() const
  {
      const QString temp = QDir(getSavePathMule()).absoluteFilePath("temp");
      QString res = value(QString::fromUtf8("Preferences/Downloads/TempPathMule"), temp).toString();
#if defined(Q_WS_WIN) || defined(Q_OS_OS2)
      res.replace("/", "\\");
#endif
      return res;
  }

  void setTempPathMule(const QString &path) { setValue(QString::fromUtf8("Preferences/Downloads/TempPathMule"), path);}

  bool useIncompleteFilesExtension() const {
    return value(QString::fromUtf8("Preferences/Downloads/UseIncompleteExtension"), false).toBool();
  }

  void useIncompleteFilesExtension(bool enabled) {
    setValue(QString::fromUtf8("Preferences/Downloads/UseIncompleteExtension"), enabled);
  }

  bool appendTorrentLabel() const {
    return value(QString::fromUtf8("Preferences/Downloads/AppendLabel"), false).toBool();
  }

  void setAppendTorrentLabel(bool b) {
    setValue("Preferences/Downloads/AppendLabel", b);
  }

  QString lastLocationPath() const {
    return value(QString::fromUtf8("Preferences/Downloads/LastLocationPath"), QString()).toString();
}

  void setLastLocationPath(const QString &path) {
    setValue(QString::fromUtf8("Preferences/Downloads/LastLocationPath"), path);
  }

  bool preAllocateAllFiles() const {
    return value(QString::fromUtf8("Preferences/Downloads/PreAllocation"), true).toBool();
  }

  void preAllocateAllFiles(bool enabled) {
    return setValue(QString::fromUtf8("Preferences/Downloads/PreAllocation"), enabled);
  }

  bool useAdditionDialog() const {
    return value(QString::fromUtf8("Preferences/Downloads/AdditionDialog"), false).toBool();
  }

  void useAdditionDialog(bool b) {
    setValue("Preferences/Downloads/AdditionDialog", b);
  }

  bool addTorrentsInPause() const {
    return value(QString::fromUtf8("Preferences/Downloads/StartInPause"), false).toBool();
  }

  void addTorrentsInPause(bool b) {
    setValue("Preferences/Downloads/StartInPause", b);
  }

  QStringList getScanDirs() const {
    return value(QString::fromUtf8("Preferences/Downloads/ScanDirs"), QStringList()).toStringList();
  }

  // This must be called somewhere with data from the model
  void setScanDirs(const QStringList &dirs) {
    setValue(QString::fromUtf8("Preferences/Downloads/ScanDirs"), dirs);
  }

  QList<bool> getDownloadInScanDirs() const {
    return misc::boolListfromStringList(value(QString::fromUtf8("Preferences/Downloads/DownloadInScanDirs")).toStringList());
  }

  void setDownloadInScanDirs(const QList<bool> &list) {
    setValue(QString::fromUtf8("Preferences/Downloads/DownloadInScanDirs"), misc::toStringList(list));
  }

  bool isTorrentExportEnabled() const {
    return !value(QString::fromUtf8("Preferences/Downloads/TorrentExport"), QString()).toString().isEmpty();
  }

  QString getExportDir() const {
    return value(QString::fromUtf8("Preferences/Downloads/TorrentExport"), QString()).toString();
  }

  void setExportDir(QString path) {
    path = path.trimmed();
    if (path.isEmpty())
      path = QString();
    setValue(QString::fromUtf8("Preferences/Downloads/TorrentExport"), path);
  }


  int getActionOnDblClOnTorrentDl() const {
    return value(QString::fromUtf8("Preferences/Downloads/DblClOnTorDl"), 0).toInt();
  }

  void setActionOnDblClOnTorrentDl(int act) {
    setValue("Preferences/Downloads/DblClOnTorDl", act);
  }

  int getActionOnDblClOnTorrentFn() const {
    return value(QString::fromUtf8("Preferences/Downloads/DblClOnTorFn"), 1).toInt();
  }

  void setActionOnDblClOnTorrentFn(int act) {
    setValue("Preferences/Downloads/DblClOnTorFn", act);
  }

  // Connection options
  int getSessionPort() const {
    return value(QString::fromUtf8("Preferences/Connection/PortRangeMin"), 6881).toInt();
  }

  void setSessionPort(int port) {
    setValue(QString::fromUtf8("Preferences/Connection/PortRangeMin"), port);
  }

  bool isUPnPEnabled() const {
    return value(QString::fromUtf8("Preferences/Connection/UPnP"), true).toBool();
  }

  void setUPnPEnabled(bool enabled) {
    setValue(QString::fromUtf8("Preferences/Connection/UPnP"), enabled);
  }

  int getGlobalDownloadLimit() const {
    return value("Preferences/Connection/GlobalDLLimit", -1).toInt();
  }

  void setGlobalDownloadLimit(int limit) {
    if (limit <= 0) limit = -1;
    setValue("Preferences/Connection/GlobalDLLimit", limit);
  }

  int getGlobalUploadLimit() const {
    return value("Preferences/Connection/GlobalUPLimit", 300).toInt();
  }

  void setGlobalUploadLimit(int limit) {
    if (limit <= 0) limit = -1;
    setValue("Preferences/Connection/GlobalUPLimit", limit);
  }

  int getAltGlobalDownloadLimit() const {
    int ret = value(QString::fromUtf8("Preferences/Connection/GlobalDLLimitAlt"), 10).toInt();
    if (ret <= 0)
      ret = 10;
    return ret;
  }

  void setAltGlobalDownloadLimit(int limit) {
    if (limit <= 0) limit = -1;
    setValue("Preferences/Connection/GlobalDLLimitAlt", limit);
  }

  int getAltGlobalUploadLimit() const {
    int ret = value(QString::fromUtf8("Preferences/Connection/GlobalUPLimitAlt"), 10).toInt();
    if (ret <= 0)
      ret = 10;
    return ret;
  }

  void setAltGlobalUploadLimit(int limit) {
    if (limit <= 0) limit = -1;
    setValue("Preferences/Connection/GlobalUPLimitAlt", limit);
  }

  bool isAltBandwidthEnabled() const {
    return value("Preferences/Connection/alt_speeds_on", false).toBool();
  }

  void setAltBandwidthEnabled(bool enabled) {
    setValue("Preferences/Connection/alt_speeds_on", enabled);
  }

  // ED2K options
  int getPartialTransfersCount() const
  {
      return value("Preferences/Connection/ED2KPartialTransfersCount", 100).toInt();
  }

  void setPartialTransfersCount(int tcount)
  {
      setValue("Preferences/Connection/ED2KPartialTransfersCount", tcount);
  }


  int getED2KDownloadLimit() const
  {
    return value("Preferences/Connection/ED2KDLLimit", ed2k_max_download).toInt();
  }

  void setED2KDownloadLimit(int limit)
  {
    if (limit <= 0) limit = -1;
    setValue("Preferences/Connection/ED2KDLLimit", limit);
  }

  int getED2KUploadLimit() const
  {
    return value("Preferences/Connection/ED2KUPLimit", ed2k_max_upload).toInt();
  }

  void setED2KUploadLimit(int limit)
  {
    if (limit <= 0) limit = -1;
    setValue("Preferences/Connection/ED2KUPLimit", limit);
  }

  int serverPort() const
  {            
      return value(QString::fromUtf8("Preferences/Connection/ServerPort"), 4661).toInt();
  }

  void setServerPort(int nServerPort)
  {
      setValue("Preferences/Connection/ServerPort", nServerPort);
  }

  QString serverName()
  {
      return value(QString::fromUtf8("Preferences/Connection/ServerName"), "emule.com").toString();
  }

  bool serverReconnect()
  {
      return value("Preferences/Connection/ServerReconnect", true).toBool();
  }

  void setServerReconnect(bool bServerReconnect)
  {
      setValue("Preferences/Connection/ServerReconnect", bServerReconnect);
  }

  void setServerName(const QString& strServerName)
  {
      setValue("Preferences/Connection/ServerName", strServerName);
  }

  void setSchedulerEnabled(bool enabled) {
    setValue(QString::fromUtf8("Preferences/Scheduler/Enabled"), enabled);
  }

  bool isSchedulerEnabled() const {
    return value(QString::fromUtf8("Preferences/Scheduler/Enabled"), false).toBool();
  }

  QTime getSchedulerStartTime() const {
    return value(QString::fromUtf8("Preferences/Scheduler/start_time"), QTime(8,0)).toTime();
  }

  void setSchedulerStartTime(const QTime &time) {
    setValue(QString::fromUtf8("Preferences/Scheduler/start_time"), time);
  }

  QTime getSchedulerEndTime() const {
    return value(QString::fromUtf8("Preferences/Scheduler/end_time"), QTime(20,0)).toTime();
  }

  void setSchedulerEndTime(const QTime &time) {
    setValue(QString::fromUtf8("Preferences/Scheduler/end_time"), time);
  }

  scheduler_days getSchedulerDays() const {
    return (scheduler_days)value(QString::fromUtf8("Preferences/Scheduler/days"), EVERY_DAY).toInt();
  }

  void setSchedulerDays(scheduler_days days) {
    setValue(QString::fromUtf8("Preferences/Scheduler/days"), (int)days);
  }

  //LIBED2K options

  QString userHash() const{
      return value(QString::fromUtf8("Preferences/eDonkey/UserHash"), QString()).toString();
  }

  void setUserHash(const QString& uhash){
      setValue(QString::fromUtf8("Preferences/eDonkey/UserHash"), uhash);
  }

  // in first time we have migration status true
  bool isMigrationStage() const
  {
      return value(QString::fromUtf8("Preferences/eDonkey/eMuleMigration"), true).toBool();
  }

  void setMigrationStage(bool mgr)
  {
      setValue(QString::fromUtf8("Preferences/eDonkey/eMuleMigration"), mgr);
  }

  bool isShowSharedFiles() const
  {
      return value(QString::fromUtf8("Preferences/eDonkey/ShowSharedFiles"), true).toBool();
  }

  void setShowSharedFiles(bool bShowSharedFiles)
  {
      setValue(QString::fromUtf8("Preferences/eDonkey/ShowSharedFiles"), bShowSharedFiles);
  }

  bool isShowSharedDirectories() const
  {
      return value(QString::fromUtf8("Preferences/eDonkey/ShowSharedDirectories"), true).toBool();
  }

  void setShowSharedDirectories(bool bShowSharedDirs)
  {
      setValue(QString::fromUtf8("Preferences/eDonkey/ShowSharedDirectories"), bShowSharedDirs);
  }

  void saveSharedDirs(const shared_map& se)
  {
      int index = 0;
      beginGroup("SharedDirectories");
      beginWriteArray("ShareDirs");

      for (shared_map::const_iterator itr = se.begin(); itr != se.end(); ++itr)
      {
          setArrayIndex(index);
          setValue("Path", itr.key());
          beginWriteArray("ExcludeFiles", itr.value().size());

          for (int n = 0; n < itr.value().size(); ++n)
          {
              setArrayIndex(n);
              setValue("FileName", itr.value().at(n));
          }

          endArray();
          ++index;
      }

      endArray();
      endGroup();
  }

  int listenPort() const {
      return value(QString::fromUtf8("Preferences/eDonkey/ListenPort"), 4662).toInt();
  }

  void setListenPort(int nListenPort) {
      setValue(QString::fromUtf8("Preferences/eDonkey/ListenPort"), nListenPort);
  }

  QString nick() const
  {
      return value(QString::fromUtf8("Preferences/eDonkey/Nick"), QString("www.emule.com")).toString();
  }

  void setNick(const QString& nick)
  {
      setValue(QString::fromUtf8("Preferences/eDonkey/Nick"), nick);
  }


  void setSaveSearchKeywords(bool enabled) {
      setValue(QString::fromUtf8("Preferences/eDonkey/SaveSearchKeywords"), enabled);
  }

  bool saveSearchKeywords() const {
      return value(QString::fromUtf8("Preferences/eDonkey/SaveSearchKeywords"), true).toBool();
  }
};

#endif // PREFERENCES_H
