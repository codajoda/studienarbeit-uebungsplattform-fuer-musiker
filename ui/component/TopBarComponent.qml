import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../component"

ToolBar {
    id: topBarComponent
    Layout.fillWidth: true

    signal openSettings()

    RowLayout {
        anchors.fill: parent
        spacing: 20

        Label {
            text: "Musica"
            font.pixelSize: 20
            Layout.leftMargin: 20
        }

        Item { Layout.fillWidth: true }

        ToolButton {
            id: settingsButton
            text: "Settings"
            onClicked: topBarComponent.openSettings()
        }
    }

    Connections {
        target: AudioController
        function onRecordingStarted() {
            settingsButton.enabled = false;
        }
        function onRecordingFinished(recording) {
            settingsButton.enabled = true;
        }

        function onSendPitch(pitch) {
            if (tunerDialog.visible) {
                tunerDialog.updateFrequency(pitch);
            }
        }
    }
}
