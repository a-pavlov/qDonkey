import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem
import org.dkfsoft.admobctrl 1.0

ApplicationWindow {
    id: qDonkey
    title: "Mule for Android"
    width:  400
    height: 500
    visible: true

    property string lastErrorFilename: ""
    property string lastErrorMessage: ""
    property bool forceExit: false
    property bool adLoaded: false
    property string link_filename: ""

    AdMobCtrl {
        id: adMob
    }

    Timer {
        interval: 5000
        running: true
        repeat: true
        onTriggered: {

            if (adMob.adLoaded) {
                adLoaded = adMob.adLoaded
                adMob.adSetPos(0, Units.gu(2))
                adMob.adShow()
                stop()
            }
        }
    }


    /*Timer {
        interval: 120000
        running: true
        repeat: true
        onTriggered: {
            console.log("Show admob interstitial")
            adCtl.showAdMobInterstitial()
        }
    }*/


    Dialog {
        id: quitAsk
        text: qsTr("Do you really want to quit?");
        positiveButtonText: qsTr("Yes")
        negativeButtonText: qsTr("No")
        hasActions: true
    }

    onClosing: {
        if (pref.askOnExit && !forceExit) {
            quitAsk.show()
            close.accepted=false
        } else {
            close.accepted=true
        }
    }

    Dialog {
        id: ioErrorDialog
        hasActions: true
        positiveButtonText: qsTr("Ok")
        negativeButton.visible: false
        title: qsTr("I/O error")
        text: qsTr("Filename: %1 message %2").arg(lastErrorFilename).arg(lastErrorMessage)
    }

    Connections {
        target: session
        onFileIOError: {
            lastErrorFilename=filename
            lastErrorMessage=message
            ioErrorDialog.show()
        }
    }

    Connections {
        target: quitAsk
        onAccepted: {
            forceExit=true
            close()
        }
    }

    InitDialog {
        id: initDialog
    }

    AddLinkDialog {
        id: linkAdded
    }

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }

    Component.onCompleted: {
        link_filename = adMob.link
        if (pref.inputDir.length === 0) initDialog.show()
        // Do not show interstitial at first time
        //adMob.interstitialShow()
        console.log("Link is " + adMob.link)
        if (adMob.link.length != 0 && pref.inputDir.length !== 0) {
            if (session.addLinkQml(adMob.link, true)) {
                linkAdded.show()
            }
        }
    }

    property string connections: "Connection"
    property string transfers: "Transfers"
    property string search: "Search"
    property string preferences: "Preferences"

    property var sections: [ connections, transfers, search, preferences ]
    property var sectionTitles: [ qsTr("Conn"), qsTr("Transfers"), qsTr("Search"), qsTr("Pref") ]
    property string selectedComponent: connections


    initialPage: TabbedPage {
        id: page
        title: "Mule for Android"
        actionBar.maxActionCount: 0
        focus: true

        Repeater {
            id: content
            model: sections
            delegate: Tab {
                title: sectionTitles[index]
                property string selectedComponent: modelData
                property string section: modelData
                sourceComponent: tabDelegate
            }
        }
    }

    Component {
        id: tabDelegate
        Item {

            Item {
                id: banner
                height: adLoaded?adMob.adHeight:Units.dp(0)
                visible: adLoaded

                anchors {
                    top: parent.top
                    left: parent.left
                    margins: Units.dp(16)
                }
            }

            Flickable {
                id: flickable
                anchors {
                    left: parent.left
                    right: parent.right
                    top: banner.bottom
                    bottom: parent.bottom
                }

                clip: true
                contentHeight: Math.max(example.implicitHeight + Units.dp(40), height+Units.dp(40))

                Loader {
                    id: example
                    anchors.fill: parent
                    asynchronous: true
                    visible: status == Loader.Ready
                    // selectedComponent will always be valid, as it defaults to the first component
                    source: {
                        console.log(Qt.resolvedUrl("% clicked").arg(selectedComponent.replace(" ", "")))
                        return Qt.resolvedUrl("%.qml").arg(selectedComponent.replace(" ", ""))
                    }
                }

                ProgressCircle {
                    anchors.centerIn: parent
                    visible: example.status == Loader.Loading
                }
            }
            Scrollbar {
                flickableItem: flickable
            }
        }
    }

    function switchBtns() {
        console.log("swith btns: " + inputDirEdit.text)
        btnInputOk.enabled=inputDirEdit.text.length !== 0
    }
}
