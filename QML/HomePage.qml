import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: homePage

    signal playClicked()

    Button{
        id: playButton
        anchors.verticalCenter: homePage.verticalCenter
        anchors.horizontalCenter: homePage.horizontalCenter
        text: "Play"

        onClicked: homePage.playClicked()
    }
}
