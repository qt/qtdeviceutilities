import QtQuick 2.0
import QtAndroidOmx 1.0

Rectangle {
    id: root
    color: "white"

    Column {
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter

        OmxItem {
            id: omx

            width: root.width * 0.5
            height: width / aspect

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 40
            source: "NativeMedia.ts"

            property real aspect: sourceWidth / (sourceHeight > 0 ? sourceHeight : 1)

            MouseArea {
                anchors.fill: parent
                onClicked: parent.paused = !parent.paused
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Video size: " + omx.sourceWidth + "x" + omx.sourceHeight
        }
    }
}
