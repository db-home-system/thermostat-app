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
        property string intTemperature: Manager.intTemperature
        property double extTemperature: Manager.extTemperature
        property string thermostatStatus: Manager.thermostatStatus
        property string time: Manager.time
        property string tempo: Manager.tempo
        property string icon: Manager.icon
    }

    Item {
        id: item1
        anchors.fill: parent
        Image{
            source: "images/logo.png"
            anchors.top: parent.top
            width: 80; height: 80
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: weatherIcon
            source: "images/icons/" + manager.icon + ".svg"
            width: 50; height:50
            fillMode: Image.PreserveAspectFit
        }

        Timeline {
            id: timeline
            anchors.centerIn: parent
            width: 270
            height: 270
        }

        Text {
            width: 220
            height: 34
            anchors.centerIn: parent
            font.pointSize: 24
            anchors.verticalCenterOffset: -32
            anchors.horizontalCenterOffset: 0
            text : "<b>" + manager.intTemperature + "</b>"
        }

        Text {
            anchors.centerIn: parent
            font.pointSize: 22
            text : manager.extTemperature
        }

        Text {
            id: thermostat
            anchors.centerIn: parent
            font.pointSize: 17
            anchors.verticalCenterOffset: 32
            anchors.horizontalCenterOffset: 0
            text: manager.thermostatStatus
        }

        Text {
            id: clock
            anchors.centerIn: parent
            font.pointSize: 17
            text: manager.time
            anchors.verticalCenterOffset: 32
            anchors.horizontalCenterOffset: 0
            color: 'grey'
            opacity: 0.7
        }

        Text {
            id: random
            width: 100
            font.pointSize: 9
            text: "Temperature: " + manager.tempo + "°"
            fontSizeMode: Text.Fit
            anchors.verticalCenterOffset: 25
            anchors.horizontalCenterOffset: -190
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }

}

