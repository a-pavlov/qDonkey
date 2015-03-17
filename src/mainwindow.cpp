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

#include <QtGlobal>
#if defined(Q_WS_X11) && defined(QT_DBUS_LIB)
#include <QDBusConnection>
#include "notifications.h"
#endif

#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopServices>
#include <QStatusBar>
#include <QClipboard>
#include <QCloseEvent>
#include <QShortcut>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QDockWidget>
#include <QKeyEvent>
#include <QInputDialog>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QUrl>

#include <libed2k/log.hpp>

#include "mainwindow.h"
#include "misc.h"

#include "preferences.h"
#include "qinisettings.h"
#include "status_bar.h"
#include "geoipmanager.h"
#include "iconprovider.h"

#include "search/search_widget.h"
#include "transfers/transfers_widget.h"
#include "preferences/preferences_widget.h"

#ifdef Q_WS_MAC
#include "qmacapplication.h"
void qt_mac_set_dock_menu(QMenu *menu);
#endif

#include "sessionapplication.h"
#include "powermanagement.h"
#include "res.h"

#define TIME_TRAY_BALLOON 5000
#define PREVENT_SUSPEND_INTERVAL 60000

/*****************************************************
 *                                                   *
 *                       GUI                         *
 *                                                   *
 *****************************************************/

