import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Page {
    title: qsTr("Configure application")

    Component.onCompleted: {
    }

    function getLangIndex(locale) {
        var i;
        for(i = 0; i < langModel.count; ++i) {
            if (langModel.get(i).key === pref.locale) return i
        }

        return 0;
    }

    Dialog {
        id: langRestart
        hasActions: true
        positiveButtonText: qsTr("Ok")
        negativeButton.visible: false
        text: qsTr("New language will be available after restart")
    }

    Flickable {
        id: flick
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: col.implicitHeight + Units.dp(40)

        ColumnLayout {
            id: col
            anchors.fill: parent

            Label {

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Units.dp(16)
                }

                style: "body2"
                text: qsTr("Change preferences here and click apply")
            }

            ListItem.Standard {
                action: Icon {
                    anchors.centerIn: parent
                    name: "social/people_outline"
                }

                content: RowLayout {
                    Label {
                        text: qsTr("Kademlia prefs")
                    }

                    Button {
                        id: btnKad
                        text: qsTr(">>>")
                        textColor: Theme.primaryColor
                        enabled: true
                        onClicked: {
                            pageStack.push(Qt.resolvedUrl("Kademlia.qml"))
                        }
                    }
                }
            }

            ListItem.Standard {
                action: Icon {
                    anchors.centerIn: parent
                    name: "action/language"
                }

                content: MenuField {
                    id: language
                    Layout.alignment: Qt.AlignLeft
                    width: parent.width

                    selectedIndex: getLangIndex(pref.locale)

                    textRole: "value"

                    model: ListModel {
                        id: langModel
                        ListElement { key: "en"; value: qsTr("English");  }
                        ListElement { key: "ru"; value: qsTr("Russian");  }
                    }

                    onItemSelected: {
                        langRestart.show()
                        pref.locale=langModel.get(index).key
                    }
                }
            }

            ListItem.Standard {
                action: Icon {
                    anchors.centerIn: parent
                    source: Qt.resolvedUrl("qrc:/images/account.svg")
                }

                content: TextField {
                    id: nick
                    anchors.centerIn: parent
                    width: parent.width
                    floatingLabel: true
                    placeholderText: qsTr("Enter your nickname")
                    text: pref.nick
                    onTextChanged: pref.nick = text
                }
            }

            ListItem.Standard {

                action: Icon {
                    anchors.centerIn: parent
                    source: Qt.resolvedUrl("qrc:/images/access-point-network.svg")
                }

                content: TextField {
                    id: listenPort
                    anchors.centerIn: parent
                    width: parent.width
                    floatingLabel: true
                    placeholderText: qsTr("Listen port")
                    text: pref.listenPort
                    validator: IntValidator {}
                    maximumLength: 5
                    onTextChanged: pref.listenPort=text
                }
            }

            ListItem.Standard {
                action: Item {}
                content: RowLayout {
                    Switch {
                        id: upnpEnabled
                        checked: pref.upnpEnabled
                        enabled: true
                        darkBackground: false
                        onCheckedChanged: pref.upnpEnabled=checked
                    }

                    Label {
                        text: qsTr("UPnP port on router")
                        wrapMode: Text.WordWrap
                    }
                }
            }

            ListItem.Standard {
                height: grid.height + Units.dp(8)
                implicitHeight: grid.implicitHeight + Units.dp(8)

                action: Icon {
                    anchors.centerIn: parent
                    source: Qt.resolvedUrl("qrc:/images/speedometer.svg")
                }

                content: GridLayout {
                    id: grid
                    columns: 2
                    anchors.centerIn: parent
                    rowSpacing: Units.dp(1)
                    columnSpacing: Units.dp(10)

                    CheckBox {
                        id: checkDownload
                        checked: pref.dlSpeedLimited
                        text: qsTr("Down")
                        onCheckedChanged: {
                            limitDownload.enabled=checked
                            pref.dlSpeedLimit=checked
                        }
                    }

                    TextField {
                        id: limitDownload
                        enabled: pref.dlSpeedLimited
                        width: parent.width
                        floatingLabel: true
                        placeholderText: qsTr("Dowload limit Kb/s")
                        text: pref.dlSpeed
                        validator: IntValidator {}
                        onTextChanged: pref.dlSpeed=text
                    }

                    CheckBox {
                        id: checkUpload
                        Layout.alignment: Qt.AlignVCenter
                        checked: pref.upSpeedLimited
                        text: qsTr("Up")
                         onCheckedChanged: {
                            limitUpload.enabled=checked
                            pref.dlSpeedLimited=checked
                        }
                    }

                    TextField {
                        id: limitUpload
                        enabled: pref.upSpeedLimited
                        width: parent.width
                        floatingLabel: true
                        placeholderText: qsTr("Upload limit Kb/s")
                        text: pref.upSpeed
                        validator: IntValidator {}
                        onTextChanged: pref.upSpeed=text
                    }
                }
            }

            ListItem.Standard {
                action: Icon {
                    anchors.centerIn: parent
                    source: Qt.resolvedUrl("qrc:/images/folder-download.svg")
                }

                content: TextField {
                    id: inputDir
                    anchors.centerIn: parent
                    width: parent.width
                    floatingLabel: true
                    placeholderText: qsTr("Incoming directory")
                    text: pref.inputDir
                    onTextChanged: pref.inputDir=text
                }
            }

            ListItem.Standard {
                action: Item {}
                content: RowLayout {
                    Switch {
                        id: showPreviusTransfers
                        checked: pref.showAllTransfers
                        enabled: true
                        darkBackground: false
                        onCheckedChanged: pref.showAllTransfers=checked
                    }

                    Label {
                        text: qsTr("Show old transfers")
                    }
                }
            }

            ListItem.Standard {
                action: Item {}
                content: RowLayout {
                    Switch {
                        id: askOnExit
                        checked: pref.askOnExit
                        enabled: true
                        darkBackground: false
                        onCheckedChanged: pref.askOnExit=checked
                    }

                    Label {
                        text: qsTr("Ask before exit program")
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: Units.dp(8)
            }
        }
    }

    Scrollbar {
        flickableItem: flick
    }
}
