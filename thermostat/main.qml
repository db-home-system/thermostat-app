import QtQuick 2.4
import QtQuick.Window 2.2
import Thermostat 1.0
import Timeline 1.0

Window {
    visible: true
    width: 480
    height: 272
    title: qsTr("Hello World")

    Item {
        anchors.fill: parent
        Image{
            source: "images/logo.png"
            anchors.top: parent.top
            width: 80; height: 80
            fillMode: Image.PreserveAspectFit
        }

        Timeline {
            anchors.centerIn: parent
            width: 270
            height: 270
        }

        Text {
            id: temperature
            anchors.centerIn: parent
            text : "30ªC"
        }
    }

}
