#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <QObject>

class QQmlApplicationEngine;
class ServerModel;
class SearchModel;
class SWSortFilterProxyModel;
class TransferModel;


class MainWindow:  public QObject {
    Q_OBJECT
private:
    QQmlApplicationEngine*  engine;
    QObject*                mainWindow;
    ServerModel*            smodel;
    SearchModel*            searchmodel;
    SWSortFilterProxyModel* searchFilterProxyModel;
    TransferModel*          transferModel;
public:
    explicit MainWindow(QObject* parent = 0);

};


#endif //__MAIN_WINDOW__
