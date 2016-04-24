import QtQuick 2.0
import QtQuick.Layouts 1.1
import Material 0.2
import QtMultimedia 5.5

Page {
    id: playerPage
    title: qsTr("Back")

    Video {
        id: player
        anchors.fill: parent
        source: session.currentMediaFile
        autoPlay: true
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            player.play()
            console.log("state: " + player.state + " status " + player.status)
        }
    }
/*
    VideoOutput {
        id: videoOutput
        source: player
        anchors.fill: parent
    }*/
}
