import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
    visible: true
    width: 480
    height: 272
    title: qsTr("DB Home System")

    Item {
        anchors.fill: parent

        StackView {
            id: stack
            anchors.fill: parent
            initialItem: Home {
                id: homePage
                onShowSettings: stack.push(myComponents)
            }
        }

        Image{
            source: "images/logo.png"
            anchors.top: parent.top
            width: 80; height: 80
            fillMode: Image.PreserveAspectFit
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    stack.clear()
                    stack.push(homePage)
                }
            }
        }

    }

    Component {
        id: myComponents

        Settings {
            onGoBack: stack.pop()
        }
    }

}

