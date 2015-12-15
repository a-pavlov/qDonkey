import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.1

Dialog {
    id: linkAdded
    text: qsTr("Link %1 added").arg(link_filename)
    width: Units.dp(300)
    hasActions: true
    positiveButtonText: qsTr("Ok")
    negativeButton.visible: false
}
