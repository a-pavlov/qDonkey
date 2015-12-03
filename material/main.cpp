#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "mainwindow.h"
#include "qed2ksession.h"
#include "preferences.h"
#include "adctl.h"
#include <QtQml>

#ifdef Q_OS_WIN
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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
    //qInstallMessageHandler(customMessageHandler);
#endif

    Preferences pref;
    // Load translation
    QString locale = pref.locale();
    //QTranslator qtTranslator;
    QTranslator translator;

    if(locale.isEmpty())
    {
        locale = QLocale::system().name();
        qDebug("locale is empty, use %s", qPrintable(locale));

        if(locale.isEmpty())
        {
            locale = "en";
        }
    }

    locale.remove(2, locale.length() - 2);
    qDebug("Program locale: %s", qPrintable(locale));
    setlocale(LC_CTYPE, ""); // TODO - need attention
/*
    if(qtTranslator.load(QString::fromUtf8("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
    {
        qDebug("Qt %s locale recognized, using translation.", qPrintable(locale));
    }
    else
    {
        qDebug("Qt %s locale unrecognized, using default (en_GB).", qPrintable(locale));
    }

    app.installTranslator(&qtTranslator);
    */

    if(translator.load(QString::fromUtf8(":/lang/qDonkey_") + locale)) {
        qDebug("%s locale recognized, using translation.", qPrintable(locale));
        pref.setLocale(locale);
        app.installTranslator(&translator);
    }
    else {
        qDebug("%s locale unrecognized, using default (en).", qPrintable(locale));
    }


    if(locale.startsWith("ar"))
    {
        qDebug("Right to Left mode");
        app.setLayoutDirection(Qt::RightToLeft);
    }
    else app.setLayoutDirection(Qt::LeftToRight);
    qmlRegisterType<AdMobCtrl>("org.dkfsoft.admobctrl", 1, 0, "AdMobCtrl");

    Session::instance()->start();
    MainWindow mw;
    return app.exec();

}
