import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Material.Extras 0.1

Page {
    title: qsTr("Configure application")


    Connections {
        target: session
        onDownloadKadCompleted: {
            console.log("emule kad download completed");
            if (rc != 0) {
                snackbar.open(qsTr("Error on download nodes.dat: %1 at %2").arg(rc).arg(system))
            } else {
                snackbar.open(qsTr("nodes.dat succesfully downloaded"))
            }
        }
    }

    function btnOn() {
        btnApply.enabled=true
        btnCancel.enabled=true
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

    Dialog {
        id: kadHelp
        hasActions: true
        positiveButtonText: qsTr("Ok")
        negativeButton.visible: false
        text: qsTr("To start use KAD in first time you need one of two things or both:
1. specify bootstrap node(ip and port).
2. downdload nodes.dat file to your Download location from any source or click KAD download button.
Application will load nodes.dat on start KAD again. Status of Kademlia you can see on click info button.")
    }

    Dialog {
        id: kadWarn
        hasActions: true
        positiveButtonText: qsTr("Ok")
        negativeButton.visible: false
        text: qsTr("You can't start use KAD without at least one bootstrap node or nodes.dat file in Download location.
Click help button for more information.")
    }

    Dialog {
        id: kadDownload
        hasActions: true
        text: qsTr("Download nodes.dat to your download location? Previous nodes.dat will be erased.")
        positiveButtonText: qsTr("Ok")
        negativeButtonText: qsTr("Cancel")
        onAccepted: {
            session.downloadEmuleKad();
        }
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
                text: qsTr("Change preferences here")
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
                        ListElement { key: "it"; value: qsTr("Italian");  }
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
                    onTextChanged: {
                        session.toPP()
                        pref.nick = text
                    }
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
                    onTextChanged: {
                        session.toPP()
                        pref.listenPort=text
                    }
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
                        onCheckedChanged: {
                            session.toPP()
                            pref.upnpEnabled=checked
                        }
                    }

                    Label {
                        text: qsTr("Forward port on router")
                        wrapMode: Text.WordWrap
                    }
                }
            }

            ListItem.Standard {
                action: Icon {
                    anchors.centerIn: parent
                    name: "social/people_outline"
                }

                content: RowLayout {
                    Switch {
                        id: kadEnabled
                        checked: pref.kad
                        enabled: true
                        darkBackground: false 
                        onCheckedChanged: {
                            if (checked) {
                                if (session.hasPrevKadState() || session.hasInitialNodesFile() || (bootstrapHost.text.length !=0 && bootstrapPort.text.length != 0))
                                {
                                    console.log("ok, ready to enabled kad");
                                    pref.kad = true;
                                    session.startKad()
                                    // no session Properties Pending state since start kad immediately
                                }
                                else
                                {
                                    kadWarn.show()
                                    checked=false
                                }
                            }
                            else {
                                console.log("stop dht here");
                                session.stopKad();
                                pref.kad = false;
                            }
                        }
                    }

                    Label {
                        text: qsTr("KAD enabled")
                        wrapMode: Text.WordWrap
                    }

                    ActionButton {
                        iconName: "action/help_outline"
                        isMiniSize: true
                        backgroundColor: "white"
                        onClicked: {
                            kadHelp.show()
                        }
                    }

                    ActionButton {
                        iconName: "action/info_outline"
                        isMiniSize: true
                        backgroundColor: "white"
                        onClicked: {
                            pageStack.push(Qt.resolvedUrl("Kademlia.qml"))
                        }
                    }

                    ActionButton {
                        iconName: "file/file_download"
                        isMiniSize: true
                        backgroundColor: "white"
                        onClicked: kadDownload.show()
                    }
                }
            }

            ListItem.Standard {
                action: Icon {
                    anchors.centerIn: parent
                    name: "communication/call"
                }

                content: RowLayout {
                    TextField {
                        id: bootstrapHost
                        text: pref.bootstrapIP
                        Layout.fillWidth: true
                        placeholderText: qsTr("Bootstrap IP")
                        onTextChanged: {
                            pref.bootstrapIP = text
                        }
                    }

                    TextField {
                        id: bootstrapPort
                        text: pref.bootstrapPort
                        width: Units.dp(50)
                        placeholderText: qsTr("Port")
                        validator: IntValidator {}
                        onTextChanged: {
                            pref.bootstrapPort = text
                        }
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
                            pref.dlSpeedLimited=checked
                            session.toPP()
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
                        onTextChanged: {
                            pref.dlSpeed=text
                            session.toPP()
                        }
                    }

                    CheckBox {
                        id: checkUpload
                        Layout.alignment: Qt.AlignVCenter
                        checked: pref.upSpeedLimited
                        text: qsTr("Up")
                         onCheckedChanged: {
                            limitUpload.enabled=checked
                            pref.upSpeedLimited=checked
                            session.toPP()
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
                        onTextChanged: {
                            pref.upSpeed=text
                            session.toPP()
                        }
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
                    onTextChanged: {
                        pref.inputDir=text
                        session.toPP()
                    }
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

        Snackbar {
            id: snackbar
        }
    }

    Scrollbar {
        flickableItem: flick
    }
}
