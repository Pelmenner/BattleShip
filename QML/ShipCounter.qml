import QtQuick 2.0
import QtQuick.Layouts 1.0
import backend.Field 1.0

Item {
    id: shipCounter

    property BackendField backendField: backend.curField


    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        ShipCounterRow {
            id: row1
            index: 0
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ShipCounterRow {
            id: row2
            index: 1
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ShipCounterRow {
            id: row3
            index: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        ShipCounterRow {
            id: row4
            index: 3
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }



}
