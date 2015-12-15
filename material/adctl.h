#ifndef ADMOBBANNERCTL_H
#define ADMOBBANNERCTL_H

#include <QObject>
class QAndroidJniObject;

class AdMobCtrl : public QObject {
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
    AdMobCtrl();
    ~AdMobCtrl();
    Q_PROPERTY(bool adLoaded READ readAdLoaded)
    Q_PROPERTY(bool valid READ readHasActivity)
    Q_PROPERTY(int adHeight READ readAdHeight)
    Q_PROPERTY(QString link READ getLink)
    Q_INVOKABLE bool readAdLoaded() const;
    Q_INVOKABLE void adShow();
    Q_INVOKABLE void adHide();
    Q_INVOKABLE void adSetPos(int x, int y);
    Q_INVOKABLE int readAdHeight() const;
    Q_INVOKABLE void interstitialShow();
    Q_INVOKABLE QString getLink();
    bool readHasActivity() const { return m_Activity != NULL; }
};

#endif // ADMOBBANNERCTL_H
