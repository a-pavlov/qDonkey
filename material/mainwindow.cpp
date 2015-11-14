#include "mainwindow.h"
#include "server_model.h"
#include "search_model.h"
#include "transfer_model.h"
#include "transfermodel_item.h"
#include "transferdetailsmodel.h"
#include "transfersortfiltermodel.h"
#include "preferences.h"
#include "../src/search/search_widget_fp_model.h"
#include "qed2kserver.h"
#include "qed2ksession.h"
#include "notificationclient.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QKeyEvent>
#include <QTimer>

MainWindow::MainWindow(QObject* parent) : QObject(parent) {
    pref.reset(new Preferences);
    connect(pref.data(), SIGNAL(inputDirChanged(QString)), this, SLOT(onIncomingDirChanged(QString)));
    connect(pref.data(), SIGNAL(preferencesChanged()), this, SLOT(onPreferencesChanged()));
    smodel = new ServerModel(this);
#ifdef IS74
    smodel->add(QED2KServer("is74", "emule.is74.ru", 4661));
#endif

    smodel->add(QED2KServer("TV Underground", "176.103.48.36", 4184));
    smodel->add(QED2KServer("eMule Security No1", "91.200.42.46" ,1176));
    smodel->add(QED2KServer("eMule Security No2", "91.200.42.47", 3883));
    smodel->add(QED2KServer("eMule Security No3", "91.200.42.119", 9939));
    smodel->add(QED2KServer("eMule Security No4", "77.120.115.66", 5041));
    smodel->add(QED2KServer("eMule oVPN.to Anonymous ed2k-Server", "213.163.71.135", 4242));
    smodel->add(QED2KServer("emule lover QQ Qun 212431292", "27.152.28.252", 4242));
    smodel->add(QED2KServer("PEERATES.NET", "195.154.83.5" ,7111));
    smodel->add(QED2KServer("PeerBooter", "212.83.184.152", 7111));
    smodel->add(QED2KServer("!! www.Sharing-Devils.org No.1 ...", "195.154.109.229" ,4232));

    foreach(const QED2KServer s, fromServersMet("./server.met")) {
        smodel->add(s);
    }

    notificationClient = new NotificationClient(this);
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
    engine->rootContext()->setContextProperty("serverModel", smodel);
    engine->rootContext()->setContextProperty("searchModel", searchmodel);
    engine->rootContext()->setContextProperty("searchFPModel", searchFilterProxyModel);
    engine->rootContext()->setContextProperty("transferProxy", transferProxy);
    engine->rootContext()->setContextProperty("transferModel", transferModel);
    engine->rootContext()->setContextProperty("transferDetails", transferDetails);
    engine->rootContext()->setContextProperty("session", Session::instance());
    engine->rootContext()->setContextProperty("pref", pref.data());
#ifdef IS74
    engine->load(QUrl(QStringLiteral("qrc:/qml/Donkey.qml")));
#else
    engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
#endif

    Session::instance()->start();
    Session::instance()->loadDirectory(pref.data()->inputDir());
    restoreLastServerConnection();

    playTimer = new QTimer(this);
    connect(playTimer, SIGNAL(timeout()), this, SLOT(onPlayTimeout()));
    playTimer->start(2500);
}

MainWindow::~MainWindow() {
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
