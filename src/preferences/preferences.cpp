#include "preferences.h"

const QString pathConfOnExt = "Common/ConfirmOnExit";
const QString pathDispSpeed = "Common/DisplaySpeedInTitle";
const QString pathServerHost = "Network/ServerHost";
const QString pathServerPort = "Network/ServerPort";
const QString pathListenPort = "Network/ListenPort";
const QString pathNick      = "Network/Nick";
const QString pathUserHash  = "Network/UserHash";
const QString pathUPnp      = "Network/UPnP";
const QString pathInputDir  = "HD/InputDir";
const QString pathSysTrAvail= "System/SystrayAvailable";

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
