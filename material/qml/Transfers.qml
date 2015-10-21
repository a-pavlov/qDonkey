import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

Item {
    ColumnLayout {
        spacing: 0
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: parent.top
        }
    }

    ListView {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }

        model: ["Transfer1", "Transfer2", "Trasfer3"]

        delegate: ListItem.SimpleMenu {
            id: transfer
            text: modelData
            subText: "stalled"
            selectedIndex: -1

            secondaryItem: ProgressCircle {
                id: determinateProgress
                Layout.alignment: Qt.AlignCenter
                width: Units.dp(64)
                height: Units.dp(64)
                indeterminate: false
                minimumValue: 0
                maximumValue: 100

                SequentialAnimation on value {
                    running: true
                    loops: NumberAnimation.Infinite

                    NumberAnimation {
                        duration: 3000
                        from: determinateProgress.minimumValue
                        to: determinateProgress.maximumValue
                    }

                    PauseAnimation { duration: 1000 }
                }

                Label {
                    anchors.centerIn: parent
                    text: Math.round(determinateProgress.value) + "%"
                }
            }

            action: Icon {
                anchors.centerIn: parent
                source: Qt.resolvedUrl("qrc:/images/folder-plus.svg")
                size: Units.dp(32)
            }

            maximumLineCount: 2
            model: ["Pause", "Resume", "Remove"]
        }
    }
}


