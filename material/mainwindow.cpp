#include "mainwindow.h"
#include "server_model.h"
#include "search_model.h"
#include "qed2kserver.h"
#include "qed2ksession.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>

MainWindow::MainWindow(QObject* parent) : QObject(parent) {
    smodel = new ServerModel(this);
#ifdef IS74
    smodel->add(QED2KServer("is74", "emule.is74.ru", 4661));
    smodel->update("is74");
#endif
    foreach(const QED2KServer s, fromServersMet("./server.met")) {
        smodel->add(s);
    }
    searchmodel = new SearchModel(this);

    connect(Session::instance(), SIGNAL(serverConnectionInitialized(QString,quint32,quint32,quint32)),
            smodel, SLOT(on_serverConnectionInitialized(QString,quint32,quint32,quint32)));
    connect(Session::instance(), SIGNAL(serverConnectionClosed(QString,QString)), smodel,
            SLOT(on_serverConnectionClosed(QString,QString)));
    connect(Session::instance(), SIGNAL(searchResult(libed2k::net_identifier,QString,QList<QED2KSearchResultEntry>,bool)),
            searchmodel, SLOT(on_searchResult(libed2k::net_identifier,QString,QList<QED2KSearchResultEntry>,bool)));
    engine = new QQmlApplicationEngine(this);
    engine->rootContext()->setContextProperty("serverModel", smodel);
    engine->rootContext()->setContextProperty("searchModel", searchmodel);
    engine->rootContext()->setContextProperty("session", Session::instance());

#ifdef IS74
    engine->load(QUrl(QStringLiteral("qrc:/qml/Donkey.qml")));
#else
    engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
#endif
}

