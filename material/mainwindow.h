#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <QObject>
#include "preferences.h"

class QQmlApplicationEngine;
class ServerModel;
class SearchModel;
class SWSortFilterProxyModel;
class TransferModel;
class TransferDetailsModel;
class TransferSortFilterModel;
class QED2KHandle;
class NotificationClient;

class MainWindow:  public QObject {
    Q_OBJECT
private:
    QQmlApplicationEngine*  engine;
    QObject*                mainWindow;
    ServerModel*            smodel;
    SearchModel*            searchmodel;
    SWSortFilterProxyModel* searchFilterProxyModel;
    TransferModel*          transferModel;
    TransferDetailsModel*   transferDetails;
    TransferSortFilterModel* transferProxy;
    QScopedPointer<Preferences>          pref;
    NotificationClient*     notificationClient;

    void restoreLastServerConnection();
public:
    explicit MainWindow(QObject* parent = 0);
    ~MainWindow();
private slots:
    void onIncomingDirChanged(QString);
    void onPreferencesChanged();
    void onServerConnectionInitialized(QString,QString,int,quint32,quint32,quint32);
    void onServerConnectionClosed(QString,QString,int,QString);
    void onShowAllTransfersChanged(bool);
    void addedTransfer(const QED2KHandle&);
    void finishedTransfer(const QED2KHandle&);
    void fileError(QString, QString);
};


#endif //__MAIN_WINDOW__
