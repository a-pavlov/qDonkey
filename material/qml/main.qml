import QtQuick 2.2
import Material 0.1
import Material.ListItems 0.1 as ListItem
import ru.forsk.adctl 1.0

ApplicationWindow {
    id: qDonkey
    title: "qDonkey"
    width:  400
    visible: true

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }

    AdCtl {
        id: adCtl

        //manage enabled components
        adMobBannerEnabled: true
        adMobIinterstitialEnabled: true
        startAdBannerEnabled: false
        gAnalyticsEnabled: false

        //set ids
        adMobId: "ca-app-pub-1671474838801728/5245648299"
        startAdId: "ca-app-pub-1671474838801728/3768915093"
        gAnalyticsId: "YOUR_GANALYTICS_TRACKING_ID"

        //Start positions for banners.
        adMobBannerPosition: Qt.point(0,-500)
        startAdBannerPosition: Qt.point(0,-500)
        startAdBannerSize: Qt.size(250, 50)

        //when StartAd.mobi baners is showed we can to reposition it
        onStartAdBannerShowed: {
            console.log("onStartAdBannerShowed");
            startAdBannerPosition = Qt.point(0,
                                     (appWindow.height - adCtl.startAdBannerHeight * 1.3))
        }

        //when AdMob baners is showed we can to reposition it
        onAdMobBannerShowed: {
            console.log("onAdMobBannerShowed");
            adMobBannerPosition = Qt.point((appWindow.width - adCtl.adMobBannerWidth) * 0.5,
                                     (appWindow.height - adCtl.adMobBannerHeight * 1.5 - 200))
        }

        //When all variables are setted, we can to initialize our code
        Component.onCompleted: {
            adCtl.init();
            adCtl.signInGPGS();
        }
    }

    Rectangle {
        id: root

        anchors.fill: parent
        anchors.bottomMargin: adCtl.startAdBannerHeight
        Component.onCompleted: { adCtl.sendGaAppView("MainWindow"); }
    }
}
