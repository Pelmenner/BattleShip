import QtQuick 2.0
import QtQuick.Layouts 1.0
import backend.Field 1.0

Item {
    id: shipCounter

    property BackendField backendField: backend.curField

    ListView{
        id: view
        anchors.fill: parent
        model: backendField.shipCounter
        verticalLayoutDirection: ListView.BottomToTop

        delegate: ShipCounterRow {
            width: view.width
            height: view.height / view.count
        }
    }

    Component.onCompleted: console.log(backendField.shipCounter)
}
