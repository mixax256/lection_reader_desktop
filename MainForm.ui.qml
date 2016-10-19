import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Item {
    width: 640
    height: 450
    Rectangle {
        width: parent.width
        height: parent.height
        color: "black"

        RowLayout {
            id: rowLayout1
            x: 3
            y: 3
            width: parent.width-3
            height: parent.height-3
            spacing: 3


            Item {
                id: itemTree
                width: 193
                height: parent.height


                ColumnLayout {
                    id: columnLayout1
                    x: 0
                    y: 0
                    width: parent.width
                    height:parent.height
                    spacing: 0

                    Rectangle {
                        x: 0
                        y: 0
                        width: parent.width
                        height: 43
                        color: "grey"

                        GroupBox {
                            id: gBoxToolsTree
                            width: parent.width
                            height: parent.height
                            visible: true
                            title: qsTr("Group Box")
                            x: 0
                            y: 0

                            ToolButton {
                                id: toolButton1
                                x: 94
                                y: -6
                            }

                            ToolButton {
                                id: toolButton2
                                x: 131
                                y: -6
                            }

                            ToolButton {
                                id: toolButton6
                                x: 49
                                y: -6
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: parent.height-43-3
                        color: "brown"
                        x: 0
                        y: 0

                        TreeView {
                            id: treeView1
                            width: parent.width-3
                            height: parent.height-3
                            x: 3
                            y: 3

                            Slider {
                                id: sliderVertical2
                                x: parent.width-18
                                y: 5
                                width: 18
                                height: parent.height-5-22
                                orientation: Qt.Vertical
                            }

                            GroupBox {

                                id: gBoxAppearOnTree
                                x: 0
                                y: (parent.height-sliderHorizontal3.height-47)
                                width: parent.width
                                height: 47
                                title: qsTr("Group Box")

                                Button {
                                    id: butCancelOnTree
                                    x: 0
                                    y: 0
                                    width: 60
                                    height: 27
                                    text: qsTr("Button")
                                }

                                ToolButton {
                                    id: toolButton3
                                    x: 66
                                    y: 0
                                    width: 28
                                    height: 27
                                }

                                ToolButton {
                                    id: toolButton4
                                    x: 100
                                    y: 0
                                    width: 29
                                    height: 27
                                }

                                ToolButton {
                                    id: toolButton5
                                    x: 135
                                    y: 0
                                    width: 31
                                    height: 27
                                }
                            }
                            Slider {
                                id: sliderHorizontal3
                                x: 5
                                y: parent.height-18
                                width: parent.width-22-5
                                height: 18
                            }


                            Rectangle {
                                id: rectangle3
                                x: parent.width-22
                                y: parent.height-22
                                width: 22
                                height: 22
                                color: "#ffffff"
                            }
                        }




                    }
                }
            }

            Item {
                id: itemView
                width: 428
                height: parent.height

                ColumnLayout {
                    id: columnLayout2
                    x: 0
                    y: 0
                    width: parent.width
                    height:parent.height
                    spacing: 0

                    Rectangle {
                        x: 0
                        y: 0
                        width: parent.width
                        height: 43
                        color: "grey"

                        GroupBox {
                            id: gBoxToolsView
                            x: 3
                            y: 3
                            width: parent.width-3
                            height: parent.height-3
                            title: qsTr("Group Box")

                            ToolButton {
                                id: toolButton7
                                x: 0
                                y: 0
                            }

                            ToolButton {
                                id: toolButton8
                                x: 37
                                y: 0
                            }

                            ToolButton {
                                id: toolButton9
                                x: 87
                                y: 0
                            }

                            ToolButton {
                                id: toolButton10
                                x: 124
                                y: 0
                            }

                            ToolButton {
                                id: toolButton11
                                x: 167
                                y: 0
                            }

                            ToolButton {
                                id: toolButton12
                                x: 212
                                y: 0
                            }

                            ToolButton {
                                id: toolButton13
                                x: 256
                                y: 0
                            }
                        }

                    }

                    Rectangle {
                        width: parent.width
                        height: parent.height-43-3
                        color: "brown"

                        Slider {
                            id: sliderVertical1
                            x: parent.width-18
                            y: 5
                            width: rectangle1.width
                            height: parent.height-5-rectangle1.height
                            orientation: Qt.Vertical
                        }

                        GroupBox {
                            id: gBoxAppearOnView
                            x: 0
                            y: (parent.height-rectangle1.height-47)
                            width: parent.width
                            height: 47
                            visible: true
                            title: qsTr("Group Box")

                            Button {
                                id: buttonOkOnView
                                x: 336
                                y: 0
                                text: qsTr("Button")
                            }

                            Button {
                                id: butCancelOnView
                                x: 255
                                y: 0
                                text: qsTr("Button")
                            }
                        }


                        RowLayout {
                            id: rowLayout2
                            width: parent.width
                            height: 18
                            x: 0
                            y: parent.height-18
                            spacing: 0

                            Slider {
                                id: sliderHorizontal1
                                width: 100
                                height: rectangle1.height

                            }
                            Rectangle {
                                id: rectangle1
                                x: 100
                                width: 18
                                height: 18
                                color: "#ffffff"
                            }
                            Image {
                                id: image1
                                x: 118
                                width: rectangle1.width
                                height: rectangle1.height
                                //source: "qrc:/qtquickplugin/images/template_image.png"
                            }

                            Slider {
                                id: sliderHorizontal2
                                x: 136
                                width:100
                                height: rectangle1.height
                            }
                            Rectangle {
                                id: rectangle2
                                width: rectangle1.width
                                height: rectangle1.height
                                color: "#ffffff"
                            }

                        }
                    }


                }
            }
        }
    }





}
