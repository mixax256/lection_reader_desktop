import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2

Item {
    property var spaces_main : 3;
    property var toolBarMinHeight : 50;

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


                 GroupBox {
                            id: gBoxToolsTree
                            anchors { bottom: parent.bottom; top: parent.top; left: parent.left; right: parent.right }
                            anchors.leftMargin: 4
                            anchors.topMargin: 4
                            anchors.bottomMargin: 4
                            anchors.rightMargin: 4
                            //width: gBoxFor1*2
                            visible: true


                            ToolButton {
                                id: toolButCheck
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: 0
                                anchors.topMargin: 0
                                width: 30
                                height: 30


                                Image {
                                    source: "buttons/check1.svg"
                                    antialiasing: true
                                    anchors.fill: parent
                                    z:2
                                }

                                Image {
                                    source: "buttons/фон.svg"
                                    anchors.fill: parent
                                    z: 1
                                }
                            }
                            ToolButton {
                                id: toolButAdd
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: gBoxFor1-5
                                anchors.topMargin: 0
                                width: 30
                                height: 30
                                onClicked: {
                                    edtInput.text = "";
                                    switch(modelTree.getType(selectionModel.currentIndex)) {
                                        case 0: txtDlgAdd.text = "Введите название предмета:";
                                                dlgAdd.open();
                                                break;
                                        case 1: txtDlgAdd.text = "Введите название лекции:";
                                                dlgAdd.open();
                                                break;
                                        case 2: fileDialog.open();
                                                break;
                                        default: break;
                                    }
                                }

                                Image {
                                    source: "buttons/add1.svg"
                                    antialiasing: true
                                    anchors.fill: parent
                                    z:2
                                }

                                Image {
                                    source: "buttons/фон.svg"
                                    anchors.fill: parent
                                    z: 1
                                }

                            }
                            ToolButton {
                                id: toolButDelete
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: gBoxFor1*2-10
                                anchors.topMargin: 0
                                width: 30
                                height: 30
                                onClicked: {
                                    modelTree.deleteItem(selectionModel.currentIndex.row, selectionModel.currentIndex);
                                    treeView1.__currentRow = selectionModel.__currentRow - 1;
                                    if (modelTree.data(selectionModel.currentIndex, 1)) {
                                        lection_image.source = modelTree.data(selectionModel.currentIndex, 1);
                                    }
                                    else {
                                        lection_image.source = "";
                                    }
                                }

                                Image {
                                    source: "buttons/delete1.svg"
                                    antialiasing: true
                                    anchors.fill: parent
                                    z:2
                                }

                                Image {
                                    source: "buttons/фон.svg"
                                    anchors.fill: parent
                                    z: 1
                                }

                            }

                        }
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
                    model: modelTree
                    selection: ItemSelectionModel{
                        id: selectionModel
                        model: treeView1.model
                    }
                    x: 3
                    y: 3
                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        onClicked: {
                            var index_item = parent.indexAt(mouse.x, mouse.y);
                            if (index_item.valid) {
                                parent.isExpanded(index_item) ? parent.collapse(index_item) : parent.expand(index_item);
                                if (parent.model.data(index_item, 1)) {
                                    lection_image.source = parent.model.data(index_item, 1);
                                }
                                else {
                                    lection_image.source = "";
                                }
                            }
                            parent.selection.setCurrentIndex(index_item, ItemSelectionModel.ClearAndSelect);
                        }
                    }

                    TableViewColumn {
                        title: ""
                        role: "display"
                        width: 200
                    }

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

                    ToolButton {
                        id: toolButUndo
                        anchors { top: parent.top; left: parent.left }
                        anchors.leftMargin: 0
                        anchors.topMargin: 0
                        //anchors.bottomMargin: 4
                        //children[1].
                        width: 30
                        height: 30

                        Image {
                            source: "buttons/undo1.svg"
                            antialiasing: true
                            anchors.fill: parent
                            z:2
                        }

                        Image {
                            source: "buttons/фон.svg"
                            anchors.fill: parent
                            z: 1
                        }

                    }

                    ToolButton {
                        id: toolButRedo
                        anchors { top: parent.top; left: parent.left }
                        anchors.leftMargin: gBoxFor1-5
                        anchors.topMargin: 0
                        width: 30
                        height: 30

                        Image {
                            source: "buttons/redo1.svg"
                            antialiasing: true
                            anchors.fill: parent
                            z:2
                        }

                        Image {
                            source: "buttons/фон.svg"
                            anchors.fill: parent
                            z: 1
                        }

                    }

                }
                GroupBox {
                            id: gBoxTViewEdit
                            anchors { bottom: parent.bottom; top: parent.top; left: gBoxTViewUndoRedo.right }
                            anchors.leftMargin: -1
                            anchors.topMargin: 4
                            anchors.bottomMargin: 4
                            width: gBoxFor1*4-10
                            visible: true
                            ToolButton {
                                id: toolButCut
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: 0
                                anchors.topMargin: 0
                                width: 30
                                height: 30
                                onClicked: {
                                    lection_image.source = modelTree.cutImage(lection_image.source);
                                }

                                Image {
                                    source: "buttons/cut.svg"
                                    antialiasing: true
                                    anchors.fill: parent
                                    z:2
                                }

                                Image {
                                    source: "buttons/фон.svg"
                                    anchors.fill: parent
                                    z: 1
                                }

                            }

                            ToolBut {
                                id: toolButRotate
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: gBoxFor1-5
                                anchors.topMargin: 0
                                width: 30
                                height: 30
                                Image {
                                    source: "buttons/rotate1.svg"
                                    antialiasing: true
                                    anchors.fill: parent
                                    z:2
                                }

                                Image {
                                    source: "buttons/фон.svg"
                                    anchors.fill: parent
                                    z: 1
                                }

                            }
                            ToolButton {
                                id: toolButBlackWhite
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: gBoxFor1*2-10
                                anchors.topMargin: 0
                                width: 30
                                height: 30
                                Image {
                                    source: "buttons/black_white1.svg"
                                    antialiasing: true
                                    anchors.fill: parent
                                    z:2
                                }

                                Image {
                                    source: "buttons/фон.svg"
                                    anchors.fill: parent
                                    z: 1
                                }

                            }

                            ToolButton {
                                id: toolButEnhance
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: gBoxFor1*3-15
                                anchors.topMargin: 0
                                width: 30
                                height: 30
                                onClicked: {
                                    lection_image.source = modelTree.imageImprovment(lection_image.source);
                                }

                                Image {
                                    source: "buttons/enhance1.svg"
                                    antialiasing: true
                                    anchors.fill: parent
                                    z:2
                                }

                                Image {
                                    source: "buttons/фон.svg"
                                    anchors.fill: parent
                                    z: 1
                                }

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
                            ToolButton {
                                id: toolButPrint
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: 0
                                anchors.topMargin: 0
                                width: 30
                                height: 30
                                Image {
                                    source: "buttons/print1.svg"
                                    antialiasing: true
                                    anchors.fill: parent
                                    z:2
                                }
                                onClicked: {
                                   modelTree.print(lection_image.source);
                                }

                                Image {
                                    source: "buttons/фон.svg"
                                    anchors.fill: parent
                                    z: 1
                                }

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

                Image {
                    id: lection_image
                    anchors.fill:parent
                    anchors.topMargin: 10
                    anchors.bottomMargin: 50
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    scale: sliderHorizontal2.value
                    fillMode: Image.PreserveAspectFit  }

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

                    ToolButton {
                        id: buttonOkOnView
                        x: 130
                        y: 0
                        width: 130
                        height: 30

                        Image {
                            source: "buttons/ok.png"
                            anchors.fill: parent
                        }
                    }

                    ToolButton {
                        id: butCancelOnView
                        x: 270
                        y: 0
                        width: 130
                        height: 30

                        Image {
                            source: "buttons/cancel.png"
                            anchors.fill: parent
                        }
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
    Dialog {
        id: dlgAdd
        visible: false
        title: "Добавление"

        standardButtons: StandardButton.Ok | StandardButton.Cancel
        Column {
            anchors.fill: parent
            Text {
                id: txtDlgAdd
            }
            TextInput {
                id: edtInput
                width: parent.width
                focus: true
                activeFocusOnTab: true
            }
        }
        onVisibleChanged: {
            edtInput.forceActiveFocus();
        }

        onAccepted: {
            modelTree.addItem(edtInput.text, selectionModel.currentIndex);
        }
    }

    FileDialog {
        id: fileDialog
        title: "Выберите изображения лекций"
        folder: shortcuts.home
        visible: false
        nameFilters: [ "Изображения (*.jpg *.png *.bmp *gif)", "Все файлы (*)" ]
        selectedNameFilter: "Изображения (*.jpg *.png *.bmp *gif)"
        onAccepted: {
            modelTree.addItem(fileDialog.fileUrl, selectionModel.currentIndex);
        }
    }
}
