import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: opponentSelectPage
    objectName: "opponentSelectPage"

    signal randomClicked(string serveraddress, string serverPort)
    signal friendClicked(string serveraddress, string serverPort)

    Button{
        id: randomButton
        text: "Random opponent"
        anchors.horizontalCenter: opponentSelectPage.horizontalCenter
        anchors.bottom: opponentSelectPage.verticalCenter
        onClicked: opponentSelectPage.randomClicked(addressField.text, portField.text)
    }

    Button{
        id: friendButton
        text: "Play with friend"
        anchors.horizontalCenter: opponentSelectPage.horizontalCenter
        anchors.top: randomButton.bottom
        anchors.topMargin: 10
        width: randomButton.width
        onClicked: opponentSelectPage.friendClicked(addressField.text, portField.text)
    }

    TextField{
        id: addressField
        placeholderText: "server address"
        anchors.top: friendButton.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: opponentSelectPage.horizontalCenter
    }

    TextField{
        id: portField
        placeholderText: "server port"
        anchors.top: addressField.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: opponentSelectPage.horizontalCenter
        validator: IntValidator {bottom: 1; top: 10000}
    }
}
