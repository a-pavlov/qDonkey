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
            //secondaryItem: Switch {
            //    id: enablingSwitch
            //    anchors.verticalCenter: parent.verticalCenter
            //}

            action: Icon {
                id: icns
                anchors.centerIn: parent
                state: "rotated"
                name: (status == 0)?"action/done":"action/search"
                rotation: (status != 1)?0:0

                NumberAnimation on rotation {
                    running: status == 1
                    from: 0
                    to: 360
                    loops: Animation.Infinite
                    duration: 1600
                }
            }

            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    console.log("Double click on " + itm.text);
                    serverModel.update(itm.text)
                }
            }
        }
    }

}
