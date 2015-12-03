import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Page {
    Dialog {
        id: alertEmptyResult
        width: Units.dp(300)
        text: qsTr("Sorry, your last search returned empty result")
        hasActions: true
        positiveButtonText: qsTr("Ok")
        negativeButton.visible: false
    }

    Connections {
        target: session
        onSearchFinished: {
            console.log("search finished, more result: " + bMoreResult)
            btnStart.enabled = true
            btnCancel.enabled = false
            btnMore.enabled = bMoreResult
            progress.enabled = false
            progress.visible = false
            if (resultsCount > 0) {
                searchFPModel.sortData()
                adCtl.hideAdMobBanner()
                pageStack.push(Qt.resolvedUrl("SearchResult.qml"))
            } else
                alertEmptyResult.show()
        }
    }

    function switchBtns() {
        btnStart.enabled = sText.text.length != 0
        btnMore.enabled=false
    }

    Dialog {
        id: alertNoConnection
        width: Units.dp(300)
        text: qsTr("You are not connected to any ED2K server. Please, connect first")
        hasActions: true
        positiveButtonText: qsTr("Ok")
        negativeButton.visible: false
    }

    Flickable {
        id: flick
        anchors.fill: parent
        contentHeight: Math.max(col.implicitHeight + Units.dp(40), col.height)
        contentWidth: parent.width //Math.max(col.implicitWidth + Units.dp(30), col.width)

         ColumnLayout {
            id: col
            anchors.fill: parent

            Label {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Units.dp(16)
                }

                style:  "body2"
                text: qsTr("Enter phrase and press start")
            }

            ListItem.Standard {
                content: RowLayout {
                    width: parent.width

                    TextField {
                        id: sText
                        Layout.alignment: Qt.AlignLeft
                        Layout.preferredWidth: 0.5 * parent.width
                        text: ""
                        placeholderText: qsTr("Search phrase")
                        onTextChanged: switchBtns()
                    }

                    MenuField {
                        id: sType
                        Layout.alignment: Qt.AlignLeft
                        Layout.preferredWidth: 0.5*parent.width

                        textRole: "name"
                        model: ListModel {
                            id: sTypeModel
                            ListElement { name: qsTr("Any"); value: "" }
                            ListElement { name: qsTr("Archive"); value: "Arc" }
                            ListElement { name: qsTr("Audio"); value: "Audio" }
                            ListElement { name: qsTr("CD Image"); value: "Iso" }
                            ListElement { name: qsTr("Picture"); value: "Image" }
                            ListElement { name: qsTr("Program"); value: "Pro" }
                            ListElement { name: qsTr("Video"); value: "Video" }
                            ListElement { name: qsTr("Document"); value: "Doc" }
                            ListElement { name: qsTr("Emule collection"); value: "EmuleCollection" }
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
                        placeholderText: qsTr("Min size, Mb")
                        validator: IntValidator {}
                    }

                    TextField {
                        id: sMax
                        placeholderText: qsTr("Max size, Mb")
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
                        placeholderText: qsTr("Availibility")
                        validator: IntValidator {}
                    }

                    TextField {
                        id: sFullSrc
                        placeholderText: qsTr("Full sources")
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
                        placeholderText: qsTr("Media length")
                        validator: IntValidator {}
                    }

                    TextField {
                        id: sMediaBitrate
                        placeholderText: qsTr("Media bitrate")
                        validator: IntValidator {}
                    }
                }
            }

            ListItem.Standard {
                content : TextField {
                    id: sExt
                    placeholderText: qsTr("Extension")
                }
            }

            ListItem.Standard {
                content : TextField {
                    id: sCodec
                    placeholderText: qsTr("Codec")
                }
            }

            RowLayout {
                id: buttonsRow
                Layout.alignment: Qt.AlignRight


                spacing: Units.dp(8)

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Units.dp(8)
                }

                ProgressCircle {
                    id: progress
                    Layout.fillWidth: false
                    Layout.alignment: Qt.AlignCenter
                    width: Units.dp(24)
                    height: Units.dp(24)
                    enabled: false
                    visible: false
                }

                Button {
                    id: btnStart
                    Layout.fillWidth: false
                    text: qsTr("Start")
                    textColor: Theme.primaryColor
                    enabled: false
                    onClicked: {
                        if (!session.isServerConnected()) {
                            alertNoConnection.show()
                        } else {

                            searchModel.clean()
                            session.searchFiles(sText.text, sMin.text*1024*1024, sMax.text*1024*1024,
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
                    Layout.fillWidth: false
                    text: qsTr("More")
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
                    Layout.fillWidth: false
                    text: qsTr("Cancel")
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
    }

    Scrollbar {
        flickableItem: flick
    }
}


