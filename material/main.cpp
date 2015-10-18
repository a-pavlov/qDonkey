#include <QApplication>
#include "mainwindow.h"
#include "qed2ksession.h"

#ifdef Q_OS_WIN
void customMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;

    switch (type)
    {
        case QtDebugMsg:
        txt = QString("Debug: %1").arg(msg);
        break;

        case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        break;
        case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        break;
        case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        break;
        default:
        abort();

    }

    QFile outFile("debuglog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

#endif

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
#ifdef Q_OS_WIN
    qInstallMsgHandler(customMessageHandler);
#endif
    Session::instance()->start();
    MainWindow mw;
    return app.exec();

}
