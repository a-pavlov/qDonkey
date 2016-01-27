import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

Page {
    id: searchResults
    title: qsTr("Back to search")

    Component.onDestruction: {
        adMob.adShow()
    }

    ColumnLayout {
        id: content
        spacing: 0
        anchors.fill: parent

        Label {
            anchors {
                top: parent.top
                left: parent.left
            }

            text: qsTr("Select item for download")
            style: "body2"
        }

        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true

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
                        break
                    case 1:
                        if (session.addTransfer(hash, name, filesize_num, sources_num, false)) {
                            snackbar.open(qsTr("File %1 added to downloads").arg(name))
                        } else {
                            snackbar.open(qsTr("Error occured on adding %1").arg(name))
                        }
                        break
                    case 2:
                        if (session.addTransfer(hash, name, filesize_num, sources_num, true)) {
                            snackbar.open(qsTr("File %1 added to downloads").arg(name))
                        } else {
                            snackbar.open(qsTr("Error occured on adding %1").arg(name))
                        }
                        break
                    default:
                        console.log("undefined index " + selectedIndex)
                    }

                    selectedIndex=-1
                }
            }
        }

        Snackbar {
            id: snackbar
        }
    }
}
