import QtQuick 2.5
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

ColumnLayout {
    id: connection
    spacing: 0
    anchors.fill: parent
    /*anchors {
        left: parent.left
        right: parent.right
    }*/

    Label {
        anchors {
            left: parent.left
            top: parent.top
            margins: Units.dp(16)
        }

        id: title
        text: qsTr("Double click on server for connect/disconnect")
        style: "body2"
    }

    ListView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        /*anchors {
            left: parent.left
            right: parent.right
            top: title.bottom
            bottom: parent.bottom
        }*/

        model: serverModel

        delegate: ListItem.Subtitled {
                id: itm
                text: alias
                subText: host + ":" + port

                action: Icon {
                    id: icns
                    anchors.centerIn: parent
                    source: (status==0)?Qt.resolvedUrl("qrc:/images/flash.svg"):Qt.resolvedUrl("qrc:/images/flash-off.svg")
                }

                valueText: {
                    if (status==0) return qsTr("Connected")
                    if (status==1) return qsTr("Connecting...")
                    if (status==2) return ""
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        serverModel.update(alias, host, port)
                }
            }
        }
    }

}
