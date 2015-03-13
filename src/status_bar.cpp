#include "status_bar.h"
#include "misc.h"
#include "res.h"
#include <QStatusBar>
#include <QDesktopServices>
#include <QTimer>
#include <QDebug>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkAccessManager>
#include <QToolButton>
#include <QFile>
#include <QTextStream>

status_bar::status_bar(QWidget *parent, QStatusBar *bar)
    :
      QWidget(parent),
      m_bar(bar),
      imgConnected(res::statusbarConnected()),
      imgDisconnected(res::statusbarDisconnected()),
      imgMsg1(res::message()),
      imgMsg2(res::messagePending()),
      imgEmpty(res::noMessage())
{
    setupUi(this);

    m_bar->addWidget(this, 1);
    reset();
}

status_bar::~status_bar()
{
}

void status_bar::browseGooglePlus()
{
    QDesktopServices::openUrl(QUrl("https://plus.google.com/+emulecom"));
}

void status_bar::setConnected(bool conn)
{
    if (conn)
    {
        labelNetImg->setPixmap(imgConnected);
        labelNet->setText(tr("eD2K:Connected"));
        labelNet->setToolTip(tr("eD2K:Connected"));
    }
    else
    {
        labelNetImg->setPixmap(imgDisconnected);
        labelNet->setText(tr("eD2K:Disconnected"));
        labelNet->setToolTip(tr("eD2K:Disconnected"));
    }
}

void status_bar::setUpDown(unsigned long nUp, unsigned long nDown)
{
    /*QIcon icon = nUp > 0 ? (nDown > 0 ? Up1Down1 : Up1Down0) : (nDown > 0 ? Up0Down1 : Up0Down0);
    labelSpeedImg->setPixmap(icon.pixmap(16, 16));*/

    QString text = tr("Up:") + QString(" %1/%2, ").arg(misc::friendlyUnit(nUp)).arg(tr("s")) +
                   tr("Down:") + QString(" %1/%2").arg(misc::friendlyUnit(nDown)).arg(tr("s"));

    labelSpeed->setText(text);
    labelSpeed->setToolTip(text);
}

void status_bar::setServerInfo(unsigned long nFiles, unsigned long nClients)
{
    QString strClients;
    QString strFiles;

    strClients.setNum(nClients);
    strFiles.setNum(nFiles);

    QString text = tr("Clients: ") + strClients + tr("|Files: ") + strFiles;
    labelInfo->setText(text);
    labelInfo->setToolTip(text);
}

void status_bar::setStatusMsg(QString strMsg)
{
    labelServer->setText(strMsg);
    labelServer->setToolTip(strMsg);
}

void status_bar::reset()
{
    setConnected(false);
    setUpDown(0, 0);
    setServerInfo(0, 0);
}


void status_bar::doubleClickNewMsg()
{
    emit stopMessageNotification();
}
