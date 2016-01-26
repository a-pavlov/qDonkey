import QtQuick 2.2
import Material 0.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

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
            Qt.quit()
        }
        onRejected: {
            console.log("Canceled")
            Qt.quit()
        }
        Component.onCompleted: visible = false
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
                id: fd
                text: "Start file dialog"
                onClicked: {
                    console.log("Button cliked")
                    fileDialog.open()
                }
            }
        }
    }

}
