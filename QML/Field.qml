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
            left:nameBox.left; right: nameBox.right}

        columnSpacing: 0
        rowSpacing: 0

        Cell{ indexX: 0; indexY: 0; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 0; indexY: 1; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 0; indexY: 2; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 0; indexY: 3; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 0; indexY: 4; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 0; indexY: 5; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 0; indexY: 6; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 0; indexY: 7; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 0; indexY: 8; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 0; indexY: 9; Layout.fillHeight: true; Layout.fillWidth: true}

        Cell{ indexX: 1; indexY: 0; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 1; indexY: 1; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 1; indexY: 2; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 1; indexY: 3; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 1; indexY: 4; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 1; indexY: 5; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 1; indexY: 6; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 1; indexY: 7; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 1; indexY: 8; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 1; indexY: 9; Layout.fillHeight: true; Layout.fillWidth: true}

        Cell{ indexX: 2; indexY: 0; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 2; indexY: 1; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 2; indexY: 2; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 2; indexY: 3; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 2; indexY: 4; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 2; indexY: 5; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 2; indexY: 6; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 2; indexY: 7; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 2; indexY: 8; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 2; indexY: 9; Layout.fillHeight: true; Layout.fillWidth: true}

        Cell{ indexX: 3; indexY: 0; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 3; indexY: 1; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 3; indexY: 2; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 3; indexY: 3; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 3; indexY: 4; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 3; indexY: 5; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 3; indexY: 6; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 3; indexY: 7; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 3; indexY: 8; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 3; indexY: 9; Layout.fillHeight: true; Layout.fillWidth: true}

        Cell{ indexX: 4; indexY: 0; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 4; indexY: 1; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 4; indexY: 2; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 4; indexY: 3; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 4; indexY: 4; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 4; indexY: 5; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 4; indexY: 6; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 4; indexY: 7; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 4; indexY: 8; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 4; indexY: 9; Layout.fillHeight: true; Layout.fillWidth: true}

        Cell{ indexX: 5; indexY: 0; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 5; indexY: 1; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 5; indexY: 2; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 5; indexY: 3; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 5; indexY: 4; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 5; indexY: 5; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 5; indexY: 6; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 5; indexY: 7; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 5; indexY: 8; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 5; indexY: 9; Layout.fillHeight: true; Layout.fillWidth: true}

        Cell{ indexX: 6; indexY: 0; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 6; indexY: 1; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 6; indexY: 2; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 6; indexY: 3; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 6; indexY: 4; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 6; indexY: 5; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 6; indexY: 6; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 6; indexY: 7; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 6; indexY: 8; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 6; indexY: 9; Layout.fillHeight: true; Layout.fillWidth: true}

        Cell{ indexX: 7; indexY: 0; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 7; indexY: 1; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 7; indexY: 2; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 7; indexY: 3; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 7; indexY: 4; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 7; indexY: 5; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 7; indexY: 6; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 7; indexY: 7; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 7; indexY: 8; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 7; indexY: 9; Layout.fillHeight: true; Layout.fillWidth: true}

        Cell{ indexX: 8; indexY: 0; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 8; indexY: 1; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 8; indexY: 2; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 8; indexY: 3; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 8; indexY: 4; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 8; indexY: 5; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 8; indexY: 6; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 8; indexY: 7; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 8; indexY: 8; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 8; indexY: 9; Layout.fillHeight: true; Layout.fillWidth: true}

        Cell{ indexX: 9; indexY: 0; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 9; indexY: 1; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 9; indexY: 2; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 9; indexY: 3; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 9; indexY: 4; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 9; indexY: 5; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 9; indexY: 6; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 9; indexY: 7; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 9; indexY: 8; Layout.fillHeight: true; Layout.fillWidth: true}
        Cell{ indexX: 9; indexY: 9; Layout.fillHeight: true; Layout.fillWidth: true}

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
