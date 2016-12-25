import QtQuick 2.5
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Lection Reader")
    x: Screen.width / 2 - mainWindow.width / 2;
    y: Screen.height / 2 - mainWindow.height / 2;

    menuBar: MenuBar {
        Menu {
            title: qsTr("Rotation")
            MenuItem {
                text: qsTr("Current image")
            }
            MenuItem {
                text: qsTr("Choose \& rotate")
                onTriggered: Qt.quit();
            }
        }
    }

    MainForm {
        id: mainForm
        anchors.fill: parent
     }


    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

}
