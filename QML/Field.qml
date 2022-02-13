import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.12
import backend.Field 1.0

Item{
    id: field
    objectName: "field"

    property BackendField backendField: backend.curField
    property bool isHealthBarShown: false
    property bool isActive: false
    property bool isWinner: false

    signal cellClicked(int x, int y)
    signal cellRClicked(int x, int y)

    width: Math.min(parent.height / 1.1, (parent.width - anchors.leftMargin - anchors.rightMargin))
    height: Math.min(parent.height, (parent.width - anchors.leftMargin - anchors.rightMargin) * 1.1)

    Item{
        id: nameBox
        anchors.top: field.top
        width: field.width
        height: field.height / 11

        Text {
            id: playerName
            text: isWinner? backendField.name + " won!" : backendField.name
            color: isActive? Qt.lighter("#FF851B") : "#FF851B"
            font.pointSize: nameBox.height / 2

            anchors{
                verticalCenter: nameBox.verticalCenter
                horizontalCenter: nameBox.horizontalCenter
            }

            Behavior on color{
                ColorAnimation {
                    duration: 75
                }
            }
        }
    }

    GridLayout{
        id: grid
        columns: 10
        rows: 10 + isHealthBarShown
        anchors{
            top: nameBox.bottom; bottom: field.bottom;
            left:nameBox.left; right: nameBox.right
        }

        columnSpacing: 0
        rowSpacing: 0

        Repeater{ // much slower than 100 hard-coded
            id: rowRepeater
            model: 10

            Repeater{
                id: columnRepeater
                property int rowIndex: index
                model: 10
                Cell{ indexX: columnRepeater.rowIndex; indexY: index; Layout.fillHeight: true; Layout.fillWidth: true}
            }
        }

        ProgressBar{
            id: healthBar
            visible: isHealthBarShown
            Layout.columnSpan: 10
            Layout.fillWidth: true

            value: backendField.health / 20

            Behavior on value{
                PropertyAnimation{
                    duration: 100
                }
            }
        }
    }
}
