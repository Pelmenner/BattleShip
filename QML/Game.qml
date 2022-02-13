import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id: game
    objectName: "game"

    signal finished()

    property bool landscape: width > height

    Field{
        id: field1
        objectName: "field1"

        anchors {
            left: game.landscape? game.left : undefined
            horizontalCenter: game.landscape? undefined : game.horizontalCenter
            leftMargin: 10
        }

        backendField: backend.firstField
        isHealthBarShown: true
        isActive: backendField == backend.curField
        isWinner: backend.secondField.health == 0

        width: game.landscape? Math.min(game.height / 1.1, game.width / 2 - anchors.leftMargin * 2) :
                               Math.min(game.width, game.height / 2 / 1.1 - 10)
        height: width * 1.1
    }

    Field{
        id: field2
        objectName: "field2"

        anchors{
            right: game.landscape? game.right : undefined
            horizontalCenter: game.landscape? undefined : game.horizontalCenter
            top: game.landscape ? game.top : field1.bottom
            rightMargin: 10
        }

        backendField: backend.secondField
        isHealthBarShown: true
        isActive: backendField == backend.curField
        isWinner: backend.firstField.health == 0

        width: field1.width
        height: field1.height
    }

    MouseArea{
        id: mouseArea
        anchors.fill: game
        enabled: backend.firstField.health == 0 || backend.secondField.health == 0
        onClicked: game.finished()
    }
}
