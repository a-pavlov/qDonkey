import QtQuick 2.2
import Material 0.1
import Material.ListItems 0.1 as ListItem

TabbedPage {
    id: page
    title: "Page Title that is very long to demonstrate eliding titles in ActionBar"

    Tab {
        title: "Overview"
        iconName: "action/home"

        Rectangle {
            color: Palette.colors.red["200"]

            Button {
                anchors.centerIn: parent
                darkBackground: true
                text: "Go to tab 3"
                onClicked: page.selectedTab = 2
            }
        }
    }

    Tab {
        title: "Projects"

        Rectangle { color: Palette.colors.purple["200"] }
    }

    Tab {
        title: "Inbox"

        Rectangle { color: Palette.colors.orange["200"] }
    }
}
