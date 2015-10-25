import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

ApplicationWindow {
    id: qDonkey
    title: "qDonkey"
    width:  400
    visible: true

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }

    Component.onCompleted: {
        initDialog.show()
    }

    property string connections: "Connection"
    property string transfers: "Transfers"
    property string search: "Search"
    property string preferences: "Preferences"

    property var sections: [ connections, transfers, search, preferences ]
    property var sectionTitles: [ connections, transfers, search, preferences ]
    property string selectedComponent: connections

    initialPage: TabbedPage {
        id: page
        title: "qDonkey"
        actionBar.maxActionCount: 2

        actions: [
            Action {
                iconName: "action/settings"
                name: "Settings"
                hoverAnimation: true
                onTriggered: {
                    console.log("load preferences tab");
                    prefDialog.show()
                }
            },

            Action {
                iconName: "action/language"
                name: "Language"
                enabled: false
            }
        ]

        Repeater {
            model: sections
            delegate: Tab {
                title: sectionTitles[index]
                property string selectedComponent: modelData
                property string section: modelData
                sourceComponent: tabDelegate
            }
        }
    }

    Component {
        id: tabDelegate
        Item {
            Flickable {
                id: flickable
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }

                clip: true
                contentHeight: Math.max(example.implicitHeight + 40, height)

                Loader {
                    id: example
                    anchors.fill: parent
                    asynchronous: true
                    visible: status == Loader.Ready
                    // selectedComponent will always be valid, as it defaults to the first component
                    source: {
                        console.log(Qt.resolvedUrl("% clicked").arg(selectedComponent.replace(" ", "")))
                        return Qt.resolvedUrl("%.qml").arg(selectedComponent.replace(" ", ""))
                    }
                }

                ProgressCircle {
                    anchors.centerIn: parent
                    visible: example.status == Loader.Loading
                }
            }
            Scrollbar {
                flickableItem: flickable
            }
        }
    }

    Dialog {
        id: initDialog
        anchors.centerIn: parent
        width: Units.dp(350)
        height: init_column.implicitHeight + Units.dp(32)
        ColumnLayout {
            id: init_column

            anchors {
                fill: parent
                topMargin: Units.dp(16)
                bottomMargin: Units.dp(16)
            }

            Label {
                id: titleLabel

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Units.dp(16)
                }

                style: "title"
                text: "Setup incoming directory"
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: Units.dp(8)
            }

            ListItem.Standard {
                action: Icon {
                    anchors.centerIn: parent
                    source: Qt.resolvedUrl("qrc:/images/folder-download.svg")
                }

                content: TextField {
                    anchors.centerIn: parent
                    width: parent.width
                    floatingLabel: true
                    placeholderText: "Incoming directory"
                    text: "/home"
                }
            }


            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: Units.dp(8)
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: Units.dp(8)

                anchors {
                    right: parent.right
                    margins: Units.dp(16)
                }

                Button {
                    text: "Cancel"
                    textColor: Theme.primaryColor
                }

                Button {
                    text: "Ok"
                    textColor: Theme.primaryColor
                }
            }

        }
    }

    Dialog {
        id: prefDialog
        anchors.centerIn: parent

        width: Units.dp(350)
        height: column.implicitHeight + Units.dp(32)

        ColumnLayout {
            id: column

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
                text: "Preferences"
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: Units.dp(8)
            }

            ListItem.Standard {
                action: Icon {
                    anchors.centerIn: parent
                    source: Qt.resolvedUrl("qrc:/images/account.svg")
                }

                content: TextField {
                    anchors.centerIn: parent
                    width: parent.width
                    floatingLabel: true
                    placeholderText: "Enter your nickname"

                    text: "qDonkey"
                }
            }

            ListItem.Standard {

                action: Icon {
                    anchors.centerIn: parent
                    source: Qt.resolvedUrl("qrc:/images/access-point-network.svg")
                }

                content: TextField {
                    anchors.centerIn: parent
                    width: parent.width
                    floatingLabel: true
                    placeholderText: "Listen port"

                    text: "4661"
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
                        checked: true
                        enabled: true
                        text: "Down"
                        onCheckedChanged: {
                            limitDownload.enabled=checked
                        }
                    }

                    TextField {
                        id: limitDownload
                        width: parent.width
                        floatingLabel: true
                        placeholderText: "Dowload limit Kb/s"
                        text: "200"
                        validator: IntValidator {}
                    }

                    CheckBox {
                        Layout.alignment: Qt.AlignVCenter
                        checked: true
                        text: "Up"
                         onCheckedChanged: {
                            limitUpload.enabled=checked
                        }
                    }

                    TextField {
                        id: limitUpload
                        width: parent.width
                        floatingLabel: true
                        placeholderText: "Upload limit Kb/s"
                        text: "200"
                        validator: IntValidator {}
                    }
                }
            }

            ListItem.Standard {
                action: Icon {
                    anchors.centerIn: parent
                    source: Qt.resolvedUrl("qrc:/images/folder-download.svg")
                }

                content: TextField {
                    anchors.centerIn: parent
                    width: parent.width
                    floatingLabel: true
                    placeholderText: "Incoming directory"
                    text: "/home"
                }
            }

            ListItem.Standard {
                action: Item {}
                content: RowLayout {
                    Switch {
                        id: showPreviusTransfers
                        checked: false
                        enabled: true
                        darkBackground: false
                    }

                    Label {
                        text: "Show old transfers"
                    }
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: Units.dp(8)
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: Units.dp(8)

                anchors {
                    right: parent.right
                    margins: Units.dp(16)
                }

                Button {
                    text: "Cancel"
                    textColor: Theme.primaryColor
                }

                Button {
                    text: "Ok"
                    textColor: Theme.primaryColor
                }
            }
        }
    }

}
