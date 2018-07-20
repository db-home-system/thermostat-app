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
        property string tempo: Manager.tempo
        property string icon: Manager.icon
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

    Image {
        id: weatherIcon
        x: 412
        y: 8
        source: "images/icons/" + manager.icon + ".svg"
        width: 50; height:50
        fillMode: Image.PreserveAspectFit
    }

    Text {
        id: weatherInfo
        width: 100
        color: "#8d8d8d"
        text: manager.tempo + "°"
        font.pointSize: 9
        fontSizeMode: Text.Fit
        anchors.verticalCenterOffset: -63
        anchors.horizontalCenterOffset: 197
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
