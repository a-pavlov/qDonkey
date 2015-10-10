import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

Page {
    id: searchResults
    title: "Back to search"
    ColumnLayout {
        spacing: 0
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: parent.top
        }

        Label {
            anchors {
                left: parent.left
                top: parent.top
            }

            id: title
            text: "Search results"
            style: "title"
        }

        ListView {
            anchors {
                left: parent.left
                right: parent.right
                top: title.bottom
                bottom: parent.bottom
            }

            model: searchModel

            delegate: ListItem.Subtitled {
                id: searchItem
                text: name + searchModel.rowCount()
                subText: hash + " size: " + filesize
                //secondaryItem: Switch {
                //    id: enablingSwitch
                //    anchors.verticalCenter: parent.verticalCenter
                //}

                action: Icon {
                    id: icns
                    anchors.centerIn: parent
                    source:  Qt.resolvedUrl("images/audio.svg")
                }
/*
                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        console.log("Double click on " + itm.text);
                        serverModel.update(itm.text)
                    }
                }
*/
            }
        }
    }
}
