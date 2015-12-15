#include "mainwindow.h"
#include "server_model.h"
#include "search_model.h"
#include "transfer_model.h"
#include "transfermodel_item.h"
#include "transferdetailsmodel.h"
#include "transfersortfiltermodel.h"
#include "peermodel.h"
#include "preferences.h"
#include "../src/search/search_widget_fp_model.h"
#include "qed2kserver.h"
#include "qed2ksession.h"
#include "notificationclient.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QKeyEvent>
#include <QTimer>
//#include "adctl.h"

MainWindow::MainWindow(QObject* parent) : QObject(parent) {
    pref.reset(new Preferences);
    Session::instance()->start();
    Session::instance()->loadDirectory(pref.data()->inputDir());
    connect(pref.data(), SIGNAL(inputDirChanged(QString)), this, SLOT(onIncomingDirChanged(QString)));
    connect(pref.data(), SIGNAL(preferencesChanged()), this, SLOT(onPreferencesChanged()));
    smodel = new ServerModel(this);
    notificationClient = new NotificationClient(this);

    smodel->load();

    searchmodel = new SearchModel(this);
    searchFilterProxyModel = new SWSortFilterProxyModel(this);
    searchFilterProxyModel->setDynamicSortFilter(false);
    searchFilterProxyModel->setSourceModel(searchmodel);

    transferModel = new TransferModel(this);
    transferDetails = new TransferDetailsModel(this);
    transferDetails->setSourceModel(transferModel);

    transferProxy = new TransferSortFilterModel(this);
    transferProxy->setSourceModel(transferModel);
    transferProxy->setDynamicSortFilter(true);
    transferProxy->setFilterRole(TransferModelItem::TM_AGE);
    transferProxy->setFilterFixedString(pref.data()->showAllTransfers()?"":"N");
    transferProxy->setSortRole(TransferModelItem::TM_NAME);
    transferProxy->sort(0);

    pmodel = new PeerModel(this);

    connect(Session::instance(), SIGNAL(serverConnectionInitialized(QString,QString, int, quint32,quint32,quint32)),
            smodel, SLOT(on_serverConnectionInitialized(QString,QString,int,quint32,quint32,quint32)));
    connect(Session::instance(), SIGNAL(serverConnectionClosed(QString,QString,int,QString)), smodel,
            SLOT(on_serverConnectionClosed(QString,QString,int,QString)));
    connect(Session::instance(), SIGNAL(searchResult(libed2k::net_identifier,QString,QList<QED2KSearchResultEntry>,bool)),
            searchmodel, SLOT(on_searchResult(libed2k::net_identifier,QString,QList<QED2KSearchResultEntry>,bool)));
    connect(pref.data(), SIGNAL(showAllTransfersChanged(bool)), this, SLOT(onShowAllTransfersChanged(bool)));

    connect(Session::instance(), SIGNAL(serverConnectionInitialized(QString,QString,int,quint32,quint32,quint32)),
            this, SLOT(onServerConnectionInitialized(QString,QString,int,quint32,quint32,quint32)));
    connect(Session::instance(), SIGNAL(serverConnectionClosed(QString,QString,int,QString)), this,
            SLOT(onServerConnectionClosed(QString,QString,int,QString)));

    connect(Session::instance(), SIGNAL(transferAdded(QED2KHandle)), SLOT(addedTransfer(QED2KHandle)));
    connect(Session::instance(), SIGNAL(transferFinished(QED2KHandle)), SLOT(finishedTransfer(QED2KHandle)));
    connect(Session::instance(), SIGNAL(fileIOError(QString, QString)), SLOT(fileError(QString, QString)));
    engine = new QQmlApplicationEngine(this);
    TransferModelItemEnum::qmlRegister();
    /*adctl = new AdCtl(this);
    adctl->setAdMobId("ca-app-pub-1671474838801728/5245648299");
    adctl->setStartAdId("ca-app-pub-1671474838801728/3768915093");
    adctl->setAdMobBannerEnabled(true);
    adctl->setAdMobIinterstitialEnabled(true);
    adctl->setStartAdBannerEnabled(true);
    adctl->init();
    */
    engine->rootContext()->setContextProperty("serverModel", smodel);
    engine->rootContext()->setContextProperty("searchModel", searchmodel);
    engine->rootContext()->setContextProperty("searchFPModel", searchFilterProxyModel);
    engine->rootContext()->setContextProperty("transferProxy", transferProxy);
    engine->rootContext()->setContextProperty("transferModel", transferModel);
    engine->rootContext()->setContextProperty("transferDetails", transferDetails);
    engine->rootContext()->setContextProperty("session", Session::instance());
    engine->rootContext()->setContextProperty("pref", pref.data());
    engine->rootContext()->setContextProperty("peerModel", pmodel);
    //engine->rootContext()->setContextProperty("adctl", adctl);
//#ifdef IS74
    engine->load(QUrl(QStringLiteral("qrc:/qml/Donkey.qml")));
//#else
    //engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
//#endif

    restoreLastServerConnection();

    playTimer = new QTimer(this);
    connect(playTimer, SIGNAL(timeout()), this, SLOT(onPlayTimeout()));
    playTimer->start(2500);
}

