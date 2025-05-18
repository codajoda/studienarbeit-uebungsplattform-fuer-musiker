import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.settings
import "../component"

Rectangle {
    id: recordIslandComponent
    property alias isVisible: recordIslandComponent.visible
    property int projectId
    height: 100
    radius: 20
    color: "#333"
    border.color: isRecording ? "#ff4b82" : "#666"
    border.width: 3
    visible: isVisible ? 1.0 : 0.0
    property bool isPlaying: false
    property bool isRecording: false

    implicitWidth: recordRow.implicitWidth + 20
    implicitHeight: recordRow.implicitHeight + 20

    Behavior on opacity { NumberAnimation { easing.type: Easing.InCubic; duration: 300 } }

    MouseArea {
        anchors.fill: parent

    }

    RowLayout {
        id: recordRow
        anchors.centerIn: parent
        spacing: 20

        Button {
            id: playStopButton
            text: isPlaying ? "Stop" : "Play"
            font.pixelSize: 20

            background: Rectangle {
                color: "#cecbc4"
                radius: 20
            }

            onClicked: {
                if (isPlaying) {
                    AudioController.stop();
                } else {
                    AudioController.start();
                }
                isPlaying = !isPlaying;
            }
        }

        Button {
            text: "⏺"
            id: button
            property string standbyColor: "#cecbc4"

            font.pixelSize: 24
            background: Rectangle {
                id: recButton
                color: button.standbyColor
                radius: 20
                Behavior on color {
                    SequentialAnimation {
                        id: recordAnimation
                        loops: Animation.Infinite
                        ColorAnimation { from: "white"; to: "red"; duration: 300 }
                        ColorAnimation { from: "red"; to: "white";  duration: 300 }

                        onLoopsChanged: {
                            if (loops === 0) {
                                ha.start();
                            }
                        }
                    }
                }
                ColorAnimation on color { id: ha; to: button.standbyColor; duration: 500; loops: 1}
            }


            onClicked: {
                recordIslandComponent.isRecording = !recordIslandComponent.isRecording;
                if (recordIslandComponent.isRecording) {
                    recordAnimation.loops = Animation.Infinite;
                    recButton.color = "white";
                    recButton.color = "red";
                    AudioController.startRecord(recordIslandComponent.projectId);
                    if (!recordIslandComponent.isPlaying) {
                        playStopButton.click();
                    }
                    playStopButton.enabled = false;
                } else {
                    recordAnimation.loops = 0;
                    recButton.color = button.standbyColor;
                    AudioController.stopRecord(recordIslandComponent.projectId);
                    playStopButton.enabled = true;
                }
            }
        }

        Button {
            id: monitoringButton
            property bool isMonitoring: false
            text: isMonitoring ? "🎧 🔵" : "🎧 ⚪️"
            font.pixelSize: 24
            background: Rectangle {
                color: "#cecbc4"
                radius: 20
            }

            onClicked: {
                isMonitoring = !isMonitoring
                console.log("Monitoring " + (isMonitoring ? "aktiviert" : "deaktiviert"))

                if (isMonitoring) {
                    AudioController.startMonitoring()
                } else {
                    AudioController.stopMonitoring()
                }
            }
        }
    }

    Connections {
        target: AudioController
        function onPlayingFinished() {
            isPlaying = false;
            if (isRecording) {
                recordIslandComponent.isRecording = !recordIslandComponent.isRecording;
                AudioController.stopRecord(recordIslandComponent.projectId);
            }
            playStopButton.enabled = true;
        }
    }
}
