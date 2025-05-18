import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.example 1.0
import "../component"

Item {
    id: projectPage

    Layout.fillWidth: true
    Layout.fillHeight: true
    property bool isIslandVisible: true
    property int projectId
    property var project
    property var instrumentalRecording;
    property var recordings: []
    property var recordingDisplayNames: []

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Button {
                text: "← Back"
                font.pixelSize: 16
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 20
                onClicked: function() {
                    AudioController.clearTracks();
                    AudioController.stopMonitoring();
                    stackView.pop();
                }
            }

            Item { Layout.fillWidth: true }

            Text {
                text: "Name: " + project.projectName
                font.bold: true
                Layout.leftMargin: 20
                Layout.rightMargin: 10
            }
            Text {
                text: "BPM: " + project.beatsPerMinute
                font.bold: true
                Layout.leftMargin: 10
                Layout.rightMargin: 10
            }
            Text {
                text: "Key: " + project.musicalKey
                font.bold: true
                Layout.leftMargin: 10
                Layout.rightMargin: 20
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Effects"
                font.pixelSize: 16
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 20
                Layout.rightMargin: 20
                onClicked: function() {
                    effectDialog.open();
                }
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Mute All"
                font.pixelSize: 16
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 20
                Layout.rightMargin: 10
                onClicked: function() {
                    AudioController.muteAll();
                }
            }

            Button {
                text: "Unmute All"
                font.pixelSize: 16
                Layout.alignment: Qt.AlignLeft
                Layout.leftMargin: 10
                Layout.rightMargin: 20
                onClicked: function() {
                    AudioController.unmuteAll();
                }
            }

            Item { Layout.fillWidth: true }

            Text {
                text: "Input Volume"
                font.bold: true
            }

            Slider {
                id: inputVolumeSlider
                orientation: Qt.Horizontal
                from: 0.0
                to: 4.0
                stepSize: 0.05
                value: 1.0

                onValueChanged: {
                    AudioController.setInputVolume(value);
                }
            }

            Text {
                text: "Main Volume"
                font.bold: true
            }

            Slider {
                id: mainVolumeSlider
                orientation: Qt.Horizontal
                from: 0
                to: 100
                stepSize: 1
                value: 50

                onValueChanged: {
                    AudioController.setMainVolume(value);
                }
            }
        }

        Dialog {
            id: effectDialog
            title: "Effects"
            modal: true
            standardButtons: Dialog.Close
            width: 450
            height: 600
            anchors.centerIn: parent

            contentItem: Item {
                width: parent.width
                height: parent.height

                EffectsComponent {
                    anchors.fill: parent
                }
            }
        }

        Item {
            z: -1
            Layout.leftMargin: 20
            Layout.rightMargin: 20
            Layout.fillWidth: true

            Layout.preferredHeight: parent.height

            ScrollView {
                anchors.fill: parent
                Layout.fillHeight: true
                contentHeight: parent.height - 80

                ListModel {
                    id: recordingsModel
                }

                ListView {
                    id: listView
                    anchors.fill: parent
                    model: recordingsModel
                    spacing: 10

                    cacheBuffer: 1000000000


                    delegate: TrackComponent {
                        record: modelData

                        Connections {
                            function onDeleteTrack(trackIdToDelete) {
                                for (var i = 0; i < recordingsModel.count; i++) {
                                    if (recordingsModel.get(i).record.id === trackIdToDelete) {
                                        recordingsModel.remove(i);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    RecordIslandComponent {
        id: recordIslandComponent
        projectId: projectPage.projectId
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: toggleButton.top
        anchors.margins: 10
        isVisible: projectPage.isIslandVisible
    }

    Button {
        id: toggleButton
        text: projectPage.isIslandVisible ? "▼" : "▲"
        font.pixelSize: 20
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.margins: 20

        onClicked: projectPage.isIslandVisible = !projectPage.isIslandVisible
    }

    Component.onCompleted: {
        projectPage.recordings = DBController.getRecordings(projectId);
        for (var j = projectPage.recordings.length-1; j>= 0; j--) {
            if (projectPage.recordings[j].isBackingTrack) {
                recordingsModel.append({"record": projectPage.recordings[j]});
            }
        }
        for (var i = projectPage.recordings.length-1; i >= 0; i--) {
            if (!projectPage.recordings[i].isBackingTrack) {
                recordingsModel.append({"record": projectPage.recordings[i]});
            }
        }
        AudioController.addTracks(projectPage.recordings);
    }

    Connections {
        target: AudioController
        function onRecordingFinished(newRecording) {
            recordingsModel.move(0, recordingsModel.count-1, 1);
            recordingsModel.append({ "record": newRecording });
            var count = recordingsModel.count;
            recordingsModel.move(0, 2, count-2);
            AudioController.addTracks([newRecording]);
        }
        function onRecordingStarted() {

        }
    }
}
