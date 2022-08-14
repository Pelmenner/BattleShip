import QtQuick 2.15
import QtQuick.Controls.Material 2.0

Item {
    id: row

    required property int length
    required property int count

    opacity: count === 5 - length? 0.5 : 1

    Text{
        id: rowText

        anchors{
            left: row.left
            leftMargin: row.width / 10
            top: row.top
            bottom: row.bottom
        }
        text: "x" + (5 - length - count)
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
        width : length * Math.min(height, (row.width - rowText.width - rowText.anchors.leftMargin - anchors.leftMargin) / 4)

        Image {
            id: image
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignVCenter
            source: "qrc:///res/ship" + length + ".png"

        }
    }

    Behavior on opacity{
        OpacityAnimator {
            duration: 200
        }
    }
}
