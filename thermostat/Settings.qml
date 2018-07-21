import QtQuick 2.0

Rectangle {
    id: root

    signal goBack()
    color: 'red'
    Text {
        text: "EXT TEMPERATURE"
        anchors.centerIn: parent
        MouseArea {
            anchors.fill: parent
            onClicked: root.goBack()
        }
    }
}
