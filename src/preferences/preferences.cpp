#include "preferences.h"
#include <QApplication>

const QString pathConfOnExt = "Common/ConfirmOnExit";
const QString pathDispSpeed = "Common/DisplaySpeedInTitle";
const QString pathPausedCT  = "Common/PausedTransfersFromCollections";
const QString pathServerHost = "Network/ServerHost";
const QString pathServerPort = "Network/ServerPort";
const QString pathListenPort = "Network/ListenPort";
const QString pathNick      = "Network/Nick";
const QString pathUserHash  = "Network/UserHash";
const QString pathUPnp      = "Network/UPnP";
const QString pathInputDir  = "HD/InputDir";
const QString pathSysTrAvail= "System/SystrayAvailable";
const QString pathShowAllransfers = "Common/ShowAllTransfers";
const QString pathLocale    = "Common/Locale";

bool Preferences::confirmOnExit() const {
    return value(pathConfOnExt, true).toBool();
}

void Preferences::setConfirmOnExit(bool flag) {
    setValue(pathConfOnExt, flag);
}

bool Preferences::displaySpeedInTitle() const {
    return value(pathDispSpeed, false).toBool();
}

void Preferences::setDisplaySpeedInTitle(bool flag) {
    setValue(pathDispSpeed, flag);
}

QString Preferences::serverHost() const {
#ifdef INET_SERVER
    return value(pathServerHost, "91.200.42.46").toString();
#else
    return value(pathServerHost, "emule.is74.ru").toString();
#endif
}

int Preferences::serverPort() const {
#ifdef INET_SERVER
    return value(pathServerPort, 1176).toInt();
#else
    return value(pathServerPort, 4661).toInt();
#endif
}

int Preferences::listenPort() const {
    return value(pathListenPort, 9001).toInt();
}

void Preferences::setListenPort(int port) {
    Q_ASSERT(port > 0);
    setValue(pathListenPort, port);
}

QString Preferences::nick() const {
    return value(pathNick, "User").toString();
}

void Preferences::setNick(const QString& nick) {
    setValue(pathNick, nick);
}

QString Preferences::userHash() const {
    return value(pathUserHash, "").toString();
}

void Preferences::setUserHash(const QString& hash) {
    setValue(pathUserHash, hash);
}

bool Preferences::forwardPort() const {
    return value(pathUPnp, false).toBool();
}

void Preferences::setForwardPort(bool flag) {
    setValue(pathUPnp, flag);
}

QString Preferences::inputDir() const {
    return value(pathInputDir, "").toString();
}

void Preferences::setInputDir(const QString& path) {
    Q_ASSERT(!path.isEmpty());
    setValue(pathInputDir, path);
}

bool Preferences::sysTrayAvailable() const {
    return value(pathSysTrAvail, true).toBool();
}

void Preferences::setSysTrayAvailable(bool flag) {
    setValue(pathSysTrAvail, flag);
}

bool Preferences::showAllTransfers() const {
    return value(pathShowAllransfers, true).toBool();
}

void Preferences::setShowAllTransfers(bool flag) {
    setValue(pathShowAllransfers, flag);
}

void Preferences::setLocale(const QString& locale) {
    setValue(pathLocale, locale);
}

QString Preferences::getLocale() const {
    return value(pathLocale, "").toString();
}

bool Preferences::pausedTransfersFromCollection() const {
    return value(pathPausedCT, true).toBool();
}

void Preferences::setPausedTransfersFromCollection(bool flag) {
    setValue(pathPausedCT, flag);
}

#ifdef Q_OS_WIN

