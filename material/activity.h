#ifndef __ACTIVITY__
#define __ACTIVITY__

#include <QObject>
class QAndroidJniObject;

class Activity : public QObject {
    Q_OBJECT
private:
    //jni
#ifdef Q_OS_ANDROID
    QAndroidJniObject *m_Activity;
#endif
    float m_dp;
    float m_pt;
    float m_mm;
public:
    Activity();
    ~Activity();
    Q_PROPERTY(QString link READ getLink)
    Q_INVOKABLE QString getLink();
};

#endif // __ACTIVITY__
