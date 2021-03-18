import QtQuick 2.0
import backend.Player 1.0

Rectangle {
    id: playerView

    property BackendPlayer backendPlayer

    radius: height / 5
    color: "transparent"
    border.width: 1
    border.color: "white"

    Text{
        id: name
        text: BackendPlayer.name
    }
}
