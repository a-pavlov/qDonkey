import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem
import TransferModelItemEnum 1.0

    ColumnLayout {
        spacing: 0
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            top: parent.top
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
                text: size

                subText: name
                valueText: {
                    switch(status) {
                    case TransferModelItem.STATE_STALLED_DL: return "Stalled dl"
                    case TransferModelItem.STATE_STALLED_UP: return "Stalled up"
                    case TransferModelItem.STATE_DOWNLOADING: return "Downloading..."
                    case TransferModelItem.STATE_SEEDING: return "Seeding..."
                    case TransferModelItem.STATE_PAUSED_DL:
                    case TransferModelItem.STATE_PAUSED_UP:
                        return "Paused"
                    case TransferModelItem.STATE_CHECKING: return "Checking..."
                    case TransferModelItem.STATE_INVALID: return "Invalid"
                    default: return "Invalid"
                    }
                }

                selectedIndex: -1

                secondaryItem: ProgressCircle {
                    id: determinateProgress
                    Layout.alignment: Qt.AlignCenter
                    width: Units.dp(48)
                    height: Units.dp(48)
                    indeterminate: false
                    minimumValue: 0
                    maximumValue: 100
                    value: progress

                    Label {
                        anchors.centerIn: parent
                        text: Math.round(determinateProgress.value) + "%"
                    }
                }

                action: Icon {
                    anchors.centerIn: parent
                    source: {
                        switch(type) {
                            case "Any": return Qt.resolvedUrl("qrc:/images/file.svg")
                            case "Video": return Qt.resolvedUrl("qrc:/images/file-video.svg")
                            case "Audio": return Qt.resolvedUrl("qrc:/images/file-music.svg")
                            case "Document": return Qt.resolvedUrl("qrc:/images/file-document.svg")
                            case "Picture": return Qt.resolvedUrl("qrc:/images/file-image.svg")
                            case "Archive": return Qt.resolvedUrl("qrc:/images/archive.svg")
                            case "CD image": return Qt.resolvedUrl("qrc:/images/disqus.svg")
                            case "Emule collection": return Qt.resolvedUrl("qrc:/images/file-multiple.svg")
                            default: return Qt.resolvedUrl("qrc:/images/file.svg")
                        }
                    }

                    size: Units.dp(24)
                }

                maximumLineCount: 4
                model: ["Pause", "Resume", "Remove", "Details"]

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
                    case 3:
                        console.log("details for " + hash);
                        transferDetails.setHash(hash)
                        pageStack.push(Qt.resolvedUrl("TransferDetails.qml"))
                        break
                    default:
                        console.log("undefined index " + selectedIndex)
                    }

                    selectedIndex=-1
                }
            }
        }

    }


