import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    Rectangle{
        implicitWidth: parent.parent.width //16
        implicitHeight: (parent.parent.height-toolBarMinHeight-spaces_main)//450-43-3
        //color: "#6A5ACD" //slate blue

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
        Rectangle{
            //color: "#6495ED" //corn flower blue
            x: 0
            y: 0
            anchors { top: parent.top; left: parent.left }
            anchors.margins: { left: 4; top: 4 }
            width: parent.width-8
            height: parent.height-8
            color: "#483D8B" //dark slate blue
            border.color: "#FFFFFF" //white
            border.width: 1

        }
    }

}
