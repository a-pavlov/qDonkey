import QtQuick 2.2
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

    property string conn: "Connection"
    property string transfers: "Transfers"
    property string search: "Search"

    property var sections: [ conn, transfers, search ]

    property var sectionTitles: [ "Connection", "Transfers", "Search" ]

    property string selectedComponent: conn

    initialPage: TabbedPage {
        id: page

        title: "qDonkey"

        actionBar.maxActionCount: navDrawer.enabled ? 1 : 3

        actions: [
            Action {
                iconName: "action/settings"
                name: "Settings"
                hoverAnimation: true
            },

            Action {
                iconName: "action/language"
                name: "Language"
                enabled: false
            }
        ]


        backAction: navDrawer.action

        NavigationDrawer {
            id: navDrawer

            enabled: page.width < Units.dp(500)

            Flickable {
                anchors.fill: parent
                contentHeight: Math.max(content.implicitHeight, height)

                Column {
                    id: content
                    anchors.fill: parent

                    Repeater {
                        model: sections

                        delegate: Column {
                            width: parent.width

                            ListItem.Standard {
                                text: sectionTitles[index]
                                selected: modelData = qDonkey.selectedComponent
                                onClicked: {
                                    console.log(Qt.resolvedUrl("model data % selected").arg(modelData));
                                    qDonkey.selectedComponent = modelData
                                    navDrawer.close()
                                }
                            }
/*
                            Repeater {
                                model: modelData
                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == qDonkey.selectedComponent
                                    onClicked: {
                                        console.log(Qt.resolvedUrl("model data % selected").arg(modelData));
                                        qDonkey.selectedComponent = modelData
                                        navDrawer.close()
                                    }
                                }
                            }*/
                        }

                    }
                }

            }

        }


        Repeater {
            model: !navDrawer.enabled ? sections : 0

            delegate: Tab {
                title: sectionTitles[index]

                property string selectedComponent: modelData
                property var section: modelData

                sourceComponent: tabDelegate
            }
        }

        Loader {
            anchors.fill: parent
            sourceComponent: tabDelegate

            property var section: []
            visible: navDrawer.enabled
            onLoaded: {
                console.log(source)
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
                        if (navDrawer.enabled) {
                            console.log(Qt.resolvedUrl("% clicked").arg(qDonkey.selectedComponent.replace(" ", "")))
                            return Qt.resolvedUrl("%.qml").arg(qDonkey.selectedComponent.replace(" ", ""))
                        } else {
                            console.log(Qt.resolvedUrl("% clicked").arg(selectedComponent.replace(" ", "")))
                            return Qt.resolvedUrl("%.qml").arg(selectedComponent.replace(" ", ""))
                        }
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

}
