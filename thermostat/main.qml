import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    visible: true
    width: 480
    height: 272
    title: qsTr("Hello World")
    Rectangle{
        color: "red"
        width: parent.width * 0.5
        height: parent.height * 0.5
        anchors.centerIn: parent
        MouseArea{
            anchors.fill: parent
            onClicked: {
                if (Qt.colorEqual(parent.color, "red"))
                    parent.color = "blue"
                else
                    parent.color = "red"
            }
        }
    }

}
