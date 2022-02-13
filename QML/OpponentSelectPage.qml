import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: opponentSelectPage
    objectName: "opponentSelectPage"

    signal randomClicked(string serverAddress, string serverPort, string playerName)
    signal createClicked(string serverAddress, string serverPort, string playerName)
    signal joinClicked(string serverAddress, string serverPort, string playerName, string room);

    Keys.onReleased: {
        if (event.key === Qt.Key_Back) {
            backend.returnHome();
            event.accepted = true
        }
    }

    Button{
        id: randomButton
        text: "Random opponent"
        anchors.horizontalCenter: opponentSelectPage.horizontalCenter
        anchors.bottom: opponentSelectPage.verticalCenter
        onClicked: opponentSelectPage.randomClicked(addressField.text, portField.text, nameField.text)
    }

    Button{
        id: createButton
        text: "Create room"
        anchors.horizontalCenter: opponentSelectPage.horizontalCenter
        anchors.top: randomButton.bottom
        anchors.topMargin: 10
        width: randomButton.width
        onClicked: opponentSelectPage.createClicked(addressField.text, portField.text, nameField.text)
    }

    Button{
        id: joinButton
        text: "Join room"
        anchors.horizontalCenter: opponentSelectPage.horizontalCenter
        anchors.top: createButton.bottom
        anchors.topMargin: 10
        width: randomButton.width
        onClicked: opponentSelectPage.joinClicked(addressField.text, portField.text, nameField.text, roomField.text)
    }

    TextField{
        id: nameField
        placeholderText: "name"
        anchors.top: joinButton.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: opponentSelectPage.horizontalCenter
    }

    TextField{
        id: addressField
        placeholderText: "server address"
        anchors.top: nameField.bottom
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

    TextField{
        id: roomField
        placeholderText: "room id (for join)"
        anchors.top: portField.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: opponentSelectPage.horizontalCenter
        validator: IntValidator {bottom: 1; top: 10000}
    }
}
