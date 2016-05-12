import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.2
import Material.ListItems 0.1 as ListItem

Page {
    id: kad
    title: qsTr("Back to Pref")

    ColumnLayout {
        spacing: 0
        anchors.fill: parent
        Label {
            anchors {
                top: parent.top
                left: parent.left
            }

            text: qsTr("Kademlia routing table")
            style: "body2"
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: kadModel

            delegate: ListItem.Subtitled {
                id: kadNode
                text: host + ":" + port
                subText: id
                valueText: distance

                action: Icon {
                    name: "action/cached"
                }
            }
        }
    }
}
