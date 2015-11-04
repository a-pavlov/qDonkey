import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem
import TransferModelItemEnum 1.0


Page {
    id: searchResults
    title: "Back to transfers"

    ListView {
        anchors.fill: parent
        model: transferDetails

        delegate: ListItem.BaseListItem {
            fullHeight: true
            height: info.implicitHeight
            RowLayout {
                anchors.fill: parent
                Icon {
                    Layout.alignment: Qt.AlignLeft

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

                    size: Units.dp(32)
                }

                ColumnLayout {
                    id: info
                    Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                    Layout.fillWidth: true

                    spacing: Units.dp(3)

                    Label {
                        id: labelHash

                        Layout.alignment: Qt.AlignVCenter
                        Layout.fillWidth: true

                        elide: Text.ElideRight
                        style: "subheading"
                        wrapMode: Text.WrapAnywhere
                        maximumLineCount: 2
                        text: hash
                    }

                    Label {
                        id: labelName
                        Layout.fillWidth: true
                        color: Theme.light.subTextColor
                        elide: Text.ElideRight
                        wrapMode: Text.WordWrap
                        style: "body1"

                        maximumLineCount: 3
                        text: name
                    }

                    Label {
                        id: labelSize

                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: visible ? implicitWidth : 0

                        color: Theme.light.subTextColor
                        elide: Text.ElideRight
                        horizontalAlignment: Qt.AlignHCenter
                        style: "body1"
                        visible: text != ""
                        text: "Size " + size
                    }

                    RowLayout {

                        Icon {
                            Layout.alignment: Qt.AlignVCenter
                            source: Qt.resolvedUrl("qrc:/images/chevron-double-down.svg")
                            size: Units.dp(16)
                        }

                        Label {
                            id: labelDlSpeed

                            Layout.alignment: Qt.AlignVCenter
                            Layout.preferredWidth: visible ? implicitWidth : 0

                            color: Theme.light.subTextColor
                            elide: Text.ElideRight
                            horizontalAlignment: Qt.AlignVCenter
                            style: "body1"
                            text: dl_speed
                        }

                        Icon {
                            Layout.alignment: Qt.AlignVCenter
                            source: Qt.resolvedUrl("qrc:/images/chevron-double-up.svg")
                            size: Units.dp(16)
                        }

                        Label {
                            id: labelUpSpeed

                            Layout.alignment: Qt.AlignVCenter
                            Layout.preferredWidth: visible ? implicitWidth : 0

                            color: Theme.light.subTextColor
                            elide: Text.ElideRight
                            horizontalAlignment: Qt.AlignVCenter
                            style: "body1"
                            text: up_speed
                        }
                    }

                    Label {
                        id: labelSeedsPeers

                        Layout.alignment: Qt.AlignVCenter
                        Layout.preferredWidth: visible ? implicitWidth : 0

                        color: Theme.light.subTextColor
                        elide: Text.ElideRight
                        horizontalAlignment: Qt.AlignVCenter
                        style: "body1"
                        text: "Seeds/Peers " + seeds + "/" + peers
                    }


                    RowLayout {
                        Label {
                            id: labelRemain

                            Layout.alignment: Qt.AlignVCenter
                            Layout.preferredWidth: visible ? implicitWidth : 0

                            color: Theme.light.subTextColor
                            elide: Text.ElideRight
                            horizontalAlignment: Qt.AlignHCenter
                            style: "body1"
                            text: "ETA " + eta
                        }

                        ProgressBar {
                            Layout.fillWidth: true
                            color: theme.accentColor

                            indeterminate: false
                            minimumValue: 0
                            maximumValue: 100
                            value: progress
                        }
                    }

                }
            }

        }
    }
}
