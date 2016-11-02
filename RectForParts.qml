import QtQuick 2.0

Item {
    Rectangle{
        width: 193
        height: 450-43-3
        color: "#6A5ACD" //slate blue
        x: 0
        y: 0
        Rectangle{
            width: parent.width-3
            height: parent.height-3
            color: "#6495ED" //corn flower blue
            x: 0
            y: 0
            anchors { bottom: parent.bottom; right: parent.right }

        }
    }

}
