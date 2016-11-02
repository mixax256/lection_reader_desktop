import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    ToolButton {
        style: ButtonStyle {
               background: Rectangle {
                    implicitWidth: 20
                    implicitHeight: 20
                    color: "#000088"
                    radius: 1.0

                    Rectangle{
                        implicitWidth: parent.width - 4
                        implicitHeight: parent.height - 4
                        anchors { bottom: parent.bottom; left: parent.left }
                        color: "#0000cc"
                        radius: 3.0
                    }
                    Rectangle{
                        implicitWidth: parent.width -4
                        implicitHeight: parent.height - 4
                        anchors { top: parent.top; right: parent.right }
                        color: "#0000a0"
                        radius: 3.0
                    }

                 }

            }
       }
}
