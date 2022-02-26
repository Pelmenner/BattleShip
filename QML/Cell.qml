import QtQuick 2.0
import QtQuick.Window 2.12
import backend.Cell 1.0

Rectangle{
    id: cell
    property int indexX: 0
    property int indexY: 0

    function toColor(level) {
        switch (level) {
        case Cell.Hit:
            return "#d50000" //red
        case Cell.Unknown:
            return "#1A1A1A" //black
        case Cell.Checked:
            return "#AAAAAA" //gray
        case Cell.AutoChecked:
            return "#AAAAAA" //gray
        case Cell.Shown:
            return "#2ECC40" //green
        case Cell.DrawStart:
            return "#B10DC9" //purple
        default:
            return "white"
        }
    }

    radius: width / 10
    color: toColor(field.backendField.getCell(indexX, indexY).state)

    BorderImage{
        anchors.fill: parent
        source: 'qrc:///res/waves.png'
        visible: field.backendField.getCell(indexX, indexY).state == Cell.Unknown
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
