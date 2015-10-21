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
                text: name
                subText: sources
                valueText: filesize
                selectedIndex: -1

                action: Icon {
                    id: icns
                    anchors.centerIn: parent
                    source: {
                        switch(type) {
                        case "Any":
                            Qt.resolvedUrl("qrc:/images/file.svg")
                            break
                        case "Video":
                            Qt.resolvedUrl("qrc:/images/file-video.svg")
                            break
                        case "Audio":
                            Qt.resolvedUrl("qrc:/images/file-music.svg")
                            break
                        case "Document":
                            Qt.resolvedUrl("qrc:/images/file-document.svg")
                            break
                        case "Picture":
                            Qt.resolvedUrl("qrc:/images/file-image.svg")
                            break
                        case "Archive":
                            Qt.resolvedUrl("qrc:/images/archive.svg")
                            break
                        case "CD image":
                            Qt.resolvedUrl("qrc:/images/disqus.svg")
                            break
                        case "Emule collection":
                            Qt.resolvedUrl("qrc:/images/file-multiple.svg")
                            break
                        default:
                            Qt.resolvedUrl("qrc:/images/file.svg")
                            break
                        }
                    }
                }


                model: previewable?["Search related", "Download", "Preview"]:[ "Search related", "Download"]

                onSelectedIndexChanged: {
                    switch(selectedIndex) {
                    case 0:
                        session.searchRelatedFiles(hash)
                        console.log("search related files " + hash)
                        break
                    case 1:
                        console.log("start download")
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
