import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

Dialog {
    id: prefDialog
    anchors.centerIn: parent
    hasActions: false

    width: Units.dp(350)
    height: column.implicitHeight + Units.dp(32)

    ColumnLayout {
        id: column

        anchors {
            fill: parent
            topMargin: Units.dp(16)
            bottomMargin: Units.dp(16)
        }

        Label {

            anchors {
                left: parent.left
                right: parent.right
                margins: Units.dp(16)
            }

            style: "title"
            text: "Preferences"
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: Units.dp(8)
        }

        ListItem.Standard {
            action: Icon {
                anchors.centerIn: parent
                source: Qt.resolvedUrl("qrc:/images/account.svg")
            }

            content: TextField {
                id: nick
                anchors.centerIn: parent
                width: parent.width
                floatingLabel: true
                placeholderText: "Enter your nickname"

                text: pref.nick
            }
        }

        ListItem.Standard {

            action: Icon {
                anchors.centerIn: parent
                source: Qt.resolvedUrl("qrc:/images/access-point-network.svg")
            }

            content: TextField {
                id: listenPort
                anchors.centerIn: parent
                width: parent.width
                floatingLabel: true
                placeholderText: "Listen port"
                text: pref.listenPort
            }
        }

        ListItem.Standard {
            height: grid.height + Units.dp(8)
            implicitHeight: grid.implicitHeight + Units.dp(8)

            action: Icon {
                anchors.centerIn: parent
                source: Qt.resolvedUrl("qrc:/images/speedometer.svg")
            }

            content: GridLayout {
                id: grid
                columns: 2
                anchors.centerIn: parent
                rowSpacing: Units.dp(1)
                columnSpacing: Units.dp(10)

                CheckBox {
                    id: checkDownload
                    checked: pref.dlSpeedLimited
                    text: "Down"
                    onCheckedChanged: {
                        limitDownload.enabled=checked
                    }
                }

                TextField {
                    id: limitDownload
                    enabled: pref.dlSpeedLimited
                    width: parent.width
                    floatingLabel: true
                    placeholderText: "Dowload limit Kb/s"
                    text: pref.dlSpeed
                    validator: IntValidator {}
                }

                CheckBox {
                    id: checkUpload
                    Layout.alignment: Qt.AlignVCenter
                    checked: pref.upSpeedLimited
                    text: "Up"
                     onCheckedChanged: {
                        limitUpload.enabled=checked
                    }
                }

                TextField {
                    id: limitUpload
                    enabled: pref.upSpeedLimited
                    width: parent.width
                    floatingLabel: true
                    placeholderText: "Upload limit Kb/s"
                    text: pref.upSpeed
                    validator: IntValidator {}
                }
            }
        }

        ListItem.Standard {
            action: Icon {
                anchors.centerIn: parent
                source: Qt.resolvedUrl("qrc:/images/folder-download.svg")
            }

            content: TextField {
                id: inputDir
                anchors.centerIn: parent
                width: parent.width
                floatingLabel: true
                placeholderText: "Incoming directory"
                text: pref.inputDir
            }
        }

        ListItem.Standard {
            action: Item {}
            content: RowLayout {
                Switch {
                    id: showPreviusTransfers
                    checked: pref.showAllTransfers
                    enabled: true
                    darkBackground: false
                }

                Label {
                    text: "Show old transfers"
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
                text: "Cancel"
                textColor: Theme.primaryColor
                onClicked: {
                    prefDialog.close()
                }
            }

            Button {
                text: "Ok"
                textColor: Theme.primaryColor
                onClicked: {
                    console.log("write settings")
                    pref.nick = nick.text
                    pref.listenPort = listenPort.text
                    pref.dlSpeedLimited = checkDownload.checked
                    pref.dlSpeed = limitDownload.text
                    pref.upSpeedLimited = checkUpload.checked
                    pref.upSpeed = limitUpload.text
                    pref.inputDir=inputDir.text
                    pref.flush()
                    prefDialog.close()
                }
            }
        }
    }
}
