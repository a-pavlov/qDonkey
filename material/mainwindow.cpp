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
#include "kadnodes.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QKeyEvent>
#include <QTimer>
//#include "adctl.h"

MainWindow::MainWindow(QObject* parent) : QObject(parent) {
    suspended = false;
    pref.reset(new Preferences);

    Session::instance()->start();

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

    knodes = new KadNodes(this);

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
    engine->rootContext()->setContextProperty("kadModel", knodes);

    //engine->rootContext()->setContextProperty("adctl", adctl);
//#ifdef IS74    

    engine->load(QUrl(QStringLiteral("qrc:/qml/Donkey.qml")));
//#else
    //engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
//#endif

    Q_ASSERT(engine->rootObjects().size() == 1);
    connect(engine->rootObjects()[0], SIGNAL(init_dir_accepted()), this, SLOT(onInitAccepted()));
    connect(engine->rootObjects()[0], SIGNAL(init_dir_rejected()), this, SLOT(onInitRejected()));
    connect(engine->rootObjects()[0], SIGNAL(switch_session()), this, SLOT(onSwitchSession()));

    //Session::instance()->start();
    Session::instance()->loadDirectory(pref.data()->inputDir());
    restoreLastServerConnection();

    playTimer = new QTimer(this);
    connect(playTimer, SIGNAL(timeout()), this, SLOT(onPlayTimeout()));
    playTimer->start(2500);

    connect(QApplication::instance(), SIGNAL(applicationStateChanged(Qt::ApplicationState)), this, SLOT(onApplicationStateChanged(Qt::ApplicationState)));
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

void MainWindow::onInitAccepted() {
    qDebug() << "init directory accepted";
    //Preferences pref;
    //if (!Session::instance()->loadDirectory(pref.inputDir())) qDebug() << "prepare input directory failed";
}

void MainWindow::onInitRejected() {
    qDebug() << "init rejected, exit ";
    qApp->quit();
}

void MainWindow::onServerConnectionInitialized(QString alias, QString host,int port,quint32 client_id, quint32,quint32) {
    Preferences pref;
    pref.beginGroup("LastConnectedServer");
    pref.setValue("Alias", alias);
    pref.setValue("Host", host);
    pref.setValue("Port", port);
    pref.endGroup();
    smodel->setClientId(alias, host, port, client_id);
}

void MainWindow::onServerConnectionClosed(QString alias, QString host, int port,QString) {
    Preferences pref;
    pref.beginGroup("LastConnectedServer");
    pref.remove("Alias");
    pref.remove("Host");
    pref.remove("Port");
    pref.endGroup();
    smodel->setClientId(alias, host, port, 0);
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

void MainWindow::onApplicationStateChanged(Qt::ApplicationState state) {
    qDebug() << Q_FUNC_INFO << " " << state;
    Preferences pref;
    switch(state){
    case Qt::ApplicationSuspended:
        qDebug() << "stop session";
        transferModel->activateRefresh(false);
        playTimer->stop();
        smodel->save();
        smodel->clean();
        Session::instance()->stop();
        suspended = true;
        break;
    case Qt::ApplicationActive:
    {
        if (suspended) {
            qDebug() << "application previously suspended, activate";
            Session::instance()->start();
            //qSleep(2000);
            if (pref.getKad()) Session::instance()->startKad();
            Session::instance()->loadDirectory(pref.inputDir());
            smodel->load();
            restoreLastServerConnection();
            suspended = false;
            transferModel->activateRefresh(true);
            playTimer->start();
        }
        break;
    }
    case Qt::ApplicationInactive:
    default:
        break;
    }
}
