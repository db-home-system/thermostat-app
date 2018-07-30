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

        Item {
            id: mainLogo
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 1
            anchors.leftMargin: 1
            width: 85
            height: 89
            Rectangle {
                id: mainLogoElement
                anchors.fill: mainLogo
                radius: 4
                border.color: "black"
                border.width: 1
                Image{
                    source: "images/logo.png"
                    fillMode: Image.PreserveAspectFit
                    anchors.fill: parent
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            stack.clear()
                            stack.push(homePage)
                        }
                    }
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

