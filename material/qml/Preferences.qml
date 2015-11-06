import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Page {

    function btnOn() {
        btnApply.enabled=true
        btnCancel.enabled=true
    }

    Component.onCompleted: {
        btnApply.enabled=false
        btnCancel.enabled=false
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
        hasActions: false
        text: qsTr("New language will be available after restart")
    }

    Flickable {
        id: flick
        anchors.fill: parent
        contentWidth: parent.width
        //width: parent.width
        //height: parent.height-buttonsRow.implicitHeight
        contentHeight: col.implicitHeight + Units.dp(40)

        ColumnLayout {
            id: col

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

                style: "title"
                text: qsTr("Preferences")
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
                        btnOn()
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
                    onTextChanged: btnOn()
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
                    onTextChanged: btnOn()
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
                            btnOn()
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
                        onTextChanged: btnOn()
                    }

                    CheckBox {
                        id: checkUpload
                        Layout.alignment: Qt.AlignVCenter
                        checked: pref.upSpeedLimited
                        text: qsTr("Up")
                         onCheckedChanged: {
                            limitUpload.enabled=checked
                             btnOn()
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
                        onTextChanged: btnOn()
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
                    onTextChanged: btnOn()
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
                        onCheckedChanged: btnOn()
                    }

                    Label {
                        text: qsTr("Show old transfers")
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: Units.dp(8)
            }

            RowLayout {
                id: buttonsRow
                Layout.alignment: Qt.AlignRight
                spacing: Units.dp(8)

                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Units.dp(8)
                }

                Button {
                    id: btnCancel
                    text: qsTr("Cancel")
                    textColor: Theme.primaryColor
                    enabled: false
                    onClicked: {
                        nick.text = pref.nick
                        listenPort.text= pref.listenPort
                        checkDownload.checked=pref.dlSpeedLimited
                        limitDownload.text=pref.dlSpeed
                        checkUpload.checked=pref.upSpeedLimited
                        limitUpload.text=pref.upSpeed
                        inputDir.text=pref.inputDir
                        showPreviusTransfers.checked=pref.showAllTransfers
                        language.selectedIndex=getLangIndex(pref.locale)
                        btnCancel.enabled=false
                        btnApply.enabled=false
                    }
                }

                Button {
                    id: btnApply
                    text: qsTr("Apply")
                    textColor: Theme.primaryColor
                    enabled: false
                    onClicked: {
                        console.log("write settings")
                        pref.nick = nick.text
                        pref.listenPort = listenPort.text
                        pref.dlSpeedLimited = checkDownload.checked
                        pref.dlSpeed = limitDownload.text
                        pref.upSpeedLimited = checkUpload.checked
                        pref.upSpeed = limitUpload.text
                        pref.inputDir=inputDir.text
                        pref.showAllTransfers=showPreviusTransfers.checked
                        pref.locale=langModel.get(language.selectedIndex).key
                        pref.flush()
                        btnCancel.enabled=false
                        btnApply.enabled=false
                    }
                }
            }
        }
    }

    Scrollbar {
        flickableItem: flick
    }
}
