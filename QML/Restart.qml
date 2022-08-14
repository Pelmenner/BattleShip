import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0

Item {
    id: restartPage

    property int fontSize: height / 40
    property bool landscape: width > height

    signal restartClicked(bool saveNames)
    signal homeClicked()
    signal exitClicked()

    Component.onCompleted: {
        restartClicked.connect(backend.restartGame)
        homeClicked.connect(backend.returnHome)
        exitClicked.connect(backend.exit)
    }

    Text{
        id: restartText
        anchors.horizontalCenter: restartPage.horizontalCenter
        anchors.top: restartPage.top
        anchors.topMargin: restartPage.height / 10
        text: "Restart"
        font.pointSize: restartPage.fontSize
        color: Material.foreground
    }

    Text{
        id: saveNamesText
        anchors.horizontalCenter: restartPage.horizontalCenter
        anchors.top: restartPage.top
        anchors.topMargin: restartPage.height / 4
        text: "Save player's names?"
        font.pointSize: restartPage.fontSize
        color: Material.foreground
    }

    ButtonGroup { buttons: radioButtonColumn.children }

    Column {
        id: radioButtonColumn
        anchors.left: restartPage.left
        anchors.leftMargin: restartPage.width / 3
        anchors.top: restartPage.top
        anchors.topMargin: restartPage.height / 3

        width: restartPage.width / 3
        height: restartPage.height / 3

        RadioButton {
            id: yesButton
            checked: true
            text: "Yes"
            spacing: restartPage.width / 20

            contentItem: Text {
                text: yesButton.text               
                font.pointSize: fontSize
                color: Material.foreground
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: yesButton.spacing
                anchors.left: yesButton.indicator.right
            }
        }

        RadioButton {
            id: noButton
            text: "No"
            spacing: restartPage.width / 20

            contentItem: Text {
                text: noButton.text
                font.pointSize: fontSize
                color: Material.foreground
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: noButton.spacing
                anchors.left: noButton.indicator.right
            }
        }
    }

    Column{
        id: buttonColumn
        anchors.horizontalCenter: restartPage.horizontalCenter
        anchors.bottom: restartPage.bottom
        height: restartPage.landscape? restartPage.height / 2 : restartPage.height / 3
        spacing: restartPage.height / 40

        Button{
            id: restartButton
            anchors.horizontalCenter: buttonColumn.horizontalCenter
            width: restartPage.width / 2
            height: buttonColumn.height / 5
            text: "Restart"
            onClicked: restartPage.restartClicked(yesButton.checked)
        }
        Button{
            id: mainMenuButton
            anchors.horizontalCenter: buttonColumn.horizontalCenter
            width: restartPage.width / 2
            height: buttonColumn.height / 5
            text: "Main menu"
            onClicked: restartPage.homeClicked()
        }
        Button{
            id: exitButton
            anchors.horizontalCenter: buttonColumn.horizontalCenter
            width: restartPage.width / 2
            height: buttonColumn.height / 5
            text: "Exit"
            onClicked: restartPage.exitClicked()
        }
    }

}
