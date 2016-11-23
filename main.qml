import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Lection Reader")

    menuBar: MenuBar {
        Menu {
            title: qsTr("Rotation")
            MenuItem {
                text: qsTr("Current image")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Choose \& rotate")
                onTriggered: Qt.quit();
            }
        }
    }

    MainForm {
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
