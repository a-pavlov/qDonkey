import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem
import QtQuick.Dialogs 1.2 as  FD

Dialog {
    property string inputDirectory: pref.defaultInputDir

    FD.FileDialog {
        id: fd
        title: qsTr("Please choose a folder")
        folder: toURL(inputDirectory)
        selectFolder: true
        onAccepted: {
            var path = decodeURIComponent(toLocalFile(folder.toString()))
            console.log("You chose: " + path)
            initDialog.inputDirectory=path
        }

        onRejected: {
            console.log("Canceled")
        }

        Component.onCompleted: visible = false
    }


    id: initDialog
    hasActions: false
    anchors.centerIn: parent
    width: Units.dp(350)
    height: init_column.implicitHeight + Units.dp(32)

    ColumnLayout {
        id: init_column

        anchors {
            fill: parent
            topMargin: Units.dp(16)
            bottomMargin: Units.dp(16)
        }

        Label {
            id: titleLabel

            anchors {
                left: parent.left
                right: parent.right
                margins: Units.dp(16)
            }

            style: "title"
            text: qsTr("Setup incoming directory")
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: Units.dp(8)
        }

        ListItem.Standard {
            id: incomingDir
            action: Icon {
                anchors.centerIn: parent
                source: Qt.resolvedUrl("qrc:/images/folder-download.svg")
            }

            content:
                TextField {
                    id: inputDirEdit
                    anchors.centerIn: parent

                    width: parent.width
                    floatingLabel: true
                    placeholderText: qsTr("Incoming directory")
                    text: inputDirectory
                    onTextChanged: {
                        btnInputOk.enabled=text.length!=0
                    }

                }
        }


        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: Units.dp(8)
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: Units.dp(8)

            anchors {
                right: parent.right
                margins: Units.dp(16)
            }

            Button {
                text: "..."
                textColor: Theme.primaryColor
                Layout.alignment: Qt.AlignLeft
                onClicked: {
                    console.log("open file dialog")
                    fd.open()
                }
            }

            Button {
                text: qsTr("Cancel")
                textColor: Theme.primaryColor
                onClicked: {
                    console.log("cancel clicked")
                    pref.inputDir = ""
                    initDialog.close()
                }
            }

            Button {
                id: btnInputOk
                text: qsTr("Ok")
                textColor: Theme.primaryColor
                enabled: true
                onClicked: {
                    console.log("input dir" + inputDirEdit.text)
                    pref.inputDir = inputDirEdit.text
                    initDialog.close()
                }
            }
        }
    }
}

