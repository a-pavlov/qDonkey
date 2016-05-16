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
                onTriggered: helpDialog.show()
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
