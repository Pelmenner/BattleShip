import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: homePage

    signal playClicked(bool online)

    Column{
         anchors.verticalCenter: homePage.verticalCenter
         anchors.horizontalCenter: homePage.horizontalCenter
        Button{
            id: playLocalButton
            text: "Play local"
            width: playOnlineButton.width

            onClicked: homePage.playClicked(false)
        }

        Button{
            id: playOnlineButton
            text: "Play online"

            onClicked: homePage.playClicked(true)
        }
    }
}
