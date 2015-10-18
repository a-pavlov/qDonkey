import QtQuick 2.5
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

    Label {
        anchors {
            left: parent.left
            top: parent.top
        }

        id: title
        text: "Choose server for connection"
        style: "title"
    }

    ListView {
        anchors {
            left: parent.left
            right: parent.right
            top: title.bottom
            bottom: parent.bottom
        }

        model: serverModel

        delegate: ListItem.Subtitled {
                id: itm
                text: alias
                subText: host + ":" + port

                action: Icon {
                    id: icns
                    anchors.centerIn: parent
                    name: "action/done"
                }

                valueText: {
                    if (status==0) return "Connected"
                    if (status==1) return "Connecting..."
                    if (status==2) return ""
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        console.log("Double click on " + itm.text);
                        serverModel.update(itm.text, itm.host, itm.port)
                }
            }
        }
    }

}
