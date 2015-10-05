import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Item {
    ColumnLayout {
        anchors {
            fill: parent
            topMargin: Units.dp(16)
            bottomMargin: Units.dp(16)
        }

        Label {
            anchors {
                left: parent.left
                right: parent.right
                margins: Units.dp(16)
            }

            style:  "title"
            text: "Seach in ed2k network"
        }

        ListItem.Standard {
            content: RowLayout {
                width: parent.width

                TextField {
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredWidth: 0.5 * parent.width
                    text: ""
                    placeholderText: "Search phrase"
                }

                MenuField {
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredWidth: 0.5*parent.width
                    model: ["Any", "Video", "Audio", "Document", "Archive", "ED2K Link"]
                }
            }
        }

        ListItem.Standard {
            content: RowLayout {
                width: parent.width
                TextField {
                    Layout.preferredWidth: 0.5*parent.width
                    placeholderText: "Min size, Mb"
                }

                TextField {
                    placeholderText: "Max size, Mb"
                }
            }
        }


        ListItem.Standard {
            content: RowLayout {
                width: parent.width
                TextField {
                    Layout.preferredWidth: 0.5*parent.width
                    placeholderText: "Availibility"
                }

                TextField {
                    placeholderText: "Full sources"
                }
            }
        }

        ListItem.Standard {
            content : TextField {
                placeholderText: "Extension"
            }
        }

        ListItem.Standard {
            content : TextField {
                placeholderText: "Codec"
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: Units.dp(8)

            anchors {
                right: parent.right
                margins: Units.dp(16)
            }

            Button {
                text: "Start"
                textColor: Theme.primaryColor
            }

            Button {
                text: "More"
                textColor: Theme.primaryColor;
            }

            Button {
                text: "Cancel"
                textColor: Theme.primaryColor
            }
        }
    }
/*
    OverlayView {
        id: overlayView

        width: Units.dp(800)
        height: Units.dp(500)

        Image {
            id: contentImage
            source: Qt.resolvedUrl("images/balloon.jpg")
            anchors.fill: parent
        }

        Row {
            anchors {
                top: parent.top
                right: parent.right
                rightMargin: Units.dp(16)
            }
            height: Units.dp(48)
            opacity: overlayView.transitionOpacity

            spacing: Units.dp(24)

            Repeater {
                model: ["content/add", "image/edit", "action/delete"]

                delegate: IconButton {
                    id: iconAction

                    iconName: modelData

                    color: Theme.dark.iconColor
                    size: iconName == "content/add" ? Units.dp(27) : Units.dp(24)
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
    */
}
