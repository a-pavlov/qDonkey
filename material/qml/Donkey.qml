import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

ApplicationWindow {
    id: qDonkey
    title: "qDonkey"
    width:  400
    visible: true

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
            console.log("exit accepted");
            forceExit=true
            close()
        }

        onRejected: {
            console.log("exit rejected");
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
            Flickable {
                id: flickable
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }

                clip: true
                contentHeight: Math.max(example.implicitHeight + 140, height+100)

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
