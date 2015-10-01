import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

ColumnLayout {
    id: connection
    spacing: 0
    anchors {
        left: parent.left
        right: parent.right
    }

    function enableServerBtns() {
        addServer.enabled = serverAlias.text.length != 0 && serverIP.text.length != 0 && serverPort.text.length != 0;
        clearServer.enabled = serverAlias.text.length != 0 || serverIP.text.length != 0 || serverPort.text.length != 0;
    }

    ColumnLayout {
        spacing: Units.dp(15)
        id: column

        TextField {
            Layout.alignment: Qt.AlignCenter
            anchors {
                left: parent.left
                right: parent.right
            }

            width: parent.width
            id: serverAlias
            text: ""
            placeholderText: "Server alias"
            floatingLabel: true
            onTextChanged: {
                enableServerBtns()
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignLeft
            anchors {
                left: parent.left
            }

            TextField {
                id: serverIP
                text: ""
                placeholderText: "Server IP or name"
                floatingLabel: true
                onTextChanged: {
                    enableServerBtns()
                }
            }

            TextField {
                id: serverPort
                text: ""
                placeholderText: "Server port"
                floatingLabel: true
                validator: IntValidator {}
                onTextChanged: {
                    enableServerBtns()
                }
            }
        }

        RowLayout {
            id: row
            Layout.alignment: Qt.AlignLeft
            spacing: Units.dp(8)

            anchors {
                left: parent.left
                margins: Units.dp(16)
            }

            Button {
                id: addServer
                enabled: false
                text: "Add"
                textColor: Theme.primaryColor
            }

            Button {
                id: clearServer
                enabled: false
                text: "Clear"
                textColor: Theme.primaryColor
                onClicked: {
                    serverAlias.text = ""
                    serverIP.text = ""
                    serverPort.text = ""
                }
            }
        }
    }

    ListView {
        anchors {
            left: parent.left
            right: parent.right
            top: column.bottom
            bottom: parent.bottom
        }

        model: serverModel

        delegate: ListItem.Subtitled {
            text: alias
            subText: host + ":" + port
            secondaryItem: Switch {
                id: enablingSwitch
                anchors.verticalCenter: parent.verticalCenter
            }

            onClicked: enablingSwitch.checked = !enablingSwitch.checked

            action: Icon {
                anchors.centerIn: parent
                name: "action/settings_input_component"
                size: Units.dp(32)
            }

        }
    }
}
