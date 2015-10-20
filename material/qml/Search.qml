import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Item {
    Connections {
        target: session
        onSearchFinished: {
            console.log("search finished, more result: " + bMoreResult)
            btnStart.enabled = true
            btnCancel.enabled = false
            btnMore.enabled = bMoreResult
            progress.enabled = false
            progress.visible = false
            pageStack.push(Qt.resolvedUrl("SearchResult.qml"))
        }
    }

    function switchBtns() {
        btnStart.enabled = sText.text.length != 0
        btnMore.enabled=false
    }

    Dialog {
        id: alertNoConnection
        width: Units.dp(300)
        text: "You are not connected to any ED2K server. Please, connect first"
        hasActions: false
        //positiveButtonText: "Ok"
        //negativeButtonText: "disagree"
    }


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
                    id: sText
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredWidth: 0.5 * parent.width
                    text: ""
                    placeholderText: "Search phrase"
                    onTextChanged: switchBtns()
                }

                MenuField {
                    id: sType
                    Layout.alignment: Qt.AlignLeft
                    Layout.preferredWidth: 0.5*parent.width

                    textRole: "name"
                    model: ListModel {
                        id: sTypeModel
                        ListElement {
                            name: "Any"
                            value: ""
                        }

                        ListElement {
                            name: "Archive"
                            value: "Arc"
                        }

                        ListElement {
                            name: "Audio"
                            value: "Audio"
                        }

                        ListElement {
                            name: "CD Image"
                            value: "Iso"
                        }

                        ListElement {
                            name: "Picture"
                            value: "Image"
                        }

                        ListElement {
                            name: "Program"
                            value: "Pro"
                        }

                        ListElement {
                            name: "Video"
                            value: "Video"
                        }

                        ListElement {
                            name: "Document"
                            value: "Doc"
                        }

                        ListElement {
                            name: "Emule collection"
                            value: "EmuleCollection"
                        }
                    }
                }
            }
        }

        ListItem.Standard {
            content: RowLayout {
                width: parent.width
                TextField {
                    id: sMin
                    Layout.preferredWidth: 0.5*parent.width
                    placeholderText: "Min size, Mb"
                    validator: IntValidator {}
                }

                TextField {
                    id: sMax
                    placeholderText: "Max size, Mb"
                    validator: IntValidator {}
                }
            }
        }


        ListItem.Standard {
            content: RowLayout {
                width: parent.width
                TextField {
                    id: sAvailibility
                    Layout.preferredWidth: 0.5*parent.width
                    placeholderText: "Availibility"
                    validator: IntValidator {}
                }

                TextField {
                    id: sFullSrc
                    placeholderText: "Full sources"
                    validator: IntValidator {}
                }
            }
        }

        ListItem.Standard {
            content: RowLayout {
                width: parent.width
                TextField {
                    id: sMediaLength
                    Layout.preferredWidth: 0.5*parent.width
                    placeholderText: "Media length"
                    validator: IntValidator {}
                }

                TextField {
                    id: sMediaBitrate
                    placeholderText: "Media bitrate"
                    validator: IntValidator {}
                }
            }
        }

        ListItem.Standard {
            content : TextField {
                id: sExt
                placeholderText: "Extension"
            }
        }

        ListItem.Standard {
            content : TextField {
                id: sCodec
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

            ProgressCircle {
                id: progress
                Layout.alignment: Qt.AlignCenter
                width: Units.dp(24)
                height: Units.dp(24)
                enabled: false
                visible: false
            }

            Button {
                id: btnStart
                text: "Start"
                textColor: Theme.primaryColor
                enabled: false
                onClicked: {
                    if (!session.isServerConnected()) {
                        alertNoConnection.show()
                    } else {
                        /*QString& strQuery,
                                           quint64 nMinSize,
                                           quint64 nMaxSize,
                                           unsigned int nSources,
                                           unsigned int nCompleteSources,
                                           QString strFileType,
                                           QString strFileExt,
                                           QString strMediaCodec,
                                           quint32 nMediaLength,
                                           quint32 nMediaBitrate
                                           */
                        session.searchFiles(sText.text, sMin.text, sMax.text,
                                            sAvailibility.text, sFullSrc.text,
                                            sTypeModel.get(sType.selectedIndex).value, sExt.text, sCodec.text,
                                            sMediaLength.text, sMediaBitrate.text)
                        console.log("Start search for: " + sText.text);
                        progress.enabled = true
                        progress.visible = true
                        btnStart.enabled = false
                        btnCancel.enabled = true
                        btnMore.enabled = false
                    }
                }
            }

            Button {
                id: btnMore
                text: "More"
                textColor: Theme.primaryColor;
                enabled: false
                onClicked: {
                    btnStart.enabled = false
                    btnMore.enabled = false
                    btnCancel.enabled = true
                    progress.enabled = true
                    progress.visible = true
                    session.searchMoreResults()
                }
            }

            Button {
                id: btnCancel
                text: "Cancel"
                textColor: Theme.primaryColor
                enabled: false
                onClicked: {
                    session.cancelSearch()
                    btnMore.enabled = false
                    btnStart.enabled = true
                    btnCancel.enabled = false
                    progress.enabled = false
                    progress.visible = false
                }
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
