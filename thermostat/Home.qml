import QtQuick 2.0
import ThermostatApp 1.0


Item {
    id: root

    signal showSettings()

    QtObject {
        id: manager
        property int intTemperature: Manager.intTemperature
        property int extTemperature: Manager.extTemperature
        property string thermostatStatus: Manager.thermostatStatus
        property string displayClock: Manager.displayClock
        property var    weatherData: Manager.weatherData
    }

    /*  Clock and time Info block */
    Item {
        id: clock
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 91
        anchors.leftMargin: 1
        width: 85
        height: 89
        Rectangle {
            id: clockElemnt
            anchors.fill: clock
            radius: 4
            border.color: "black"
            border.width: 1
            Text {
                id: dayName
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 16
                text: "Lunedi, 30"
                color: "#364922"
            }
            Text {
                id: dateStr
                anchors.top: dayName.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 16
                text: "Luglio"
                color: "#364922"
            }
            Text {
                id: clockStr
                anchors.top: dateStr.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 16
                text: manager.displayClock
                color: "#364922"
            }
            Text {
                id: yearStr
                anchors.top: clockStr.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: 16
                text: "2018"
                color: "#364922"
            }
        }
    }

    /* Mode block and commands */
    Item {
        id: mode
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 182
        anchors.leftMargin: 1
        width: 85
        height: 89
        Rectangle {
            id: modeElemnt
            anchors.fill: parent
            radius: 4
            border.color: "black"
            border.width: 1
            Text {
                anchors.centerIn: parent
                font.pointSize: 17
                text: "Mode"
                color: "#364922"
            }
        }
    }



    /*  TimeLine block */
    Item {
        id: timeLine
        anchors.centerIn: parent
        width: 305
        height: 270
        Rectangle {
            id:timeLineBox
            anchors.fill: parent
            anchors.centerIn: timeLine
            radius: 4
            border.color: "black"
            border.width: 1
            Timeline {
                id: timelineElement
                anchors.centerIn: timeLineBox
                width: 270
                height: 270
            }

            Text {
                anchors.centerIn: parent
                font.pointSize: 24
                anchors.verticalCenterOffset: -32
                text : {
                    var i = "-"
                    if (manager.intTemperature !== -273000) {
                        var n = manager.intTemperature / 1000;
                        i = String(n.toFixed(1)) + "°"
                        i = i.replace(".", ",")
                    }
                    return "<b>" + i + "</b>"
                }
            }
            Text {
                anchors.centerIn: parent
                font.pointSize: 22
                text : {
                    var ext = "-"
                    if (manager.extTemperature !== -273000) {
                        var n = manager.extTemperature / 1000;
                        ext = String(n.toFixed(1)) + "°"
                        ext = ext.replace(".", ",")
                    }
                    return ext;
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: root.showSettings()
                }
            }

            Text {
                id: thermostat
                anchors.centerIn: parent
                font.pointSize: 17
                anchors.verticalCenterOffset: 64
                anchors.horizontalCenterOffset: 0
                text: manager.thermostatStatus
            }
        }
    }

    Item {
        id: weatherInfo
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 1
        anchors.rightMargin: 1
        width: 85
        Column {
            spacing: 1
            Repeater {
                model: manager.weatherData
                Rectangle {
                    id: weatherbox
                    width: 85
                    height: 89
                    radius: 4
                    border.color: "black"
                    border.width: 1
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
                                min = String(m.toFixed(1)) + "°"
                                min = min.replace(".", ",")
                            }
                            var max = "-"
                            if (modelData.temp_max !== -273000) {
                                var n = modelData.temp_max / 1000;
                                max = String(n.toFixed(1)) + "°"
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