//static
bool Preferences::isFileAssocSet(const QString& file) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes", QIniSettings::NativeFormat);
    if (settings.value(file + QString("/Default")).toString() != "qDonkey") {
      qDebug(".torrent != qDonkey");
      return false;
    }

    qDebug("Checking shell command");
    QString shell_command = settings.value("qDonkey/shell/open/command/Default", "").toString();
    qDebug("Shell command is: %s", qPrintable(shell_command));
    QRegExp exe_reg("\"([^\"]+)\".*");
    if (exe_reg.indexIn(shell_command) < 0)
      return false;
    QString assoc_exe = exe_reg.cap(1);
    qDebug("exe: %s", qPrintable(assoc_exe));
    if (assoc_exe.compare(qApp->applicationFilePath().replace("/", "\\"), Qt::CaseInsensitive) != 0)
      return false;
    // Icon
    const QString icon_str = "\""+qApp->applicationFilePath().replace("/", "\\")+"\",0";
    if (settings.value("qDonkey/DefaultIcon/Default", icon_str).toString().compare(icon_str, Qt::CaseInsensitive) != 0)
      return false;

    return true;
}

//static
void Preferences::setFileAssoc(const QString& file, bool set) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes", QIniSettings::NativeFormat);

    // .file association
    if (set)
    {
        settings.setValue(file + QString("/Default"), "qDonkey");
        settings.setValue(file + QString("/Content Type"), "application/x-bittorrent");
        settings.setValue(file + QString("/OpenWithProgids/qDonkey"), "");
    }
    else
    {
        settings.remove(file + QString("/Default"));
        settings.remove(file + QString("/Content Type"));
        settings.remove(file + QString("/OpenWithProgids"));
    }
}

// set common section of qDonkey
//static
void Preferences::setCommonAssocSection(bool set) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes", QIniSettings::NativeFormat);

    if (set) {
        const QString command_str = "\""+qApp->applicationFilePath().replace("/", "\\")+"\" \"%1\"";
        const QString icon_str = "\""+qApp->applicationFilePath().replace("/", "\\")+"\",0";
        settings.setValue("qDonkey/shell/Default", "open");
        settings.setValue("qDonkey/shell/open/command/Default", command_str);
        settings.setValue("qDonkey/Content Type/Default", "application/x-bittorrent");
        settings.setValue("qDonkey/DefaultIcon/Default", icon_str);
    }
    else {
        settings.remove("qDonkey/shell/Default");
        settings.remove("qDonkey/shell/open/command/Default");
        settings.remove("qDonkey/Content Type/Default");
        settings.remove("qDonkey/DefaultIcon/Default");
    }
}

//static
bool Preferences::isLinkAssocSet(const QString& link) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes", QIniSettings::NativeFormat);
    // Check link assoc
    QRegExp exe_reg("\"([^\"]+)\".*");
    QString shell_command = settings.value(link + "/shell/open/command/Default", "").toString();
    if (exe_reg.indexIn(shell_command) < 0)
    return false;
    QString assoc_exe = exe_reg.cap(1);
    qDebug("exe: %s", qPrintable(assoc_exe));
    if (assoc_exe.compare(qApp->applicationFilePath().replace("/", "\\"), Qt::CaseInsensitive) != 0) return false;
    return true;
}

//static
void Preferences::setLinkAssoc(const QString& link, bool set) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Classes", QIniSettings::NativeFormat);

    // Magnet association
    if (set) {
        const QString command_str = "\""+qApp->applicationFilePath().replace("/", "\\")+"\" \"%1\"";
        const QString icon_str = "\""+qApp->applicationFilePath().replace("/", "\\")+"\",1";

        settings.setValue(link + "/Default", QString(link + QString(" URI")));
        settings.setValue(link + "/Content Type", QString("application/x-" + link.toLower()));
        settings.setValue(link + "/URL Protocol", "");
        settings.setValue(link + "/DefaultIcon/Default", icon_str);
        settings.setValue(link + "/shell/Default", "open");
        settings.setValue(link + "/shell/open/command/Default", command_str);
    } else {
        settings.remove(link + "/Default");
        settings.remove(link + "/Content Type");
        settings.remove(link + "/URL Protocol");
        settings.remove(link + "/DefaultIcon/Default");
        settings.remove(link + "/shell/Default");
        settings.remove(link + "/shell/open/command/Default");
    }
}

#endif