// Constructor
MainWindow::MainWindow(QWidget *parent, QStringList torrentCmdLine)
    :
      QMainWindow(parent),
      m_posInitialized(false),
      force_exit(false),
      icon_TrayConn(res::trayConnected()),
      icon_TrayDisconn(res::trayDisconnected()),
      icon_CurTray(icon_TrayDisconn)
{
    setupUi(this);
    stackedWidget->addWidget(new transfers_widget(this));
    stackedWidget->addWidget(new search_widget(this));
    stackedWidget->addWidget(new preferences_widget(this));
    stackedWidget->setCurrentIndex(0);
    QCoreApplication::instance()->installEventFilter(this);

    //QApplication::setOverrideCursor(Qt::WaitCursor);
    m_last_file_error = QDateTime::currentDateTime().addSecs(-1); // imagine last file error event was 1 seconds in past

#ifdef Q_WS_WIN
    m_nTaskbarButtonCreated = RegisterWindowMessage(L"TaskbarButtonCreated");
#else
    m_nTaskbarButtonCreated = 0;
#endif

    Preferences pref;
    setWindowTitle(QString::fromUtf8(PRODUCT_NAME));

    // Clean exit on log out
    connect(static_cast<SessionApplication*>(qApp), SIGNAL(sessionIsShuttingDown()), this, SLOT(deleteSession()));

    this->setWindowIcon(QIcon(res::favicon()));

#ifdef Q_WS_MAC
    connect(static_cast<QMacApplication*>(qApp), SIGNAL(newFileOpenMacEvent(QString)), this, SLOT(processParams(QString)));
#endif

    statusBar = new status_bar(this, QMainWindow::statusBar());
    m_pwr = new PowerManagement(this);

    // Configure session according to options
    loadPreferences(false);

    // Start connection checking timer
    guiUpdater = new QTimer(this);
    connect(guiUpdater, SIGNAL(timeout()), this, SLOT(updateGUI()));
    guiUpdater->start(2000);

    // Accept drag 'n drops
    //setAcceptDrops(true);
    createKeyboardShortcuts();

#ifdef Q_WS_MAC
    setUnifiedTitleAndToolBarOnMac(true);
#endif

    // View settings



    // Auto shutdown actions
/*    QActionGroup * autoShutdownGroup = new QActionGroup(this);
    autoShutdownGroup->setExclusive(true);
    autoShutdownGroup->addAction(actionAutoShutdown_Disabled);
    autoShutdownGroup->addAction(actionAutoExit_mule);
    autoShutdownGroup->addAction(actionAutoShutdown_system);
    autoShutdownGroup->addAction(actionAutoSuspend_system);

#if !defined(Q_WS_X11) || defined(QT_DBUS_LIB)
    actionAutoShutdown_system->setChecked(pref.shutdownWhenDownloadsComplete());
    actionAutoSuspend_system->setChecked(pref.suspendWhenDownloadsComplete());
#else
    actionAutoShutdown_system->setDisabled(true);
    actionAutoSuspend_system->setDisabled(true);
#endif

    actionAutoExit_mule->setChecked(pref.shutdownqBTWhenDownloadsComplete());

    if (!autoShutdownGroup->checkedAction())
        actionAutoShutdown_Disabled->setChecked(true);
*/

    readSettings();

    show();
    activateWindow();
    raise();

    // Start watching the executable for updates
    executable_watcher = new QFileSystemWatcher();
    connect(executable_watcher, SIGNAL(fileChanged(QString)), this, SLOT(notifyOfUpdate(QString)));
    executable_watcher->addPath(qApp->applicationFilePath());


    // Resume unfinished torrents
    Session::instance()->startUpTransfers();

    // Add torrent given on command line
    processParams(torrentCmdLine);

    qDebug("GUI has been built");

#ifdef Q_WS_MAC
    qt_mac_set_dock_menu(getTrayIconMenu());
#endif

    // Make sure the Window is visible if we don't have a tray icon
    if (!systrayIcon && isHidden())
    {
        show();
        activateWindow();
        raise();
    }

    //connect(servers, SIGNAL(sigConnectPending(QED2KServerFingerprint)),
    //        this, SLOT(ed2kConnectionPending(QED2KServerFingerprint)));
    //connect(Session::instance(), SIGNAL(serverConnectionInitialized(QED2KServerFingerprint, quint32, quint32, quint32)),
    //      this, SLOT(ed2kConnectionInitialized(QED2KServerFingerprint, quint32, quint32, quint32)));
    //connect(Session::instance(), SIGNAL(serverStatus(QED2KServerFingerprint, int, int)),
    //      this, SLOT(ed2kServerStatus(QED2KServerFingerprint, int, int)));
   // connect(Session::instance(), SIGNAL(serverConnectionClosed(QED2KServerFingerprint, QString)),
    //      this, SLOT(ed2kConnectionClosed(QED2KServerFingerprint, QString)));

    //connect(Session::instance(), SIGNAL(newConsoleMessage(const QString&)), servers, SLOT(addHtmlLogMessage(const QString&)));
    connect(Session::instance(), SIGNAL(serverNameResolved(QString)), this, SLOT(handleServerNameResolved(QString)));
    connect(Session::instance(), SIGNAL(serverConnectionInitialized(quint32,quint32,quint32)), this, SLOT(handleServerConnectionInitialized(quint32,quint32,quint32)));
    connect(Session::instance(), SIGNAL(serverConnectionClosed(QString)), this, SLOT(handleServerConnectionClosed(QString)));
    connect(Session::instance(), SIGNAL(serverStatus(int,int)), this, SLOT(handleServerStatus(int,int)));
    connect(Session::instance(), SIGNAL(serverMessage(QString)), this, SLOT(handleServerMessage(QString)));
    connect(Session::instance(), SIGNAL(serverIdentity(QString,QString)), this, SLOT(handleServerIdentity(QString,QString)));

    //Tray actions.
    //connect(actionToggleVisibility, SIGNAL(triggered()), this, SLOT(toggleVisibility()));
    //connect(actionStart_All, SIGNAL(triggered()), Session::instance(), SLOT(resumeAllTransfers()));
    //connect(actionPause_All, SIGNAL(triggered()), Session::instance(), SLOT(pauseAllTransfers()));

    Session::instance()->start();
}

void MainWindow::deleteSession()
{
    guiUpdater->stop();
    Session::drop();
    m_pwr->setActivityState(false);
    // Save window size, columns size
    writeSettings();
    // Accept exit
    qApp->exit();

}

