#include "activity.h"
#include <QtQml>
#include <QDebug>
#include <QScreen>
#include <QApplication>

#if (__ANDROID_API__ >= 9)

#include <android/api-level.h>
#include <QAndroidJniObject>
#include <qpa/qplatformnativeinterface.h>
#include <QtAndroidExtras/QAndroidJniObject>

#endif

Activity::Activity() : m_Activity(NULL) {
    QPlatformNativeInterface* interface = QApplication::platformNativeInterface();
    jobject activity = (jobject)interface->nativeResourceForIntegration("QtActivity");
    if (activity) m_Activity = new QAndroidJniObject(activity);
    qDebug() << (activity?"activity created":"activity is null");

    //mm and dp
#ifdef Q_OS_ANDROID
    //  BUG with dpi on some androids: https://bugreports.qt-project.org/browse/QTBUG-35701
    //  Workaround:
    QAndroidJniObject qtActivity = QAndroidJniObject::callStaticObjectMethod("org/qtproject/qt5/android/QtNative", "activity", "()Landroid/app/Activity;");
    QAndroidJniObject resources = qtActivity.callObjectMethod("getResources", "()Landroid/content/res/Resources;");
    QAndroidJniObject displayMetrics = resources.callObjectMethod("getDisplayMetrics", "()Landroid/util/DisplayMetrics;");
    int density = displayMetrics.getField<int>("densityDpi");
#else
    QScreen *screen = qApp->primaryScreen();
    float density = screen->physicalDotsPerInch();
#endif
    m_mm = density / 25.4;
    m_pt =  1;

    double scale = density < 180 ? 1 :
                                   density < 270 ? 1.5 :
                                                   density < 360 ? 2 : 3;
    m_dp = scale;
}

Activity::~Activity() {
    delete m_Activity;
}

QString Activity::getLink() {
    if (m_Activity){
        QAndroidJniObject res = m_Activity->callObjectMethod<jstring>("getLink");
        return res.toString();
    }

    return QString();
}
