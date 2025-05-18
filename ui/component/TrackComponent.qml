import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: trackComponent

    property var record

    property bool isMuted: false
    property bool isSolo: false

    property color defaultColor: "#F8D231"
    property color muteColor: "#CFD3D6"
    property color soloColor: "Blue"

    signal deleteTrack(int trackId)

    width: parent.width
    height: 180

    Rectangle {
        width: parent.width
        height: parent.height
        color: "#F4F4F4"

        border.color: record.isBackingTrack ? "#D81B60" : "transparent"
        border.width: record.isBackingTrack ? 2 : 0

        ColumnLayout {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 30
            width: parent.width

            spacing: 10

            Item {
                height: 40
                Layout.preferredWidth: parent.width/2
                Layout.alignment: Qt.AlignLeft

                Text {
                    id: trackNameText
                    text: trackComponent.record.displayName


                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter

                    font.family: "Helvetica"
                    font.pointSize: 14
                    font.bold: true
                    color: "black"
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        trackNameText.visible = false;
                        trackNameEdit.visible = true;
                        trackNameEdit.forceActiveFocus();
                    }
                }

                TextField {
                    id: trackNameEdit
                    visible: false
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    text: trackNameText.text
                    font.family: "Helvetica"
                    horizontalAlignment: TextInput.AlignLeft
                    font.pointSize: 14
                    font.bold: true
                    color: "black"
                    width: parent.width - 40
                    height: 40
                    onEditingFinished: {
                        trackNameText.text = trackNameEdit.text;
                        trackNameEdit.visible = false;
                        trackNameText.visible = true;

                        DBController.updateTrackName(trackComponent.record.id, trackNameEdit.text);
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 10

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.preferredWidth: 100
                    spacing: 10

                    Button {
                        text: trackComponent.isMuted ? "MUTED" : "UNMUTED"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        background: Rectangle {
                            color: trackComponent.isMuted ? trackComponent.muteColor : trackComponent.defaultColor
                            radius: 5
                        }
                        onClicked: {
                            trackComponent.isMuted = !trackComponent.isMuted;
                            waveFormComponent.mutedSolo(trackComponent.isMuted, trackComponent.isSolo);
                            AudioController.updateTrack(trackComponent.record.id, trackComponent.isMuted, trackComponent.isSolo);
                        }
                    }

                    Button {
                        text: trackComponent.isSolo ? "SOLO" : "UNSOLO"
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        background: Rectangle {
                            color: trackComponent.isSolo ? trackComponent.soloColor : trackComponent.muteColor
                            radius: 5
                        }
                        onClicked: {
                            trackComponent.isSolo = !trackComponent.isSolo;
                            waveFormComponent.mutedSolo(trackComponent.isMuted, trackComponent.isSolo);
                            AudioController.updateTrack(trackComponent.record.id, trackComponent.isMuted, trackComponent.isSolo);
                        }
                    }
                }

                WaveFormComponent {
                    id: waveFormComponent
                    offset: trackComponent.record.startTime / 1000
                    trackId: trackComponent.record.id
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: 20
                }

                Slider {
                    id: volumeSlider
                    orientation: Qt.Vertical
                    Layout.preferredHeight: 100
                    from: 0
                    to: 100
                    stepSize: 1
                    value: 50

                    onValueChanged: {
                        AudioController.setTrackVolume(trackComponent.record.id, value);
                    }
                }
            }
        }

        Button {

            text: "Delete"
            font.pixelSize: 12

            height: 35
            width: 90

            anchors.rightMargin: 10
            anchors.topMargin: 10

            anchors.right: parent.right
            anchors.top: parent.top


            visible: !trackComponent.record.isBackingTrack
            onClicked: {
                DBController.deleteTrack(trackComponent.record.id);
                AudioController.removeTrack(trackComponent.record.id);
                deleteTrack(trackComponent.record.id);
            }
        }
    }

    Connections {
        target: AudioController
        function onSoloChanged(solo, trackId) {
            if (trackComponent.record.id === trackId) {
                trackComponent.isSolo = solo;
            }
            waveFormComponent.mutedSolo(trackComponent.isMuted, trackComponent.isSolo);
        }
        function onAllMuteChanged(muted) {
            trackComponent.isMuted = muted;
            waveFormComponent.mutedSolo(trackComponent.isMuted, trackComponent.isSolo);
        }
    }
}

