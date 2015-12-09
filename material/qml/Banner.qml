import QtQuick 2.0
import Material 0.1

Item {
    id: bannerPlaceHolder
    height: adCtl.adMobBannerHeight
    anchors.margins: Units.dp(16)
    visible: false

    Connections {
        target: adCtl
        onAdMobBannerShowed: {
            console.log("adctl banner showed " + height)
            visible = true
        }
    }
}
