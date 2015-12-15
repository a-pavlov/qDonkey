import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

Dialog {
    id: addLink
    anchors.centerIn: parent
    width: Units.dp(350)
    hasActions: true
    positiveButtonText: qsTr("Ok")
    negativeButton.visible: false
    text: qsTr("Download %1").arg(act.link)
}
