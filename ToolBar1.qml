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
                //color: "#6A5ACD" //slate blue
                //inner area
                gradient: Gradient {
                  GradientStop {
                    position: 0.0
                    color: "#6A5ACD" //slate blue
                  }
                  GradientStop {
                    position: 1.0
                    color: "#FF000000" //black"#FFFFFF" //white //
                  }
                }
                Rectangle {
                                anchors { top: parent.top; left: parent.left }
                                anchors.margins: { left: 4; top: 4 }
                                width: parent.width-8
                                height: parent.height-9
                                color: "#483D8B" //dark slate blue
                                border.color: "#FFFFFF" //white
                                border.width: 1

                }
                /*//right gradient
                Rectangle {
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: parent.width - 4
                                width: 4
                                height: parent.height
                                //border.color: "#999"
                                //color: "#6495ED" //corn flower blue
                                gradient: Gradient {
                                        GradientStop { position: 0.0; color: "#6A5ACD" }
                                        GradientStop { position: 1.0; color: "#6495ED"  }
                                    }
                }
                //bottom gradient
                Rectangle {
                                anchors { bottom: parent.bottom; left: parent.left }
                                anchors.topMargin: -4
                                width: parent.width
                                height: 4
                                //border.color: "#999"
                                //color: "#6495ED" //corn flower blue
                                gradient: Gradient {
                                        GradientStop { position: 0.0; color: "#6495ED" }
                                        GradientStop { position: 0.1; color: "#6A5ACD" }
                                    }
                }*/

            }
        }
    }
}