// Destructor
MainWindow::~MainWindow()
{
    qDebug("GUI destruction");
    hide();

#ifdef Q_WS_MAC
    // Workaround to avoid bug http://bugreports.qt.nokia.com/browse/QTBUG-7305
    setUnifiedTitleAndToolBarOnMac(false);
#endif

    // Delete other GUI objects
    if(executable_watcher)
        delete executable_watcher;

    delete statusBar;
    delete guiUpdater;


    if(systrayCreator)
        delete systrayCreator;

    if(systrayIcon)
        delete systrayIcon;

    if(myTrayIconMenu)
        delete myTrayIconMenu;

    // Keyboard shortcuts
    delete switchTransferShortcut;
    delete hideShortcut;

    IconProvider::drop();

    // Delete Session::instance() object
    m_pwr->setActivityState(false);
    //Session::instance()->dropDirectoryTransfers();
    //Session::instance()->saveFileSystem();
    qDebug("Deleting Session::instance()");
    Session::drop();
    qDebug("Exiting GUI destructor...");
}


void MainWindow::writeSettings()
{
    Preferences pref;
    pref.beginGroup(QString::fromUtf8("MainWindow"));
    pref.setValue("geometry", saveGeometry());
    pref.endGroup();
}

void MainWindow::readSettings()
{
    Preferences pref;
    pref.beginGroup(QString::fromUtf8("MainWindow"));

    if(pref.contains("geometry"))
    {
        if(restoreGeometry(pref.value("geometry").toByteArray()))
            m_posInitialized = true;
    }

    const QByteArray splitterState = pref.value("vsplitterState").toByteArray();
    pref.endGroup();
}

void MainWindow::balloonClicked() {
    if(isHidden()) {
        show();
        if(isMinimized()) showNormal();
        raise();
        activateWindow();
    }
}

// called when a transfer has started
void MainWindow::addedTransfer(const QED2KHandle& h) const
{
    //if(TorrentPersistentData::getAddedDate(h.hash()).secsTo(QDateTime::currentDateTime()) <= 1 && !h.is_seed())
    //    showNotificationBaloon(tr("Download starting"), tr("%1 has started downloading.", "e.g: xxx.avi has started downloading.").arg(h.name()));
}

// called when a transfer has finished
void MainWindow::finishedTransfer(const QED2KHandle& h) const
{
    //if(!TorrentPersistentData::isSeed(h.hash()))
    //    showNotificationBaloon(tr("Download completion"), tr("%1 has finished downloading.", "e.g: xxx.avi has finished downloading.").arg(h.name()));
}

// Notification when disk is full and other disk errors
void MainWindow::fileError(const QED2KHandle& h, QString msg)
{
    QDateTime cdt = QDateTime::currentDateTime();

    if(m_last_file_error.secsTo(cdt) > 1)
    {
        showNotificationBaloon(tr("I/O Error"), tr("An I/O error occured for %1.\nReason: %2").arg(h.name()).arg(msg));
    }

    m_last_file_error = cdt;
}

void MainWindow::createKeyboardShortcuts()
{
    actionOpen->setShortcut(QKeySequence(QString::fromUtf8("Ctrl+O")));
    actionExit->setShortcut(QKeySequence(QString::fromUtf8("Ctrl+Q")));
    switchTransferShortcut = new QShortcut(QKeySequence(tr("Alt+1", "shortcut to switch to first tab")), this);
    connect(switchTransferShortcut, SIGNAL(activated()), this, SLOT(displayTransferTab()));
    hideShortcut = new QShortcut(QKeySequence(QString::fromUtf8("Esc")), this);
    connect(hideShortcut, SIGNAL(activated()), this, SLOT(hide()));
}


// Necessary if we want to close the window
// in one time if "close to systray" is enabled
void MainWindow::on_actionExit_triggered()
{
    force_exit = true;
    close();
}

// Toggle Main window visibility
void MainWindow::toggleVisibility(QSystemTrayIcon::ActivationReason e)
{
    if(e == QSystemTrayIcon::Trigger || e == QSystemTrayIcon::DoubleClick)
    {
        if(isHidden())
        {
            show();

            if(isMinimized())
            {
                if(isMaximized())
                {
                    showMaximized();
                }
                else
                {
                    showNormal();
                }
            }

            raise();
            activateWindow();
        }
        else
        {
            hide();
        }
    }

    //actionToggleVisibility->setText(isVisible()? tr("Hide") : tr("Show"));
}

