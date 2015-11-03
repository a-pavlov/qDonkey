import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

ApplicationWindow {
    id: qDonkey
    title: "qDonkey"
    width:  400
    visible: true


    PrefDialog {
        id: prefDialog
    }

    InitDialog {
        id: initDialog
    }

    theme {
        primaryColor: Palette.colors["blue"]["500"]
        primaryDarkColor: Palette.colors["blue"]["700"]
        accentColor: Palette.colors["red"]["A200"]
        tabHighlightColor: "white"
    }

    Component.onCompleted: {
        if (pref.inputDir.length === 0) initDialog.show()
    }

    property string connections: "Connection"
    property string transfers: "Transfers"
    property string search: "Search"

    property var sections: [ connections, transfers, search ]
    property var sectionTitles: [ connections, transfers, search ]
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

    function switchBtns() {
        console.log("swith btns: " + inputDirEdit.text)
        btnInputOk.enabled=inputDirEdit.text.length != 0
    }
}
