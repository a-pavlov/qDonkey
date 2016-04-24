import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.2

Dialog {
    id: helpDialog
    width: Units.dp(350)
    title: "Mule for Android"
    text: qsTr("This application for eDonkey2000 networks. This app is developed by one person. More information about KAD support,development and future plns at https://github.com/a-pavlov/qdonkey/wiki. You can help the development by give a good rating to the app in Google Play. Tthank you for your purchase")
    hasActions: true
    positiveButtonText: qsTr("More")
    negativeButtonText: qsTr("Ok")
    onAccepted: Qt.openUrlExternally("https://github.com/a-pavlov/qdonkey/wiki")
}
