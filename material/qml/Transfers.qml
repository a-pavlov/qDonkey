import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem
import TransferModelItemEnum 1.0

Page {
    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        Label {
            id: title
            anchors {
                left: parent.left
                right: parent.right
                margins: Units.dp(16)
            }

            style:  "body2"
            text: qsTr("Click for action menu")
        }

        Dialog {
            id: failOnOpen
            text: qsTr("Unable to open file of this type")
            hasActions: true
            negativeButton.visible: false
        }


        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            model: transferProxy

            delegate: ListItem.SimpleMenu {
                id: transfer
                text: size

                subText: name
                valueText: {
                    switch(status) {
                    case TransferModelItem.STATE_STALLED_DL: return qsTr("Stalled dl")
                    case TransferModelItem.STATE_STALLED_UP: return qsTr("Stalled up")
                    case TransferModelItem.STATE_DOWNLOADING: return qsTr("Downloading...")
                    case TransferModelItem.STATE_SEEDING: return qsTr("Seeding...")
                    case TransferModelItem.STATE_PAUSED_DL:
                    case TransferModelItem.STATE_PAUSED_UP:
                        return qsTr("Paused")
                    case TransferModelItem.STATE_CHECKING: return qsTr("Checking...")
                    case TransferModelItem.STATE_INVALID: return qsTr("Invalid")
                    default: return qsTr("Invalid")
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
                }

                maximumLineCount: {
                                        return 4
                }

                model: {
                    return [qsTr("Pause"), qsTr("Resume"), qsTr("Remove"), qsTr("Details"), qsTr("Preview")]

                }

                onSelectedIndexChanged: {
                    switch(selectedIndex) {
                    case 0:
                        session.pauseTransfer(hash);
                        break
                    case 1:
                        session.resumeTransfer(hash);
                        break
                    case 2:
                        session.deleteTransfer(hash,true)
                        transferModel.removeTransfer(hash)
                        break
                    case 3:
                        transferDetails.setHash(hash)
                        pageStack.push(Qt.resolvedUrl("TransferDetails.qml"))
                        break
                    case 4:
                        session.deferPlayMediaFile(hash)
                        break
                    default:
                        break
                    }

                    selectedIndex=-1
                }

            }
        }

    }
}


