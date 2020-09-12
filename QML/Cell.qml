import QtQuick 2.0
import QtQuick.Window 2.12

Rectangle{
    id: cell
    property int indexX: 0
    property int indexY: 0

    radius: width / 10
    color: field.backendField.getCell(indexX, indexY).color
    BorderImage{
        anchors.fill: parent
        source: '/data/waves.png'
        visible: cell.color == '#1a1a1a'
    }

    border{
        width: 1
        color: '#0074D9'
    }

    Behavior on color{
        ColorAnimation {
            duration: 75
        }
    }

    ColorAnimation{
        id: mouseEnterAnimation
        target: cell
        property: 'border.color'
        to: '#7FDBFF'
        duration: 30
    }

    ColorAnimation{
        id: mouseExitAnimation
        target: cell
        property: 'border.color'
        to: '#0074D9'
        duration: 30
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true

        onPressAndHold: field.cellRClicked(indexX, indexY)

        onClicked: {
            if (mouse.button & Qt.RightButton) {
                field.cellRClicked(indexX, indexY)
            } else {
                field.cellClicked(indexX, indexY)
            }}

        onEntered: mouseEnterAnimation.start()
        onExited: mouseExitAnimation.start()
    }
}
