import QtQuick 2.4
import QtQuick.Window 2.2
import com.thermostat 1.0

Window {
    visible: true
    width: 480
    height: 272
    title: qsTr("Hello World")

    Thermostat {
        id: temperatura;
    }

    Item {
        Text {
            id: txt
            text: temperatura.temperature
            anchors.bottom: parent.bottom
        }

        anchors.fill: parent
        Image{
            source: "images/logo.png"
            anchors.top: parent.top
            width: 80; height: 80
            fillMode: Image.PreserveAspectFit
        }

        Rectangle{
            color: "red"
            width: parent.width * 0.5
            height: parent.height * 0.5
            anchors.centerIn: parent
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if (Qt.colorEqual(parent.color, "red")) {
                        parent.color = "blue"
                        temperatura.temperature = "18°C"
                    }
                    else {
                        parent.color = "red"
                        temperatura.temperature = "30°C"
                    }
                }
            }
        }
    }

}