void MainWindow::showEvent(QShowEvent *e)
{
    qDebug("** Show Event **");

//  if(getCurrentTabWidget() == transferList) {
//    properties->loadDynamicData();
    //}

    e->accept();

    // Make sure the window is initially centered
    if(!m_posInitialized)
    {
        move(misc::screenCenter(this));
        m_posInitialized = true;
    }
}

// Called when we close the program
void MainWindow::closeEvent(QCloseEvent *e)
{
    Preferences pref;
    SessionStatus status = Session::instance()->getSessionStatus();

    // has active transfers or sessions speed > 0 (we have incoming peers)
    if (pref.confirmOnExit() && (Session::instance()->hasActiveTransfers() || (status.payload_download_rate > 0) || (status.payload_upload_rate > 0))) {
        if (e->spontaneous() || force_exit) {
            if(!isVisible())
                show();

            QMessageBox confirmBox(QMessageBox::Question, tr("Exiting qDonkey"),
                                   tr("Some files are currently transferring.\nAre you sure you want to quit qDonkey?"),
                                   QMessageBox::NoButton, this);

            QPushButton *noBtn = confirmBox.addButton(tr("No"), QMessageBox::NoRole);
            QPushButton *yesBtn = confirmBox.addButton(tr("Yes"), QMessageBox::YesRole);
            QPushButton *alwaysBtn = confirmBox.addButton(tr("Always"), QMessageBox::YesRole);
            confirmBox.setDefaultButton(yesBtn);
            confirmBox.exec();

            if(!confirmBox.clickedButton() || confirmBox.clickedButton() == noBtn)
            {
                // Cancel exit
                e->ignore();
                force_exit = false;
                return;
            }

            if(confirmBox.clickedButton() == alwaysBtn)
            {
                // Remember choice
                Preferences().setConfirmOnExit(false);
            }
        }
    }

    hide();

    if(systrayIcon) {
        systrayIcon->hide();
    }

    // Save window size, columns size
    writeSettings();

    // Accept exit
    e->accept();
    qApp->exit();
}

bool MainWindow::event(QEvent * e)
{
    switch(e->type())
    {
        case QEvent::WindowStateChange:
        {
            qDebug("Window change event");

            //Now check to see if the window is minimised
            if(isMinimized())
            {
                qDebug("minimisation");

                /*if(systrayIcon) {
                    qDebug("Minimize to Tray enabled, hiding!");
                    e->accept();
                    QTimer::singleShot(0, this, SLOT(hide()));
                    return true;
                }
                */
            }

            break;
        }
#ifdef Q_WS_MAC
            case QEvent::ToolBarChange:
            {
                qDebug("MAC: Received a toolbar change event!");
                bool ret = QMainWindow::event(e);

                qDebug("MAC: new toolbar visibility is %d", !actionTop_tool_bar->isChecked());
                actionTop_tool_bar->toggle();
                Preferences().setToolbarDisplayed(actionTop_tool_bar->isChecked());
                return ret;
            }
#endif
        default:
            break;
    }

    return QMainWindow::event(e);
}

#ifdef Q_WS_WIN
bool MainWindow::winEvent(MSG * message, long * result)
{
    if (message->message == m_nTaskbarButtonCreated)
    {
        qDebug() << "initialize task bar";
        m_tbar->initialize();
        m_tbar->setState(winId(), taskbar_iface::S_NOPROGRESS);
        return true;
    }

    return false;
}
#endif

