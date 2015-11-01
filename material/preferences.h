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

#include <QString>
#include <QDebug>
#include "qinisettings.h"
#include "misc.h"

class Preferences : public QIniSettings {
    Q_OBJECT

    Q_DISABLE_COPY(Preferences)
    Q_PROPERTY(int listenPort READ listenPort WRITE setListenPort)
    Q_PROPERTY(QString nick READ nick WRITE setNick)
    Q_PROPERTY(QString userHash READ userHash WRITE setUserHash)
    Q_PROPERTY(QString inputDir READ inputDir WRITE setInputDir NOTIFY inputDirChanged)
    Q_PROPERTY(bool showAllTransfers READ showAllTransfers WRITE setShowAllTransfers)
    Q_PROPERTY(QString locale READ locale WRITE setLocale)
    Q_PROPERTY(bool pausedTransfersFromCollection READ pausedTransfersFromCollection WRITE setPausedTransfersFromCollection)
    Q_PROPERTY(QString defaultInputDir READ defaultInputDir)
public:
  Preferences() : QIniSettings(COMPANY_NAME,  PRODUCT_NAME){
  }

public:
    int listenPort() const;
    void setListenPort(int);

    QString nick() const;
    void setNick(const QString& nick);

    QString userHash() const;
    void setUserHash(const QString&);


    QString inputDir() const;
    void setInputDir(const QString&);


    bool showAllTransfers() const;
    void setShowAllTransfers(bool);

    void setLocale(const QString&);
    QString locale() const;

    bool pausedTransfersFromCollection() const;
    void setPausedTransfersFromCollection(bool);

    QString defaultInputDir() const;

#ifdef Q_OS_WIN
    static bool isFileAssocSet(const QString& file);
    static void setFileAssoc(const QString& file, bool set);
    static void setCommonAssocSection(bool set);
    static bool isLinkAssocSet(const QString& link);
    static void setLinkAssoc(const QString& link, bool set);
#endif
signals:
     void inputDirChanged(QString);
};

#endif // PREFERENCES_H
