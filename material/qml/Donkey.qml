import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem
import org.dkfsoft.Activity 1.0

ApplicationWindow {
    id: qDonkey
    title: "Mule for Android"
    width:  400
    height: 500
    visible: true

    signal init_dir_accepted()
    signal init_dir_rejected()

    property string lastErrorFilename: ""
    property string lastErrorMessage: ""
    property bool forceExit: false

    Activity {
        id: act
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


    AddLinkDialog {
        id: addLink
    }

    HelpDialog {
        id: helpDialog
    }

    Dialog {
        id: helpConn
        hasActions: false
        text: qsTr("Choose server for connection and click for action menu.
Connection to server need for search by keywords and search for sources on download stage.
If you have router and got LowID try to activate UPnP in preferences and reconnect to server.
LowID means your port is not accessible from outside.")
    }

    Dialog {
        id: helpSearch
        hasActions: false
        text: qsTr("Enter search phrase into text field. Additionaly you can specify minimal sources count, size and so on.
To start search press Start button. If search returns results you will be forwarded to search result page.
To search more results click More button. Search in KAD will be available soon.
Choose search result and click Download or Preview")
    }

    Dialog {
        id: helpTransfers
        hasActions: false
        text: qsTr("This page shows your current active transfers include all previous files from incoming directory.
By default it is your Download directory.
Click on transfer and choose action in menu. You can start view file before download completed using Previw option.")
    }

    Dialog {
        id: helpPref
        hasActions: false
        text: qsTr("Setup program options here. Language changes will applied after restart application.
Incoming directory and port applies after you leave pref page.
Be careful about changing listen port and incoming directory.
All content of incoming directory will be shared.
KAD help you can see on help button near KAD switch.")
    }

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }

    Component.onCompleted: {
        initDialog.accepted.connect(init_accepted)
        initDialog.rejected.connect(init_rejected);

        if (pref.inputDir.length === 0) initDialog.show()
        else if (act.link.length != 0) {
            if (session.loadLink(act.link, true)) addLink.show()
        }
    }

    function init_accepted() {
        console.log("init a")
        init_dir_accepted()
    }

    function init_rejected() {
        console.log("init r")
        init_dir_rejected()
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
        actionBar.maxActionCount: 1
        focus: true

        actions: [
            Action {
                iconName: "action/help"
                name: "Help"                
                onTriggered: {
                    switch(page.selectedTab) {
                    case 0:
                        helpConn.show()
                        break;
                    case 1:
                        helpTransfers.show()
                        break;
                    case 2:
                        helpSearch.show()
                        break;
                    case 3:
                        helpPref.show()
                        break;
                    }
                }
            }
        ]

        Repeater {
            model: sections
            delegate: Tab {
                title: sectionTitles[index]
                property string selectedComponent: modelData
                property string section: modelData
                sourceComponent: tabDelegate
            }
        }

        onSelectedTabChanged: {
            session.syncProperties()
        }
    }

    Component {
        id: tabDelegate
        Item {
            Flickable {
                id: flickable
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
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
