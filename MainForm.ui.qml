import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

Item {
    property var spaces_main : 3;
    property var toolBarMinHeight : 43;

    property var treeMinWidth : 193;
    property var treeMinHeight : 300;


    property var viewMinWidth : 428;
    property var viewMinHeight : 300;

    property var gBoxFor1 : 40 ;

    width:  640 //ApplicationWindow.__width
    height: 450 //ApplicationWindow.__height

    Rectangle {
        width: parent.width
        height: parent.height

        color: "black"

        anchors.fill: parent

        Item {
            id: itemTree
            width: treeMinWidth
            //height: parent.height
            height: (parent.height < treeMinHeight) ? treeMinHeight: parent.height
            anchors.left: parent.left


            ToolBar1 {
                id: toolBar1Tree
                width: parent.width
                height: toolBarMinHeight
                //anchors.fill: parent.width


                /* GroupBox {
                            id: gBoxToolsTree
                            anchors { bottom: parent.bottom; right: parent.right }
                            width: parent.width-4
                            height: parent.height-4
                            visible: true
                            x: -1
                            y: 0

                            ToolBut {
                                id: toolBut1
                                x: 94
                                y: 6
                            }

                            ToolBut {
                                id: toolBut2
                                x: 131
                                y: 6
                            }

                            ToolBut {
                                id: toolBut6
                                x: 49
                                y: 6
                            }
                        }*/
            }

            RectForParts {
                id: rectForPartsTree

                width: parent.width
                //implicitHeight: parent.height-toolBar1Tree.height-spaces_main

                anchors.top: toolBar1Tree.bottom
                anchors.topMargin: spaces_main
                anchors.bottom: parent.bottom
                anchors.left: parent.left

                TreeView {
                    id: treeView1
                    width: parent.width-6
                    height: parent.height-6
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

                        ToolBut {
                            id: toolBut3
                            x: 66
                            y: 0
                            width: 28
                            height: 27
                        }

                        ToolBut {
                            id: toolBut4
                            x: 100
                            y: 0
                            width: 29
                            height: 27
                        }

                        ToolBut {
                            id: toolBut5
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

        Item {
            id: itemView
            //width: viewMinWidth
            width: (parent.width-treeMinWidth-spaces_main < viewMinWidth) ? viewMinWidth: parent.width-treeMinWidth-spaces_main
            //height: parent.height
            height: (parent.height < viewMinHeight) ? viewMinHeight: parent.height
            anchors.left: itemTree.right
            anchors.leftMargin: spaces_main

            ToolBar1 {
                id:toolBar1View
                width: parent.width
                height: toolBarMinHeight
                //anchors.fill: parent.width

                GroupBox {
                    id: gBoxTViewUndoRedo
                    anchors { bottom: parent.bottom; top: parent.top; left: parent.left }
                    anchors.leftMargin: 4
                    anchors.topMargin: 4
                    anchors.bottomMargin: 4
                    width: gBoxFor1*2
                    visible: true

                    ToolBut {
                        id: toolButUndo
                        anchors { top: parent.top; left: parent.left }
                        anchors.leftMargin: 0
                        anchors.topMargin: 1
                        //anchors.bottomMargin: 4

                    }

                    ToolBut {
                        id: toolButRedo
                        anchors { top: parent.top; left: parent.left }
                        anchors.leftMargin: gBoxFor1
                        anchors.topMargin: 1

                    }

                }
                GroupBox {
                            id: gBoxTViewEdit
                            anchors { bottom: parent.bottom; top: parent.top; left: gBoxTViewUndoRedo.right }
                            anchors.leftMargin: -1
                            anchors.topMargin: 4
                            anchors.bottomMargin: 4
                            width: gBoxFor1*4
                            visible: true
                            ToolBut {
                                id: toolButCut
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: 0
                                anchors.topMargin: 1

                            }

                            ToolBut {
                                id: toolButRotate
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: gBoxFor1
                                anchors.topMargin: 1

                            }
                            ToolBut {
                                id: toolButBlackWhite
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: gBoxFor1*2
                                anchors.topMargin: 1

                            }

                            ToolBut {
                                id: toolButEnhance
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: gBoxFor1*3
                                anchors.topMargin: 1

                            }
                        }
                GroupBox {
                            id: gBoxTViewPrint
                            anchors { bottom: parent.bottom; top: parent.top; left: gBoxTViewEdit.right; right: toolBar1View.right }
                            anchors.leftMargin: -1
                            anchors.topMargin: 4
                            anchors.bottomMargin: 4
                            anchors.rightMargin: 4
                            visible: true
                            ToolBut {
                                id: toolButPrint
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: 0
                                anchors.topMargin: 1

                            }
                        }

            }

            RectForParts {
                id: rectForPartsView

                width: parent.width
                //implicitHeight: parent.height-toolBar1View.height-spaces_main

                anchors.top: toolBar1View.bottom
                anchors.topMargin: spaces_main
                anchors.bottom: parent.bottom
                anchors.right: parent.right

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
