import QtQuick
import QtCharts

Item {
    id: waveFormComponent

    width: 800

    property var offset;
    property var trackId;
    property color defaultColor: "#F8D231"
    property color muteColor: "#CFD3D6"
    property color soloColor: "Blue"
    property color waveColor: defaultColor

    property real currentPosition: 0
    property real mouseTimePosition: 0
    property real mouseXPosition: 0
    property real mouseYPosition: 0

    property bool isRecording: false;

    property var waveform;

    property int i: 0;

    Rectangle {
        id: playhead
        width: 2
        height: parent.height
        color: "red"
        anchors.top: parent.top

        Behavior on x {
            NumberAnimation {
                duration: 100
                easing.type: Easing.InOutQuad
            }
        }

        x: Math.min(waveFormComponent.currentPosition / axisX.max * parent.width, parent.width)
    }

    Rectangle {
        id: previewWlayhead
        width: 2
        height: parent.height
        color: "blue"
        anchors.top: parent.top
        visible: false
        x: mouseTimePosition / axisX.max * parent.width
    }

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent
        onClicked: function(mouse) {
            if (waveFormComponent.isRecording) {
                return;
            }
            var newX = mouse.x;
            var newPosition = newX / parent.width * axisX.max;
            waveFormComponent.updatePlayheadPosition(newPosition);
            AudioController.setPlayheadPosition(newPosition);
        }
        onPositionChanged: function(mouse) {
            var newX = mouse.x;
            mouseTimePosition = newX / parent.width * axisX.max;
            previewWlayhead.visible = true;
            timeBackground.visible = true;
            mouseXPosition = mouse.x;
            mouseYPosition = mouse.y;
        }
        onExited: function() {
            mouseTimePosition = currentPosition;
            previewWlayhead.visible = false;
            timeBackground.visible = false;
        }
    }


    Rectangle {
        id: timeBackground
        color: defaultColor
        radius: 5
        width: 70
        height: 30
        x: waveFormComponent.mouseXPosition - width / 2
        y: waveFormComponent.mouseYPosition - height - 10
        z: 5
        visible: false

        Text {
            id: timeDisplay
            text: formatTime(mouseTimePosition)
            color: "black"
            font.pixelSize: 18
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    ChartView {
        id: chart
        anchors.fill: parent
        backgroundColor: "transparent"
        antialiasing: true
        legend.visible: false
        plotArea: Qt.rect(0, 0, parent.width, parent.height)

        ValuesAxis {
            id: axisX
            min: 0
            max: 0
            tickCount: 5
            visible: false
        }

        ValuesAxis {
            id: axisY
            min: -0.6
            max: 0.6
            tickCount: 2
            visible: false
        }

        LineSeries {
            id: waveformSeries
            axisX: axisX
            axisY: axisY
            color: waveFormComponent.waveColor
            width: 2
            property int xIndex: 0
        }

        z: -1
    }

    Component.onCompleted: {
        waveFormComponent.waveform = AudioController.getWaveformForFile(trackId);
        waveformSeries.clear();

        if (waveFormComponent.waveform.length > 0) {
            axisX.max = waveFormComponent.waveform[waveFormComponent.waveform.length - 1].timestamp;
        }

        currentPosition = waveFormComponent.waveform[0].timestamp;
    }

    Timer {
        interval: 20; running: true; repeat: true
        onTriggered: rara(2)
    }

    function rara(a) {
        for (var j = 0; j <= a; j++ ) {
            if (waveFormComponent.i < waveFormComponent.waveform.length - 1) {
                waveformSeries.append(waveFormComponent.waveform[waveFormComponent.i].timestamp, waveFormComponent.waveform[waveFormComponent.i].value * 2);
            }
            i++;
        }
    }

    function mutedSolo(isMuted, isSolo) {
        if (isSolo) {
            waveColor = soloColor;
        } else if (isMuted) {
            waveColor = muteColor;
        } else {
            waveColor = defaultColor;
        }
    }

    function updatePlayheadPosition(newPosition) {
        if (waveFormComponent.waveform[0].timestamp <= newPosition) {
            if (newPosition <= waveFormComponent.waveform[waveFormComponent.waveform.length-2].timestamp) {
                currentPosition = newPosition;
            } else {
                currentPosition = waveFormComponent.waveform[waveFormComponent.waveform.length-2].timestamp;
            }
        } else {
            currentPosition = waveFormComponent.waveform[0].timestamp;
        }
    }

    Timer {
        interval: 50
        running: true
        repeat: true
        onTriggered: {
            var newPosition = AudioController.getCurrentPlayheadPosition();
            waveFormComponent.updatePlayheadPosition(newPosition);
        }
    }

    function formatTime(seconds) {
        var minutes = Math.floor(seconds / 60);
        var secs = Math.floor(seconds % 60);
        return (minutes < 10 ? "0" + minutes : minutes) + ":" + (secs < 10 ? "0" + secs : secs);
    }

    Connections {
        target: AudioController
        function onRecordingStarted() {
            waveFormComponent.isRecording = true;
        }
        function onRecordingFinished(recording) {
            waveFormComponent.isRecording = false;
        }
    }
}
