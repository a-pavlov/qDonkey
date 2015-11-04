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
    QScopedPointer<Preferences>          pref;
public:
    explicit MainWindow(QObject* parent = 0);
    ~MainWindow();
private slots:
    void onIncomingDirChanged(QString);
    void onPreferencesChanged();

};


#endif //__MAIN_WINDOW__
