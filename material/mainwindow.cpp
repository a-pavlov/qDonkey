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
#include "kadnodes.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QKeyEvent>
#include <QTimer>

MainWindow::MainWindow(QObject* parent) : QObject(parent) {
    pref.reset(new Preferences);
    smodel = new ServerModel(this);
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
    engine->rootContext()->setContextProperty("peerModel", pmodel);
    engine->rootContext()->setContextProperty("kadModel", knodes);
#ifdef IS74
    engine->load(QUrl(QStringLiteral("qrc:/qml/Donkey.qml")));
#else
    engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
#endif

    Q_ASSERT(engine->rootObjects().size() == 1);
    connect(engine->rootObjects()[0], SIGNAL(init_dir_accepted()), this, SLOT(onInitAccepted()));
    connect(engine->rootObjects()[0], SIGNAL(init_dir_rejected()), this, SLOT(onInitRejected()));

    //Session::instance()->start();
    Session::instance()->loadDirectory(pref.data()->inputDir());
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

void MainWindow::onPlayTimeout() {
    Session::instance()->playPendingMedia();
}
