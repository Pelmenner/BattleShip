import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    id: fieldInitializer

    signal okButtonClicked()
    signal randomButtonClicked()
    signal clearButtonClicked()

    property bool landscape: width > height

    Component.onCompleted: {
        okButtonClicked.connect(backend.okClicked)
        randomButtonClicked.connect(backend.randomClicked)
        clearButtonClicked.connect(backend.clearClicked)
    }

    Field {
        id:field

        anchors{leftMargin: 10; left: fieldInitializer.left}
        width: fieldInitializer.landscape? Math.min(fieldInitializer.height / 1.1, fieldInitializer.width / 1.5 - anchors.leftMargin * 2) :
                                           Math.min(fieldInitializer.width - anchors.leftMargin * 2, fieldInitializer.height / 1.5 / 1.1 - 10)
        height: width * 1.1

        Component.onCompleted: {
            cellClicked.connect(backend.fieldInitClicked)
            cellRClicked.connect(backend.fieldInitRClicked)
        }
    }

    Column{
        id: buttonColumn

        states: [
            State{
                name: "landscape"
                AnchorChanges {
                    target: buttonColumn; anchors{
                        left: field.right
                        right: fieldInitializer.right
                        top: fieldInitializer.top
                        bottom: fieldInitializer.bottom
                    }
                }
            },

            State{
                name: "portrait"
                AnchorChanges {
                    target: buttonColumn; anchors{
                        left: fieldInitializer.left
                        right: field.right
                        top: field.bottom
                        bottom: fieldInitializer.bottom
                    }
                }
            }
        ]

        state: fieldInitializer.landscape? "landscape" : "portrait"

        ShipCounter{
            width: buttonColumn.width
            height: buttonColumn.height * 5 / 8
        }

        RowLayout {
            id: buttonRowLayout
            anchors.horizontalCenter: buttonColumn.horizontalCenter
            height: buttonColumn.height / 6
            width: buttonColumn.width

            Button {
                id: clearButton
                text: "Clear"
                Layout.maximumWidth: 150; Layout.fillWidth: true
                Layout.alignment: Qt.AlignRight

                onClicked: fieldInitializer.clearButtonClicked()
            }
            Button {
                id: randomButton
                text: "Random"
                Layout.maximumWidth: 150; Layout.fillWidth: true

                onClicked: fieldInitializer.randomButtonClicked()
            }
        }

        Button{
            id: okButton
            anchors.horizontalCenter: buttonColumn.horizontalCenter
            width: clearButton.width * 2 + buttonRowLayout.spacing
            height: clearButton.height

            visible: backend.curField.filled
            text: "Ok"

            onClicked: fieldInitializer.okButtonClicked()
        }
    }

}
