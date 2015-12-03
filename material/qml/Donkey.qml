import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem
import ru.forsk.adctl 1.0

ApplicationWindow {
    id: qDonkey
    title: "qDonkey"
    width:  400
    height: 500
    visible: true

    property string lastErrorFilename: ""
    property string lastErrorMessage: ""
    property bool forceExit: false
    property bool showBanner: false

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
        gAnalyticsId: ""

        //Start positions for banners.
        adMobBannerPosition: Qt.point(0,-500)
        startAdBannerPosition: Qt.point(0,-500)
        startAdBannerSize: Qt.size(250, 50)

        //when StartAd.mobi baners is showed we can to reposition it
        onStartAdBannerShowed: {
            console.log("onStartAdBannerShowed");
            startAdBannerPosition = Qt.point(0,
                                     (qDonkey.height - adCtl.startAdBannerHeight * 1.3))
        }

        //when AdMob baners is showed we can to reposition it
        onAdMobBannerShowed: {
            console.log("onAdMobBannerShowed");
            adCtl.adMobBannerPosition = Qt.point(0, Units.gu(2) + Units.dp(2))
            console.log("Set offset for banner to " + Units.gu(2));
        }

        onAdMobBannerLoaded: {
            console.log("onAdMobBannerLoaded")
            //showBanner = true
            //adCtl.showAdMobBanner()
        }

        //When all variables are setted, we can to initialize our code
        Component.onCompleted: {
            adCtl.init();
        }
    }

    Timer {
        interval: 120000
        running: true
        repeat: true
        onTriggered: {
            console.log("Show admob interstitial")
            adCtl.showAdMobInterstitial()
        }
    }

    Timer {
        interval: 30000
        running: true
        repeat: false
        onTriggered: {
            showBanner = true
            adCtl.showAdMobBanner()
        }
    }

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

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }

    Component.onCompleted: {
        if (pref.inputDir.length === 0) initDialog.show()
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
        title: "qDonkey"
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
                height: showBanner?adCtl.adMobBannerHeight:Units.dp(0)
                visible: showBanner
                enabled: showBanner
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
        btnInputOk.enabled=inputDirEdit.text.length != 0
    }
}
