import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Item {
    width: 640
    height: 480

    GroupBox {
        id: groupBox1
        x: 12
        y: 408
        width: 174
        height: 48
        title: qsTr("Group Box")

        ToolButton {
            id: toolButton1
            x: 74
            y: -6
        }

        ToolButton {
            id: toolButton2
            x: 124
            y: -6
        }
    }

    TreeView {
        id: treeView1
        x: 12
        y: 47
        width: 174
        height: 355

        Slider {
            id: sliderVertical2
            x: 157
            y: 0
            width: 18
            height: 353
            orientation: Qt.Vertical
        }
    }

    GroupBox {
        id: groupBox2
        x: 14
        y: 350
        width: 172
        height: 43
        title: qsTr("Group Box")

        Button {
            id: button1
            x: 0
            y: 0
            text: qsTr("Button")
        }

        Button {
            id: button2
            x: 81
            y: -3
            text: qsTr("Button")
        }
    }

    Item {
        id: item1
        x: 192
        y: 47
        width: 432
        height: 409

        Slider {
            id: sliderHorizontal1
            x: 264
            y: 387
            width: 135
            height: 22
        }

        Slider {
            id: sliderHorizontal2
            x: 0
            y: 387
            width: 258
            height: 22
        }

        Slider {
            id: sliderVertical1
            x: 402
            y: 0
            width: 22
            height: 386
            orientation: Qt.Vertical
        }
    }

}
