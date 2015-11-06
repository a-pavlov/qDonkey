import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

Page {
    id: searchResults
    title: qsTr("Back to search")

    Dialog {
        id: alertTransferAddSucc
        width: Units.dp(300)
        text: qsTr("Transfer was added to download list")
        hasActions: false
    }

    Dialog {
        id: alertTransferAddFail
        width: Units.dp(300)
        text: qsTr("Unable to add transfer")
        hasActions: false
    }

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

            model: searchFPModel

            delegate: ListItem.SimpleMenu {
                id: searchItem
                text: filesize
                subText: name
                valueText: sources
                selectedIndex: -1
                maximumLineCount: 3

                action: Icon {
                    id: icns
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


                model: previewable?[qsTr("Search related"), qsTr("Download"), qsTr("Preview")]:[ qsTr("Search related"), qsTr("Download")]

                onSelectedIndexChanged: {
                    switch(selectedIndex) {
                    case 0:
                        session.searchRelatedFiles(hash)
                        console.log("search related files " + hash)
                        break
                    case 1:
                        console.log("start download")
                        if (session.addTransfer(hash, name, filesize_num, sources_num)) {
                            alertTransferAddSucc.show()
                        } else {
                            alertTransferAddFail.show()
                        }

                        break
                    case 2:
                        console.log("start preview")
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
