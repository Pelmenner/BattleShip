import QtQuick 2.12
import QtQuick.Window 2.12
import QtQml.Models 2.1
import backend.Cell 1.0
import QtQuick.Controls.Material 2.0
import "QML"

Window {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 450
    height: 950
    title: qsTr("Battleship")
    color: Material.background

    signal loadCompleted()

    Material.theme: Material.Dark
    Material.foreground: Material.theme == Material.Dark? 'white' : 'black'

    Loader{
        id: loader
        anchors.fill: parent
        sourceComponent: homePage
        asynchronous: true
        opacity: 0
        onStatusChanged: {
            if (loader.status == Loader.Ready){
                mainWindow.loadCompleted();
                fadeInAnimation.start();
            }
        }

        property var targetComponent: sourceComponent

        SequentialAnimation{
            id: fadeOutAnimation
            NumberAnimation {
                target: loader
                property: "opacity"
                to: 0
                duration: 200
            }
            ScriptAction {script: loader.sourceComponent = loader.targetComponent}
        }

        NumberAnimation {
            id: fadeInAnimation
            target: loader
            property: "opacity"
            to: 1
            duration: 200
        }

        onTargetComponentChanged: fadeOutAnimation.start();

    }

    Component{
        id: init
        FieldInitializer{
            objectName: "initializer"
            width: mainWindow.width
            height: mainWindow.height
        }
    }

    Component{
        id: play
        Game{
            objectName: "game"
            width: mainWindow.width
            height: mainWindow.height
        }
    }

    Component{
        id: homePage
        HomePage{
            objectName: "homePage"
            width: mainWindow.width
            height: mainWindow.height
        }
    }

    Component{
        id: namesInput
        NamesInput{
            objectName: "namesInput"
            width: mainWindow.width
            height: mainWindow.height
        }
    }

    Component{
        id: restartPage
        Restart{
            objectName: "restartPage"
            width: mainWindow.width
            height: mainWindow.height
        }
    }

    Component{
        id: opponentSelectPage
        OpponentSelectPage{
            objectName: "OpponentSelectPage"
            width: mainWindow.width
            height: mainWindow.height
        }
    }

    function changeLoaderComponent(newComponent){
        loader.targetComponent = newComponent
    }

    Connections {
        target: backend

        onNameInputStarted: {
            changeLoaderComponent(namesInput)
        }
        onLocalGameStarted: {
            changeLoaderComponent(play)
        }
        onInitializationStarted: {
            changeLoaderComponent(init)
        }
        onGameClosed: {
            changeLoaderComponent(restartPage)
        }
        onReturnedHome: {
            changeLoaderComponent(homePage)
        }
        onOpponentSelectionStarted: {
            changeLoaderComponent(opponentSelectPage)
        }
    }
}
