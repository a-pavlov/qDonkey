import QtQuick 2.2
import Material 0.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: qDonkey
    title: "qDonkey"
    width:  400
    visible: true


    // compatibility with qDonkey.qml
    function toLocalFile(url) {
        var lp = url.replace(/^(file:\/{3})/,"")
        return lp
    }

    function toURL(filepath) {
        return "file:///" + filepath
    }

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }

    property string conn: "Connection"
    property string transfers: "Transfers"
    property string search: "Search"

    property var sections: [ conn, transfers, search ]

    property var sectionTitles: [ "Connection", "Transfers", "Search" ]

    property string selectedComponent: conn

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        selectFolder: true
        onAccepted: {
            console.log("You chose: " + fileDialog.folder)
            pref.folder=fileDialog.folder
        }
        onRejected: {
            console.log("Canceled")
        }
        Component.onCompleted: visible = false
    }

    InitDialog {
        id: initDialog
    }


    initialPage: Page {
        id: page
        title: "qDonkey"
        ColumnLayout {
            Label {
                id: label
                text: "Some text"
            }

            Button {
                text: "Start file dialog"
                onClicked: {
                    console.log("Button cliked")
                    fileDialog.open()
                }
            }
        }

        Component.onCompleted: initDialog.show()
    }

}
