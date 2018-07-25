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
                Rectangle {
                    width: 85
                    height: 90
                    color: "#cef2ff"
                    radius: 3

                    Text {
                        id: label
                        anchors.top: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pointSize: 10
                        text: "<b>" + modelData.desc + "</b>"
                    }
                    Image {
                        id: iconText
                        anchors.top: label.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: modelData.icon
                        width: 60; height:60
                        fillMode: Image.PreserveAspectFit
                    }

                    Text {
                        id: tempMinMaxText
                        anchors.top: iconText.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pointSize: 9.5
                        fontSizeMode: Text.Fit
                        text: {
                            var min = "-"
                            if (modelData.temp_min !== -273000) {
                                var m = modelData.temp_min / 1000;
                                min = m.toFixed(1) + "°C"
                                min = min.replace(".", ",")
                            }
                            var max = "-"
                            if (modelData.temp_max !== -273000) {
                                var n = modelData.temp_max / 1000;
                                max = n.toFixed(1) + "°C"
                                max = max.replace(".", ",")
                            }

                            return min + " / " + max
                        }
                    }
                }
            }
        }
    }
}
