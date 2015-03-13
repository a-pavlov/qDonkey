#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include <QWidget>
#include <QUrl>
#include "ui_status_bar.h"

QT_BEGIN_NAMESPACE
class QStatusBar;
class QWebView;
QT_END_NAMESPACE

class status_bar : public QWidget, public Ui::status_bar
{
    Q_OBJECT
    QStatusBar* m_bar;
    QPixmap  imgConnected;
    QPixmap imgDisconnected;
    QPixmap Up0Down0;
    QPixmap Up1Down0;
    QPixmap Up0Down1;
    QPixmap Up1Down1;
    QPixmap imgMsg1;
    QPixmap imgMsg2;
    QPixmap imgEmpty;

public:
    status_bar(QWidget *parent, QStatusBar *bar);
    ~status_bar();
    void setConnected(bool conn);
    void setUpDown(unsigned long nUp, unsigned long nDown);
    void setServerInfo(unsigned long nFiles, unsigned long nClients);
    void setStatusMsg(QString strMsg);
    void reset();


private slots:
    void browseGooglePlus();
    void doubleClickNewMsg();

signals:
    void stopMessageNotification();
};

#endif // STATUS_BAR_H
