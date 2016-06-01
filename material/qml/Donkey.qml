import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem

ApplicationWindow {
    id: qDonkey
    title: "qDonkey"
    width:  400
    height: 500
    visible: true

    signal init_dir_accepted()
    signal init_dir_rejected()

    property string lastErrorFilename: ""
    property string lastErrorMessage: ""
    property bool forceExit: false

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

    HelpDialog {
        id: helpDialog
    }

    Dialog {
        id: helpConn
        hasActions: false
        text: qsTr("Choose server for connection, click for menu. Connection to server need for search keywords and search sources for downloading.
If you have router and got LowID try to forward listen port in Pref(reconnect to server after it).
LowID means your port is not accessible from outside - it is significantly reduce download speed.")
    }

    Dialog {
        id: helpSearch
        hasActions: false
        text: qsTr("Enter search phrase into text field. You can specify parameters like sources count, size and so on.
To start search press Start button. If search returns results you will be forwarded to search result page.
Press Results button to see previous search results.
More buttton is enabled when more results are available for search.
To search in KAD use switch KAD only search.")
    }

    Dialog {
        id: helpTransfers
        hasActions: false
        text: qsTr("Shows your current active transfers. All files in incoming directory are transfers. By default incoming directory is your Download directory.")
    }

    Dialog {
        id: helpPref
        hasActions: false
        text: qsTr("Setup program options here. Language changes will applied after restart application.
Incoming directory and port applies after you leave preferences page. Be careful about changing listen port and incoming directory.
Help about KAD you can see on help button near KAD switch.")
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
        else if (pref.needHelp) {
            helpDialog.show()
            pref.needHelp = false
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
        title: "qDonkey"
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