// Action executed when a file is dropped
/*void MainWindow::dropEvent(QDropEvent *event)
{
    event->acceptProposedAction();
    QStringList files;

    if(event->mimeData()->hasUrls())
    {
        const QList<QUrl> urls = event->mimeData()->urls();

        foreach (const QUrl &url, urls)
        {
            if (!url.isEmpty())
            {
                if (url.scheme().compare("file", Qt::CaseInsensitive) == 0)
                files << url.toLocalFile();
                else
                files << url.toString();
            }
        }
    }
    else
    {
        files = event->mimeData()->text().split(QString::fromUtf8("\n"));
    }

    // Add file to download list
    Preferences pref;
    const bool useTorrentAdditionDialog = pref.useAdditionDialog();

    foreach (QString file, files)
    {
        qDebug("Dropped file %s on download list", qPrintable(file));

        if (misc::isUrl(file))
        {
            Session::instance()->downloadFromUrl(file); // TODO - check it for using only in torrent
            continue;
        }

        // Bitcomet or Magnet link
        if (file.startsWith("bc://bt/", Qt::CaseInsensitive))
        {
            qDebug("Converting bc link to magnet link");
            file = misc::bcLinkToMagnet(file);
        }

        if (file.startsWith("magnet:", Qt::CaseInsensitive))
        {
            if (useTorrentAdditionDialog)
            {
                torrentAdditionDialog *dialog = new torrentAdditionDialog(this);
                dialog->showLoadMagnetURI(file);
            }
            else
            {
                Session::instance()->addLink(file); // TODO - check it fir using only in torrent
            }

            continue;
        }

        // Local file
        if (useTorrentAdditionDialog)
        {
            torrentAdditionDialog *dialog = new torrentAdditionDialog(this);

            if (file.startsWith("file:", Qt::CaseInsensitive))
            file = QUrl(file).toLocalFile();

            dialog->showLoad(file);
        }
        else
        {
            Session::instance()->addLink(file); // TODO - possibly it is torrent only
        }
    }
}*/

// Decode if we accept drag 'n drop or not
/*void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    foreach (const QString &mime, event->mimeData()->formats())
    {
        qDebug("mimeData: %s", mime.toLocal8Bit().data());
    }

    if(event->mimeData()->hasFormat(QString::fromUtf8("text/plain")) ||
       event->mimeData()->hasFormat(QString::fromUtf8("text/uri-list")))
    {
        event->acceptProposedAction();
    }
}*/

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type())
    {
        case QEvent::KeyRelease:
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            int mask = Qt::ControlModifier | Qt::ShiftModifier | Qt::AltModifier;

            if(keyEvent->modifiers() == mask)
            {
            }

            break;
        }
        case QEvent::DragEnter:
        {


            QDragEnterEvent *dragEnterEvent = static_cast<QDragEnterEvent*>(event);
            const QMimeData* mimeData = dragEnterEvent->mimeData();

            // check for our needed mime type, here a file or a list of files
            if (!mimeData->hasUrls())
                break;

            QList<QUrl> urlList = mimeData->urls();

            for (int i = 0; i < urlList.size(); ++i)
            {
                qDebug() << urlList.at(i).toString();
                if (urlList.at(i).toString().endsWith(".torrent"))
                {
                    dragEnterEvent->acceptProposedAction();
                    return true;
                }
            }

            break;
        }
        case QEvent::Drop:
        {
            QDropEvent *dropEvent = static_cast<QDropEvent*>(event);
            const QMimeData* mimeData = dropEvent->mimeData();

            // check for our needed mime type, here a file or a list of files
            if (!mimeData->hasUrls())
                break;

            QStringList pathList;
            QList<QUrl> urlList = mimeData->urls();

            // extract the local paths of the files
            for (int i = 0; i < urlList.size(); ++i)
            {
                pathList.append(urlList.at(i).toLocalFile());
            }

            //addTorrents(pathList);
            return true;
        }
        default:
            break;
    }

    return QObject::eventFilter(watched, event);
}

// As program parameters, we can get paths or urls.
// This function parse the parameters and call
// the right addTorrent function, considering
// the parameter type.
void MainWindow::processParams(const QString& params_str)
{
    processParams(QStringList(params_str));
}