MainWindow::~MainWindow() {
    smodel->save();
    Session::instance()->drop();
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Exit) {
        qDebug() << "exit key pressed";
    }

    event->setAccepted(true);
}

void MainWindow::onIncomingDirChanged(QString dir) {
    qDebug() << "user set incoming dir to: " << dir;
    if (dir.isEmpty()) qApp->quit();
    if (misc::prepareInputDirectory(dir)) Session::instance()->loadDirectory(dir);
    else qDebug() << "prepare input directory failed";
}

void MainWindow::onPreferencesChanged() {
    qDebug() << "preferences changed";
    Preferences pref;
    Session::instance()->configureSession();
    if (!misc::prepareInputDirectory(pref.inputDir())) qDebug() << "preparation input dir failed";
}

void MainWindow::onServerConnectionInitialized(QString alias, QString host,int port,quint32,quint32,quint32) {
    Preferences pref;
    pref.beginGroup("LastConnectedServer");
    pref.setValue("Alias", alias);
    pref.setValue("Host", host);
    pref.setValue("Port", port);
    pref.endGroup();
}

void MainWindow::onServerConnectionClosed(QString,QString,int,QString) {
    Preferences pref;
    pref.beginGroup("LastConnectedServer");
    pref.remove("Alias");
    pref.remove("Host");
    pref.remove("Port");
    pref.endGroup();
}

void MainWindow::onShowAllTransfersChanged(bool showAll) {
    qDebug() << "switch show all transfers " << showAll;
    transferProxy->setFilterFixedString(showAll?"":"N");
}

void MainWindow::restoreLastServerConnection() {
    Preferences pref;
    pref.beginGroup("LastConnectedServer");
    if (pref.contains("Alias") && pref.contains("Host") && pref.contains("Port")) {
        qDebug() << "restore server connection "
                 << pref.value("Alias", "").toString() << " "
                 << pref.value("Host", "").toString() << " "
                 <<  pref.value("Port", -1).toInt();
        smodel->update(pref.value("Alias", "").toString(), pref.value("Host", "").toString(), pref.value("Port", -1).toInt());
    }
    pref.endGroup();
}

// called when a transfer has started
void MainWindow::addedTransfer(const QED2KHandle& h) {
    //showNotificationBaloon(tr("Download starting"), tr("%1 has started downloading.", "e.g: xxx.avi has started downloading.").arg(h.name()));
    qDebug() << "started transssfer " << h.name();
    searchFilterProxyModel->invalidate();
    notificationClient->setNotification(tr("%1 has started downloading").arg(h.name()));
}

// called when a transfer has finished
void MainWindow::finishedTransfer(const QED2KHandle& h) {
    //showNotificationBaloon(tr("Download completion"), tr("%1 has finished downloading.", "e.g: xxx.avi has finished downloading.").arg(h.name()));
    qDebug() << "finished transfer "   << h.name();
    notificationClient->setNotification(tr("%1 has finished downloading").arg(h.name()));
}

// Notification when disk is full and other disk errors
void MainWindow::fileError(QString filename, QString msg) {
    notificationClient->setNotification(tr("I/O error on %1: %2").arg(filename).arg(msg));
}


void MainWindow::onPlayTimeout() {
    Session::instance()->playPendingMedia();
}
