#include <QApplication>
#include "mainwindow.h"
#include "qed2ksession.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Session::instance()->start();
    MainWindow mw;
    return app.exec();
}

