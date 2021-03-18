import QtQuick 2.12
import QtQuick.Controls 2.2

Item {
    id: waitingPage

    BusyIndicator
    {
        id: busyIndicator
        anchors.horizontalCenter: waitingPage.horizontalCenter
        anchors.verticalCenter: waitingPage.verticalCenter
        running: true
    }

    Text {
        id: text
        text: "Waiting..."
        color: "white"
        anchors.horizontalCenter: waitingPage.horizontalCenter
        anchors.bottom: busyIndicator.top
        anchors.bottomMargin: 20
    }
}
