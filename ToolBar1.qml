import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    ToolBar {
        x: 0
        y: 0
        implicitWidth: 193
        implicitHeight: 43
        style: ToolBarStyle{
            /*padding {
                left: 8
                right: 8
                top: 13
                bottom: 3
            }*/
            background: Rectangle {
                width: parent.parent.width
                height: parent.parent.height
                //border.color: "#999"
                color: "#6A5ACD" //slate blue
                Rectangle {
                                anchors { bottom: parent.bottom; right: parent.right }
                                width: parent.width-4
                                height: parent.height-4
                                //border.color: "#999"
                                color: "#6495ED" //corn flower blue
                }

            }
        }
    }
}
