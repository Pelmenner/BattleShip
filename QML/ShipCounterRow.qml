import QtQuick 2.0
import QtQuick.Controls.Material 2.0

Item {
    id: row

    property int index: 0
    opacity: rowText.text == "x0" ? 0.5 : 1

    Text{
        id: rowText

        anchors{
            left: row.left
            leftMargin: row.width / 10
            top: row.top
            bottom: row.bottom
        }    
        text: "x" + (index + 1 - backend.curField.shipCount[3 - index])
        color: Material.foreground
        font.pointSize: row.height / 4
        verticalAlignment: Text.AlignVCenter
    }

    Item{
        id: imageHolder

        anchors{
            left: rowText.right;
            bottom :row.bottom;
            top: row.top;
            leftMargin: row.width / 10
        }
        width : (4 - index) * Math.min(height, (row.width - rowText.width - rowText.anchors.leftMargin - anchors.leftMargin) / 4)

        Image {
            id: image
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            source: "qrc:///data/ship" + Number(4 - index).toString() + ".png"
        }
    }

    Behavior on opacity{
        ColorAnimation {
            duration: 200
        }
    }
}
