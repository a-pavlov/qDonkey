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
#include <QQmlApplicationEngine>
#include <QQmlContext>

MainWindow::MainWindow(QObject* parent) : QObject(parent) {
    pref.reset(new Preferences);
    connect(pref.data(), SIGNAL(inputDirChanged(QString)), this, SLOT(onIncomingDirChanged(QString)));
    connect(pref.data(), SIGNAL(preferencesChanged()), this, SLOT(onPreferencesChanged()));
    smodel = new ServerModel(this);
#ifdef IS74
    smodel->add(QED2KServer("is74", "emule.is74.ru", 4661));
    //smodel->update("is74", "emule.is74.ru", 4661);
#endif
    foreach(const QED2KServer s, fromServersMet("./server.met")) {
        smodel->add(s);
    }
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
    engine = new QQmlApplicationEngine(this);
    TransferModelItemEnum::qmlRegister();
    engine->rootContext()->setContextProperty("serverModel", smodel);
    engine->rootContext()->setContextProperty("searchModel", searchmodel);
    engine->rootContext()->setContextProperty("searchFPModel", searchFilterProxyModel);
    engine->rootContext()->setContextProperty("transferModel", transferProxy);
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
}

MainWindow::~MainWindow() {
    Session::instance()->drop();
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
    qDebug() << "sort column " << transferProxy->sortColumn();
    transferProxy->sort(0);
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
