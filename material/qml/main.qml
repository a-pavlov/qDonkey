import QtQuick 2.2
import Material 0.2
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem
import org.dkfsoft.admobctrl 1.0

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

    property string aLoaded: "false"
    property int posY: 0

    AdMobCtrl {
        id: adMob
    }

    Timer {
        interval: 5000
        running: true
        repeat: true
        onTriggered: {
            aLoaded = adMob.adLoaded?"true":"false"
            console.log("AdMob loaded " + aLoaded)
            if (aLoaded) {
                adMob.adSetPos(0, 0)
                placeholder.height = adMob.adHeight
                placeholder.visible = true
                adMob.adShow()
                stop()
            }
        }
    }

    ColumnLayout {

        Item {
            id: placeholder
            height: 0
            visible: false
        }

        Label {
            id: lStatus
            Layout.fillWidth: true
            text: "AdMob status:" + adMob.valid?" valid":"invalid"
        }

        Label {
            text: "AdMob state: " + aLoaded
        }

        Label {
            text: "Banner height: " + adMob.adHeight
        }

        Button {
            text: "Hide"
            onClicked: {
                adMob.adHide()
            }
        }


        Button {
            text: "Show"
            onClicked: {
                adMob.adShow()
            }
        }

        Button {
            text: "Set pos"
            onClicked: {
                adMob.adSetPos(0, posY)
                posY = posY + 10
            }
        }

        Button {
            text: "Intersititial show"
            onClicked: {
                adMob.interstitialShow()
            }
        }
    }


}