void MainWindow::processParams(const QStringList& params)
{
    Preferences pref;
    qDebug() << "process params: " << params;

    foreach (QString param, params)
    {
        param = param.trimmed();

        if (misc::isUrl(param))
        {
            //Session::instance()->downloadFromUrl(param);
        }
        else
        {
            if (param.startsWith("ed2k://", Qt::CaseInsensitive))
            {
                Session::instance()->addLink(param);
            }
            else if (param.endsWith(".emulecollection"))
            {
                //collection_save_dlg dialog(this, param);
                //dialog.exec();
            }

        }
    }
}

void MainWindow::addTorrent(QString path)
{
    //Session::instance()->addTorrent(path);
}

void MainWindow::optionsSaved()
{
    loadPreferences();
}

// Load program preferences
void MainWindow::loadPreferences(bool configure_session)
{
    if (!systrayIcon) {
        if(!QSystemTrayIcon::isSystemTrayAvailable()) {
            systrayCreator = new QTimer(this);
            connect(systrayCreator, SIGNAL(timeout()), this, SLOT(createSystrayDelayed()));
            systrayCreator->setSingleShot(true);
            systrayCreator->start(2000);
            qDebug("Info: System tray is unavailable, trying again later.");
        }
        else {
            createTrayIcon();
        }
    }
    /*
    Session::instance()->addConsoleMessage(tr("Options were saved successfully."));
    const Preferences pref;
    const bool newSystrayIntegration = pref.systrayIntegration();
    actionLock_mule->setVisible(newSystrayIntegration);

    if(newSystrayIntegration != (systrayIcon != 0))
    {
        if(newSystrayIntegration)
        {
            // create the trayicon
            if(!QSystemTrayIcon::isSystemTrayAvailable())
            {
                if(!configure_session)
                { // Program startup
                    systrayCreator = new QTimer(this);
                    connect(systrayCreator, SIGNAL(timeout()), this, SLOT(createSystrayDelayed()));
                    systrayCreator->setSingleShot(true);
                    systrayCreator->start(2000);
                    qDebug("Info: System tray is unavailable, trying again later.");
                }
                else
                {
                    qDebug("Warning: System tray is unavailable.");
                }
            }
            else
            {
                createTrayIcon();
            }
        }
        else
        {
            // Destroy trayicon
            delete systrayIcon;
            delete myTrayIconMenu;
        }
    }
     // Reload systray icon
    if(newSystrayIntegration && systrayIcon)
    {
        systrayIcon->setIcon(getSystrayIcon());
    }
     // General
    if(pref.isToolbarDisplayed())
    {
        toolBar->setVisible(true);
    }
    else
    {
        // Clear search filter before hiding the top toolbar
        search_filter->clear();
        toolBar->setVisible(false);
    }

    if(pref.preventFromSuspend())
    {
        preventTimer->start(PREVENT_SUSPEND_INTERVAL);
    }
    else
    {
        preventTimer->stop();
        m_pwr->setActivityState(false);
    }

    const uint new_refreshInterval = pref.getRefreshInterval();
    transferList->setRefreshInterval(new_refreshInterval);
    transfersView->setAlternatingRowColors(pref.useAlternatingRowColors());

     // Queueing System
    if(pref.isQueueingSystemEnabled())
    {
        if(!actionDecreasePriority->isVisible())
        {
            transferList->hidePriorityColumn(false);
            actionDecreasePriority->setVisible(true);
            actionIncreasePriority->setVisible(true);
        }
    }
    else
    {
        if(actionDecreasePriority->isVisible())
        {
            transferList->hidePriorityColumn(true);
            actionDecreasePriority->setVisible(false);
            actionIncreasePriority->setVisible(false);
        }
    }

    // Torrent properties
    // Icon provider
#if defined(Q_WS_X11)
    IconProvider::instance()->useSystemIconTheme(pref.useSystemIconTheme());
#endif

    if(configure_session)
        Session::instance()->configureSession();
*/
    qDebug("GUI settings loaded");
}

