import QtQuick 2.5
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem

Page {
    id: conn

    Dialog {
        id: addDialog
        title: qsTr("Add server")
        positiveButtonEnabled: false
        width: Units.dp(300)

        ColumnLayout {
            TextField {
                id: name
                Layout.fillWidth: true
                placeholderText: qsTr("Alias")
                onTextChanged: {
                    addDialog.positiveButtonEnabled=name.text.length != 0 &&
                            host.text.length != 0 && port.text.length != 0
                }
            }

            RowLayout {
                TextField {
                    id: host
                    Layout.fillWidth: true
                    placeholderText: qsTr("Server host or IP")
                    onTextChanged: {
                        addDialog.positiveButtonEnabled=name.text.length != 0 &&
                                host.text.length != 0 && port.text.length != 0
                    }
                }

                TextField {
                    id: port
                    width: Units.dp(50)
                    placeholderText: qsTr("Port")
                    validator: IntValidator {}
                    onTextChanged: {
                        addDialog.positiveButtonEnabled=name.text.length != 0 &&
                                host.text.length != 0 && port.text.length != 0
                    }
                }
            }
        }

        onAccepted: {
            serverModel.create(name.text, host.text, port.text)
        }
    }

    ColumnLayout {
        id: connection
        spacing: 0
        anchors.fill: parent
        Label {
            anchors {
                left: parent.left
                top: parent.top
                margins: Units.dp(16)
            }

            id: title
            text: qsTr("Select server for connect")
            style: "body2"
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: serverModel

            delegate: ListItem.SimpleMenu {
                id: itm
                selectedIndex: -1
                text: alias
                subText: host + ":" + port

                valueText: {
                    if (status==0) {
                        if (cid == 0) return qsTr("Connected")
                        else {
                            return cid + ":" + (lowid?"LowId":"HiId");
                        }
                    }
                    if (status==1) return qsTr("Connecting...")
                    if (status==2) return ""
                }

                action: Icon {
                    id: icns
                    anchors.centerIn: parent
                    source: (status==0)?Qt.resolvedUrl("qrc:/images/flash.svg"):Qt.resolvedUrl("qrc:/images/flash-off.svg")
                }

                model: [qsTr("Conn/Disconnect"), qsTr("Add"), qsTr("Delete")]

                onSelectedIndexChanged: {
                    switch(selectedIndex) {
                    case 0:
                        serverModel.update(alias, host, port)
                        break
                    case 1:
                        addDialog.show()
                        break
                    case 2:
                        serverModel.remove(alias, host, port)
                        break
                    default:
                        console.log("undefined index " + selectedIndex)
                    }

                    selectedIndex=-1
                }
            }
        }

    }
}
