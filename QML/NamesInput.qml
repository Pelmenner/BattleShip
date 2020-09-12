import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0

Item {
    id: namesInput

    signal completed()

    property bool landscape: width > height
    property string name1: player1Input.text
    property string name2: player2Input.text

    Text{
        id: player1Text

        anchors{
            bottom: player1InputRect.top
            left: player1InputRect.left
            bottomMargin: 10
        }
        text: "First player's name:"
        color: Material.foreground
        font.pointSize: player1InputRect.height / 3
    }

    Text{
        id: player2Text

        anchors{
            bottom: player2InputRect.top;
            left: player2InputRect.left
            bottomMargin: 10
        }
        text: "Second player's name:"
        color: Material.foreground
        font.pointSize: player2InputRect.height / 3
    }

    Rectangle{
        id: player1InputRect

        anchors{
            horizontalCenter: namesInput.horizontalCenter
            top: namesInput.top
            topMargin: namesInput.height / 5
        }
        width: namesInput.landscape ? namesInput.width / 1.5 : namesInput.width - 10
        height: namesInput.height / 16

        color: Material.background

        radius: 10
        border{
            color: Material.foreground
            width: 1
        }

        opacity: player1Input.activeFocus? 1 : 0.5
        TextInput{
            id: player1Input

            anchors{fill: player1InputRect; leftMargin: 10}
            color: Material.foreground
            font.pointSize: height / 2
            maximumLength: 20
            verticalAlignment: TextInput.AlignVCenter

            Component.onCompleted: forceActiveFocus()
            onAccepted: player2Input.forceActiveFocus()
        }
    }

    Rectangle{
        id: player2InputRect

        anchors{
            horizontalCenter: namesInput.horizontalCenter
            top: player1InputRect.bottom
            topMargin: namesInput.height / 5
        }
        width: namesInput.landscape ? namesInput.width / 1.5 : namesInput.width - 10
        height: namesInput.height / 16

        color: Material.background

        radius: 10
        border{
            color: Material.foreground
            width: 1
        }

        opacity: player2Input.activeFocus? 1 : 0.5

        TextInput{
            id: player2Input
            anchors{fill: player2InputRect; leftMargin: 10}
            color: Material.foreground
            font.pointSize: height / 2
            maximumLength: 20
            verticalAlignment: TextInput.AlignVCenter

            onAccepted: okButton.forceActiveFocus()
        }
    }

    Text{
        id: warningText

        anchors{
            horizontalCenter: namesInput.horizontalCenter
            bottom: namesInput.bottom
            bottomMargin: namesInput.height / 4
        }

        text: {player1Input.text.length == 0 ? "First player's name must not be empty" :
                                               player2Input.text.length == 0 ? "Second player's name must not be empty" :
                                                                               "Names must not match"}
        color: Material.color(Material.Red)
        font.pointSize: namesInput.landscape? namesInput.height / 40 : namesInput.width / 40
        opacity: player1Input.text.length == 0 || player2Input.text.length == 0 || player1Input.text == player2Input.text

        Behavior on opacity {
            NumberAnimation{
                duration: 150
            }
        }
    }

    Button{
        id: okButton

        anchors{
            horizontalCenter: namesInput.horizontalCenter
            bottom: namesInput.bottom
            bottomMargin: namesInput.height / 12
        }
        width: namesInput.width / 4
        height: namesInput.landscape? namesInput.height / 10 : namesInput.height / 15
        enabled: warningText.opacity == 0
        onClicked: namesInput.completed()
        text: "OK"
    }
}
