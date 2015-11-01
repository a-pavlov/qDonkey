import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem
import TransferModelItemEnum 1.0

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

        model: transferModel

        delegate: ListItem.SimpleMenu {
            id: transfer
            text: name
            subText: size
            selectedIndex: -1

            secondaryItem: ProgressCircle {
                id: determinateProgress
                Layout.alignment: Qt.AlignCenter
                width: Units.dp(64)
                height: Units.dp(64)
                indeterminate: false
                minimumValue: 0
                maximumValue: 100
                value: progress
                dashThickness: Units.dp(8)

                /*SequentialAnimation on value {
                    running: true
                    loops: NumberAnimation.Infinite

                    NumberAnimation {
                        duration: 3000
                        from: determinateProgress.minimumValue
                        to: determinateProgress.maximumValue
                    }

                    PauseAnimation { duration: 1000 }
                }*/

                Label {
                    anchors.centerIn: parent
                    text: Math.round(determinateProgress.value) + "%"
                }
            }

            action: Icon {
                anchors.centerIn: parent
                source: {
                    switch(status) {
                    case TransferModelItem.STATE_STALLED_DL:
                        return Qt.resolvedUrl("qrc:/images/timer.svg")
                    case TransferModelItem.STATE_STALLED_UP:
                        return Qt.resolvedUrl("qrc:/images/file-check.svg")
                    case TransferModelItem.STATE_DOWNLOADING:
                        return Qt.resolvedUrl("qrc:/images/chevron-double-down.svg")
                    case TransferModelItem.STATE_SEEDING:
                        return Qt.resolvedUrl("qrc:/images/chevron-double-up.svg")
                    case TransferModelItem.STATE_PAUSED_DL:
                    case TransferModelItem.STATE_PAUSED_UP:
                        return Qt.resolvedUrl("qrc:/images/pause-circle.svg")
                    case TransferModelItem.STATE_CHECKING:
                        return Qt.resolvedUrl("qrc:/images/wrench.svg")
                    case TransferModelItem.STATE_INVALID:
                        return Qt.resolvedUrl("qrc:/images/alert-circle.svg")
                    default:
                        return Qt.resolvedUrl("qrc:/images/alert-circle.svg")
                    }
                }

                size: Units.dp(32)
            }

            maximumLineCount: 3
            model: ["Pause", "Resume", "Remove"]

            onSelectedIndexChanged: {
                switch(selectedIndex) {
                case 0:
                    console.log("pause " + hash)
                    session.pauseTransfer(hash);
                    break
                case 1:
                    console.log("resume " + hash)
                    session.resumeTransfer(hash);
                    break
                case 2:
                    console.log("remove " + hash)
                    session.deleteTransfer(hash,true)
                    transferModel.removeTransfer(hash)
                    break
                default:
                    console.log("undefined index " + selectedIndex)
                }

                selectedIndex=-1
            }
        }
    }
}