// Check connection status and display right icon
void MainWindow::updateGUI()
{
    SessionStatus status = Session::instance()->getSessionStatus();

    // update global informations
    if(systrayIcon) {
#if defined(Q_WS_X11) || defined(Q_WS_MAC)
        QString html = "<div style='background-color: #678db2; color: #fff;height: 18px; font-weight: bold; margin-bottom: 5px;'>";
        html += tr("qDonkey");
        html += "</div>";
        html += "<div style='vertical-align: baseline; height: 18px;'>";
        html += "<img src=':icons/search/Download.png'/>&nbsp;" +
        tr("DL speed: %1 KiB/s", "e.g: Download speed: 10 KiB/s")
        .arg(QString::number(status.payload_download_rate/1024., 'f', 1));
        html += "</div>";
        html += "<div style='vertical-align: baseline; height: 18px;'>";
        html += "<img src=':/icons/transfer_list/Upload.png'/>&nbsp;" +
        tr("UP speed: %1 KiB/s", "e.g: Upload speed: 10 KiB/s")
        .arg(QString::number(status.payload_upload_rate/1024., 'f', 1));
        html += "</div>";
#else
        // OSes such as Windows do not support html here
        QString html = tr("DL speed: %1 KiB/s", "e.g: Download speed: 10 KiB/s").arg(QString::number(status.payload_download_rate / 1024., 'f', 1));
        html += "\n";
        html += tr("UP speed: %1 KiB/s", "e.g: Upload speed: 10 KiB/s").arg(QString::number(status.payload_upload_rate / 1024., 'f', 1));
#endif
        systrayIcon->setToolTip(html); // tray icon
    }

    if(Preferences().displaySpeedInTitle()) {
        setWindowTitle(tr("[D: %1/s, U: %2/s] qDonkey %3", "D = Download; U = Upload; %3 is qDonkey version").arg(misc::friendlyUnit(status.payload_download_rate)).arg(misc::friendlyUnit(status.payload_upload_rate)).arg(misc::versionString()));
    } else {
        setWindowTitle("qDonkey");
    }

    statusBar->setUpDown(status.payload_upload_rate, status.payload_download_rate);
    //Session::instance()->playPendingMedia();
}

void MainWindow::showNotificationBaloon(QString title, QString msg) const {
    // forward all notifications to the console
    addConsoleMessage(msg);

#if defined(Q_WS_X11) && defined(QT_DBUS_LIB)
    org::freedesktop::Notifications notifications("org.freedesktop.Notifications",
            "/org/freedesktop/Notifications",
            QDBusConnection::sessionBus());

    if (notifications.isValid())
    {
        QVariantMap hints;
        hints["desktop-entry"] = "qDonkey";
        QDBusPendingReply<uint> reply = notifications.Notify("qDonkey", 0, "qDonkey", title,
                                                             msg, QStringList(), hints, -1);
        reply.waitForFinished();

        if (!reply.isError())
            return;
    }
#endif
    if(systrayIcon && QSystemTrayIcon::supportsMessages())
        systrayIcon->showMessage(title, msg, QSystemTrayIcon::Information, TIME_TRAY_BALLOON);
}

/*****************************************************
 *                                                   *
 *                      Utils                        *
 *                                                   *
 *****************************************************/

void MainWindow::downloadFromURLList(const QStringList& url_list)
{
    /*
    Preferences settings;
    const bool useTorrentAdditionDialog = settings.value(QString::fromUtf8("Preferences/Downloads/AdditionDialog"), true).toBool();

    foreach (QString url, url_list)
    {
        if (url.startsWith("bc://bt/", Qt::CaseInsensitive))
        {
            qDebug("Converting bc link to magnet link");
            url = misc::bcLinkToMagnet(url);
        }

        if (url.startsWith("magnet:", Qt::CaseInsensitive))
        {
            if (useTorrentAdditionDialog)
            {
                torrentAdditionDialog *dialog = new torrentAdditionDialog(this);
                dialog->showLoadMagnetURI(url);
            }
            else
            {
                Session::instance()->addLink(url);
            }
        }
        else
        {
            Session::instance()->downloadFromUrl(url);
        }
    }
    */
}

