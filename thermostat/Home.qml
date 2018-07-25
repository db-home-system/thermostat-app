import QtQuick 2.0
import ThermostatApp 1.0


Item {
    id: root

    property color coloreDiSfondo

    signal showSettings()

    QtObject {
        id: manager
        property string intTemperature: Manager.intTemperature
        property string extTemperature: Manager.extTemperature
        property string thermostatStatus: Manager.thermostatStatus
        property string displayClock: Manager.displayClock
        property var    weatherData: Manager.weatherData
    }

    //    Image{
    //        source: "file://Users/asterix/Desktop/shot.jpg"
    //        anchors.top: parent.Center
    //        width: 480; height: 272
    //        fillMode: Image.PreserveAspectFit
    //    }

    Timeline {
        id: timeline
        anchors.centerIn: parent
        width: 270
        height: 270
    }

    Text {
        anchors.centerIn: parent
        font.pointSize: 24
        anchors.verticalCenterOffset: -32
        text : "<b>" + manager.intTemperature + "</b>"
    }

    Text {
        anchors.centerIn: parent
        font.pointSize: 22
        text : manager.extTemperature
        MouseArea {
            anchors.fill: parent
            onClicked: root.showSettings()
        }
    }

    Text {
        id: clock
        anchors.centerIn: parent
        font.pointSize: 17
        text: manager.displayClock
        anchors.verticalCenterOffset: 32
        color: "#364922"
    }

    Text {
        id: thermostat
        anchors.centerIn: parent
        font.pointSize: 17
        anchors.verticalCenterOffset: 64
        anchors.horizontalCenterOffset: 0
        text: manager.thermostatStatus
    }

    Item {
        id: weatherInfo
        anchors.verticalCenterOffset: - (parent.height / 2)
        anchors.horizontalCenterOffset: (parent.width / 2) - 85
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        Column {
            spacing: 2
            Repeater {
                model: manager.weatherData
                onModelChanged: console.log(model)
                Rectangle {
                    width: 85
                    height: 75
                    Text {
                        anchors.top: parent.top
                        font.pointSize: 10
                        text: modelData.temp_min + "°C / " + modelData.temp_max + "°C"
                    }
                    Image {
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: modelData.icon
                        width: 60; height:60
                        fillMode: Image.PreserveAspectFit
                    }
                }
            }
        }
    }
}
