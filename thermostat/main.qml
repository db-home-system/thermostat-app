import QtQuick 2.4
import QtQuick.Window 2.2
import ThermostatApp 1.0

Window {
    visible: true
    width: 480
    height: 272
    title: qsTr("DB Home System")

    Item {
        id: manager
        property string temperature: Manager.temperature
    }

    Item {
        anchors.fill: parent
        Image{
            source: "images/logo.png"
            anchors.top: parent.top
            width: 80; height: 80
            fillMode: Image.PreserveAspectFit
        }

        Timeline {
            id: timeline
            anchors.centerIn: parent
            width: 270
            height: 270

        }

        Text {
            anchors.centerIn: parent
            font.pointSize: 24
            text : "<b>"+ manager.temperature+"</b>"
        }
    }

}