/*****************************************************
 *                                                   *
 *                     Options                       *
 *                                                   *
 *****************************************************/

void MainWindow::createSystrayDelayed()
{
    static int timeout = 20;
    if(QSystemTrayIcon::isSystemTrayAvailable())
    {
        // Ok, systray integration is now supported
        // Create systray icon
        createTrayIcon();
        delete systrayCreator;
    }
    else
    {
        if(timeout)
        {
            // Retry a bit later
            systrayCreator->start(2000);
            --timeout;
        }
        else
        {
            // Timed out, apparently system really does not
            // support systray icon
            delete systrayCreator;
            // Disable it in program preferences to
            // avoid trying at earch startup
            Preferences().setSysTrayAvailable(false);
        }
    }
}

void MainWindow::createTrayIcon() {
    // Tray icon
    systrayIcon = new QSystemTrayIcon(icon_CurTray, this);
    //systrayIcon->setContextMenu(getTrayIconMenu());
    connect(systrayIcon, SIGNAL(messageClicked()), this, SLOT(balloonClicked()));

    // End of Icon Menu
    connect(systrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(toggleVisibility(QSystemTrayIcon::ActivationReason)));
    systrayIcon->show();
}

void MainWindow::minimizeWindow()
{
    setWindowState(windowState() ^ Qt::WindowMinimized);
}

void MainWindow::addConsoleMessage(const QString& msg, QColor color /*=QApplication::palette().color(QPalette::WindowText)*/) const { qDebug() << msg; }


void MainWindow::on_actionTransfers_triggered() {
    stackedWidget->setCurrentIndex(wTransfer);
}

void MainWindow::on_actionSearch_triggered() {
    stackedWidget->setCurrentIndex(wSearch);
}

void MainWindow::on_actionOptions_triggered() {
    stackedWidget->setCurrentIndex(wPreferences);
}

void MainWindow::on_actionConnect_triggered() {
    //QMessageBox confirmBox(QMessageBox::Question, tr("Server connection"), tr("Do you want to break network connection?"), QMessageBox::NoButton, this);
    //confirmBox.addButton(tr("No"), QMessageBox::NoRole);
    //QPushButton *yesBtn = confirmBox.addButton(tr("Yes"), QMessageBox::YesRole);
    //confirmBox.setDefaultButton(yesBtn);

    if (!Session::instance()->isServerConnected()) {
        actionConnect->setIcon(QIcon(res::toolbarConnecting()));
        actionConnect->setText(tr("Cancel"));
        Session::instance()->startServerConnection();
    } else {
        Session::instance()->stopServerConnection();
    }
}

void MainWindow::handleServerNameResolved(QString name) {

}

void MainWindow::handleServerConnectionInitialized(quint32 client_id, quint32 tcp_flags, quint32 aux_port) {
    Q_UNUSED(tcp_flags);
    Q_UNUSED(aux_port);
    actionConnect->setIcon(QIcon(res::toolbarConnected()));
    actionConnect->setText(tr("Disconnect"));
    QString log_msg("Client ID: ");
    QString id;
    id.setNum(client_id);
    log_msg += id;
    statusBar->setStatusMsg(log_msg);
    statusBar->setConnected(true);
    icon_CurTray = icon_TrayDisconn;
    if (systrayIcon) systrayIcon->setIcon(icon_CurTray);
}

void MainWindow::handleServerConnectionClosed(QString) {
    actionConnect->setIcon(QIcon(res::toolbarDisconnected()));
    actionConnect->setText(tr("Connect"));
    icon_CurTray = icon_TrayDisconn;
    if (systrayIcon) systrayIcon->setIcon(icon_CurTray);
    statusBar->setConnected(false);
}

void MainWindow::handleServerStatus(int files, int users) {
    statusBar->setServerInfo(files, users);
}

void MainWindow::handleServerMessage(QString) {

}

void MainWindow::handleServerIdentity(QString,QString) {

}
