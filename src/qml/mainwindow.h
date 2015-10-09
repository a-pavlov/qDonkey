#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <QObject>

class QQmlApplicationEngine;
class ServerModel;
class SearchModel;


class MainWindow:  public QObject {
    Q_OBJECT
private:
    QQmlApplicationEngine*  engine;
    QObject*                mainWindow;
    ServerModel*            smodel;
    SearchModel*            searchmodel;
public:
    explicit MainWindow(QObject* parent = 0);

};


#endif //__MAIN_WINDOW__
