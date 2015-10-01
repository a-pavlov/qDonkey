#include "mainwindow.h"
#include "server_model.h"
#include "qed2kserver.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>

MainWindow::MainWindow(QObject* parent) : QObject(parent) {
    smodel = new ServerModel(this);
    foreach(const QED2KServer s, fromServersMet("./server.met")) {
        smodel->add(s);
    }

    engine = new QQmlApplicationEngine(this);
    engine->rootContext()->setContextProperty("serverModel", smodel);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
}
