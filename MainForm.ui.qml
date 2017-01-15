import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
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
    property var rectX;
    property var rectY;
    property var rectWidth;
    property var rectHeight;
    property var lectImage;
    property var lastImage;
    property var hasChanges: false;

    width:  640
    height: 480
    Rectangle {
        width: parent.width
        height: parent.height

        color: "black"

        anchors.fill: parent
        focus: true
        Keys.onPressed: {
                if ( lection_image.status != Image.Null ) {
                    if (event.key == Qt.Key_Left || event.key == Qt.Key_Right) {
                        selectionModel.setCurrentIndex(modelTree.getImage(selectionModel.currentIndex, event.key),
                                                         ItemSelectionModel.ClearAndSelect);
                        treeView1.__currentRow = selectionModel.currentIndex.row;
                        lection_image.source = modelTree.data(selectionModel.currentIndex, 1);
                    }
                }
            }

        Item {
            id: itemTree
            width: treeMinWidth
            height: (parent.height < treeMinHeight) ? treeMinHeight: parent.height
            anchors.left: parent.left


            ToolBar1 {
                id: toolBar1Tree
                width: parent.width
                height: toolBarMinHeight

                 GroupBox {
                            id: gBoxToolsTree
                            anchors { bottom: parent.bottom; top: parent.top; left: parent.left; right: parent.right }
                            anchors.leftMargin: 4
                            anchors.topMargin: 4
                            anchors.bottomMargin: 4
                            anchors.rightMargin: 4
                            visible: true

                            ToolButton {
                                id: toolButAdd
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: 0
                                anchors.topMargin: 0
                                width: 30
                                height: 30
                                opacity: 0.9

                                MouseArea {
                                    hoverEnabled:true
                                    anchors.fill: parent
                                    onEntered: { parent.opacity = 1 }
                                    onExited: { parent.opacity = 0.9 }
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
                                anchors.leftMargin: gBoxFor1-5
                                anchors.topMargin: 0
                                width: 30
                                height: 30
                                opacity: 0.9

                                MouseArea {
                                    hoverEnabled:true
                                    anchors.fill: parent
                                    onEntered: { parent.opacity = 1 }
                                    onExited: { parent.opacity = 0.9 }
                                    onClicked: {
                                        var parent = selectionModel.currentIndex.parent;
                                        modelTree.deleteItem(selectionModel.currentIndex.row, selectionModel.currentIndex);

                                        if (selectionModel.currentIndex.row == -1) {
                                            selectionModel.setCurrentIndex(parent, ItemSelectionModel.ClearAndSelect);
                                        }
                                        if (modelTree.data(selectionModel.currentIndex,1)) {
                                            lection_image.source = modelTree.data(selectionModel.currentIndex,1);
                                        }
                                        else {
                                            lection_image.source = "";
                                        }
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
                width: itemTree.width
                anchors.top: toolBar1Tree.bottom
                anchors.topMargin: spaces_main
                anchors.bottom: itemTree.bottom
                anchors.left: itemTree.left

                TreeView {
                    id: treeView1
                    width: rectForPartsTree.width-6
                    height: rectForPartsTree.height-6
                    model: modelTree
                    selection: ItemSelectionModel{
                        id: selectionModel
                        model: treeView1.model
                    }
                    x: 3
                    y: 3
                    MouseArea {
                        id: mouseArea
                        width: parent.width
                        height: parent.height - 30
                        acceptedButtons: Qt.LeftButton
                        onClicked: {
                            var index_item = parent.indexAt(mouse.x, mouse.y);
                            if (index_item.valid) {
                                parent.isExpanded(index_item) ? parent.collapse(index_item) : parent.expand(index_item);
                                if (parent.model.data(index_item, 1)) {
                                    lection_image.source = parent.model.data(index_item, 1);
                                    sliderH_ScaleView.visible = true;
                                }
                                else {
                                    lection_image.source = "";
                                    buttonOkOnView.visible = false;
                                    butCancelOnView.visible = false;
                                    sliderH_ScaleView.visible = false;
                                }
                            }
                            lectImage = lection_image.source;
                            lastImage = lectImage;
                            hasChanges = false;
                            parent.selection.setCurrentIndex(index_item, ItemSelectionModel.ClearAndSelect);
                        }
                    }

                    TableViewColumn {
                        title: ""
                        role: "display"
                        resizable:true
                        width: 400/*treeView1.width-2*/

                    }

                }

            }
        }

        Item {
            id: itemView
            width: (parent.width-treeMinWidth-spaces_main < viewMinWidth) ? viewMinWidth: parent.width-treeMinWidth-spaces_main
            height: (parent.height < viewMinHeight) ? viewMinHeight: parent.height
            anchors.left: itemTree.right
            anchors.leftMargin: spaces_main

            ToolBar1 {
                id:toolBar1View
                width: itemView.width
                height: toolBarMinHeight

                GroupBox {
                            id: gBoxTViewEdit
                            anchors { bottom: parent.bottom; top: parent.top; left: parent.left }
                            anchors.leftMargin: 4
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
                                opacity: 0.9

                                MouseArea {
                                    hoverEnabled:true
                                    anchors.fill: parent
                                    onEntered: { parent.opacity = 1 }
                                    onExited: { parent.opacity = 0.9 }
                                    onClicked: {
                                        if ( lection_image.status != Image.Null ) {
                                            canvas.visible = false;
                                            lection_image.source = modelTree.cutImage(lastImage, rectX, rectY, rectWidth, rectHeight, lection_image.paintedWidth, lection_image.paintedHeight);
                                            lastImage = lection_image.source;
                                            hasChanges = true;
                                            buttonOkOnView.visible = true;
                                            butCancelOnView.visible = true;
                                        }
                                    }
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

                            ToolButton {
                                id: toolButRotate
                                anchors { top: parent.top; left: parent.left }
                                anchors.leftMargin: gBoxFor1-5
                                anchors.topMargin: 0
                                width: 30
                                height: 30
                                opacity: 0.9

                                MouseArea {
                                    hoverEnabled:true
                                    anchors.fill: parent
                                    onEntered: { parent.opacity = 1 }
                                    onExited: { parent.opacity = 0.9 }
                                    onClicked: {
                                        lastImage = lection_image.source;
                                        if ( lection_image.status != Image.Null ) {
                                            if (sliderRotation.visible == false)
                                                sliderRotation.visible = true
                                            else
                                                sliderRotation.visible = false
                                        }

                                        if (sliderRotation.value != 0) {
                                            hasChanges = true;
                                            buttonOkOnView.visible = true;
                                            butCancelOnView.visible = true;
                                        }
                                   }
                                }

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
                                opacity: 0.9
                                MouseArea {
                                    hoverEnabled:true
                                    anchors.fill: parent
                                    onEntered: { parent.opacity = 1 }
                                    onExited: { parent.opacity = 0.9 }
                                    onClicked: {
                                        if ( lection_image.status != Image.Null ) {
                                            lection_image.source=modelTree.toBlack(lection_image.source);
                                            lastImage = lection_image.source;
                                            hasChanges = true;
                                            buttonOkOnView.visible = true;
                                            butCancelOnView.visible = true;
                                        }
                                    }
                                }

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
                                opacity: 0.9
                                MouseArea {
                                    hoverEnabled:true
                                    anchors.fill: parent
                                    onEntered: { parent.opacity = 1 }
                                    onExited: { parent.opacity = 0.9 }
                                    onClicked: {
                                        if ( lection_image.status != Image.Null ) {
                                            lection_image.source = modelTree.imageImprovment(lection_image.source);
                                            lastImage = lection_image.source;
                                            hasChanges = true;
                                            buttonOkOnView.visible = true;
                                            butCancelOnView.visible = true;
                                        }
                                    }
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
                                opacity: 0.9

                                MouseArea {
                                    hoverEnabled:true
                                    anchors.fill: parent
                                    onEntered: { parent.opacity = 1 }
                                    onExited: { parent.opacity = 0.9 }
                                    onClicked: {
                                        if (selectionModel.currentIndex.valid) {
                                            modelTree.print(selectionModel.currentIndex);
                                        }
                                    }
                                }

                                Image {
                                    source: "buttons/print1.svg"
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
                id: rectForPartsView

                width: itemView.width
                anchors.top: toolBar1View.bottom
                anchors.topMargin: spaces_main
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                
                Slider {
                    id: sliderVertical1
                    x: parent.width-18
                    y: 5
                    width: rectImage.width
                    height: parent.height-5-rectImage.height
                    orientation: Qt.Vertical
                }
                
                Rectangle{
                    id: rectImage
                    anchors.top: rectForPartsView.top
                    anchors.topMargin: 5

                    anchors.right: rectForPartsView.right
                    anchors.rightMargin: 5
                    anchors.left: rectForPartsView.left
                    anchors.leftMargin: 5

                    height: rectForPartsView.height-rectBottomArea.height-10
                    color:"#483D8B" //dark slate blue

                    ScrollView{
                        id: scrollView
                        anchors.centerIn: parent
                        anchors.fill:parent

                       Image {
                            id: lection_image
                            scale: sliderH_ScaleView.value
                            fillMode: Image.PreserveAspectFit
                            transformOrigin: Item.Center
                            cache: false
                            rotation: sliderRotation.value * 360

                            Canvas {
                                id: canvas
                                anchors.fill: parent
                                onPaint: {
                                    var ctx = getContext("2d");
                                    ctx.lineWidth = 2 / sliderH_ScaleView.value;
                                    ctx.clearRect(0,0,canvas.width,canvas.height);
                                    ctx.beginPath();
                                    ctx.rect(rectX, rectY, rectWidth, rectHeight);
                                    ctx.stroke();
                                }
                            }

                            MouseArea {
                                 id: imageMouseArea
                                 width: lection_image.width
                                 height: lection_image.height
                                 onPositionChanged: {
                                     rectWidth = mouse.x - rectX;
                                     rectHeight = mouse.y - rectY;
                                     canvas.requestPaint();
                                 }

                                 onPressed: {
                                     rectX = mouse.x;
                                     rectY = mouse.y;
                                     rectWidth = 0;
                                     rectHeight = 0;
                                     canvas.visible = true;
                                     canvas.requestPaint();
                                 }

                                 onReleased: {
                                     rectX = (rectWidth < 0) ? rectX + rectWidth : rectX;
                                     rectY = (rectHeight < 0) ? rectY + rectHeight : rectY;
                                     rectWidth = (rectWidth < 0) ? -rectWidth : rectWidth;
                                     rectHeight = (rectHeight < 0) ? -rectHeight : rectHeight;
                                 }

                            }
                        }

                    }
                }

                Rectangle{
                    id: rectBottomArea
                    anchors.left:rectForPartsView.left
                    anchors.right:rectForPartsView.right
                    anchors.bottom: rectForPartsView.bottom
                    anchors.rightMargin: 5
                    anchors.leftMargin: 5
                    anchors.bottomMargin: 5

                    height: gBoxAppearOnView.height+bottomView.height
                    color:"#483D8B" //dark slate blue

                    GroupBox {
                        id: gBoxAppearOnView
                        x:-1
                        y:1

                        width: rectBottomArea.width+2
                        height: toolBarMinHeight-8
                        visible: true

                        ToolButton {
                            id: buttonOkOnView
                            anchors.right:butCancelOnView.left
                            anchors.rightMargin: 7
                            y: -1
                            width: 120
                            height: 30
                            visible: false
                            opacity: 0.9
                            MouseArea {
                                hoverEnabled:true
                                anchors.fill: parent
                                onEntered: { parent.opacity = 1 }
                                onExited: { parent.opacity = 0.9 }
                                onClicked: {
                                    modelTree.saveChanges(lastImage, sliderRotation.value * 360, lectImage);
                                    lection_image.source = "";
                                    lection_image.source = lectImage;
                                    sliderRotation.value = 0;
                                    buttonOkOnView.visible = false;
                                    butCancelOnView.visible = false;
                                }
                            }

                            Image {
                                source: "buttons/ok1.svg"
                                anchors.fill: parent
                                z: 2
                                antialiasing: true
                            }
                            Image {
                                source: "buttons/фон2.svg"
                                anchors.fill: parent
                                z: 1
                                antialiasing: true
                            }
                        }

                        ToolButton {
                            id: butCancelOnView
                            anchors.right:parent.right
                            y: -1
                            width: 120
                            height: 30
                            visible: false
                            opacity: 0.9
                            MouseArea {
                                hoverEnabled:true
                                anchors.fill: parent
                                onEntered: { parent.opacity = 1 }
                                onExited: { parent.opacity = 0.9 }
                                onClicked: {
                                    lection_image.source = lectImage;
                                    modelTree.cancelChanges(lectImage);
                                    buttonOkOnView.visible = false;
                                    butCancelOnView.visible = false;
                                }
                            }

                            Image {
                                source: "buttons/cancel1.svg"
                                anchors.fill: parent
                                z :2
                                antialiasing: true
                            }
                            Image {
                                source: "buttons/фон2.svg"
                                anchors.fill: parent
                                z: 1
                                antialiasing: true
                            }
                        }
                    }

                    Rectangle {
                        id: bottomView

                        anchors.left:rectBottomArea.left
                        anchors.right:rectBottomArea.right
                        anchors.bottom: rectBottomArea.bottom

                        height: 18

                        Slider {
                            id: sliderH_ScaleView
                            anchors.right: parent.right
                            width: buttonOkOnView.width*2+14
                            height: parent.height
                            visible: false
                            value:0.0
                            minimumValue:0.2
                            maximumValue:2

                        }

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
    Slider {
            id: sliderRotation
            x: 220
            y:150
            width:100
            height: 100
            visible: false
            orientation: Qt.Vertical
        }
}
