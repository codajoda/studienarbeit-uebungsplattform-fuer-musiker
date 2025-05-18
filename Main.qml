import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import "ui/component"
import "ui/page"

ApplicationWindow {
    visible: true
    width: 1600
    height: 900
    title: "Musica"

    Material.theme: Material.Light

    ColumnLayout {
        anchors.fill: parent

        TopBarComponent {
            Layout.preferredHeight: 50
            Layout.fillWidth: true
            onOpenSettings: audioDialog.open()
        }

        Dialog {
            id: audioDialog
            title: "Audio Einstellungen"
            modal: true
            standardButtons: Dialog.Close
            width: 450
            height: 600
            anchors.centerIn: parent

            contentItem: Item {
                width: parent.width
                height: parent.height

                AudioSettingsComponent {
                    anchors.fill: parent
                    color: audioDialog.background.color
                }
            }
        }

        StackView {
            z: -1
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: OverviewPage {
            }
        }

        ToolBar {
            Layout.preferredHeight: 50
            Layout.fillWidth: true
        }
    }
}
