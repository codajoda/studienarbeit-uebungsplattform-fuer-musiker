import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Qt.labs.settings

Rectangle {
    id: createProjectComponent
    width: 450
    color: "white"
    anchors.centerIn: parent

    property string projectName: projectNameField.text
    property int bpm: parseInt(bpmField.text)
    property string musicalKey: keyComboBox.currentText
    property string instrumentalPath: instrumentalFilePath
    property string recordingsFolder: recordingsFolderPath

    property string instrumentalFilePath: ""
    property string recordingsFolderPath: ""

    GridLayout {
        anchors.fill: parent
        anchors.margins: 20
        columns: 2
        columnSpacing: 20
        rowSpacing: 15

        Text {
            text: "Project Name:"
            Layout.alignment: Qt.AlignVCenter
        }
        TextField {
            id: projectNameField
            placeholderText: "Enter Project Name..."
            Layout.fillWidth: true
        }

        Text {
            text: "BPM:"
            Layout.alignment: Qt.AlignVCenter
        }
        TextField {
            id: bpmField
            placeholderText: "(optional)"
            inputMethodHints: Qt.ImhDigitsOnly
            Layout.fillWidth: true
        }

        Text {
            text: "Key:"
            Layout.alignment: Qt.AlignVCenter
        }
        ComboBox {
            id: keyComboBox
            Layout.fillWidth: true
            model: ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]
            currentIndex: 0
        }

        Text {
            text: "Instrumental Audio File:"
            Layout.alignment: Qt.AlignVCenter
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Text {
                id: instrumentalPathText
                text: instrumentalFilePath === "" ? "Not Selected" : instrumentalFilePath.split("/").pop()
                color: instrumentalFilePath === "" ? "grey" : "black"
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
            }
            Button {
                text: "Select"
                onClicked: fileDialog.open()
            }
        }

        Text {
            text: "Record Folder:"
            Layout.alignment: Qt.AlignVCenter
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Text {
                id: recordingsFolderText
                text: recordingsFolderPath === "" ? "Not Selected" : recordingsFolderPath.split("/").pop()
                color: recordingsFolderPath === "" ? "grey" : "black"
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
            }
            Button {
                text: "Select"
                onClicked: folderDialog.open()
            }
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Verzeichnis wählen"
        onAccepted: {
            recordingsFolderPath = selectedFolder;
        }
    }
    FileDialog {
        id: fileDialog
        title: "Datei wählen"
        fileMode: FileDialog.OpenFile
        onAccepted: {
            instrumentalFilePath = selectedFile;
        }
    }

    signal validFieldsChanged(bool valid)

    function checkValid() {
        var ok = projectNameField.text !== ""
              && instrumentalFilePath !== ""
              && recordingsFolderPath !== "";
        validFieldsChanged(ok);
    }

    onProjectNameChanged:    checkValid()
    onInstrumentalPathChanged: checkValid()
    onRecordingsFolderChanged: checkValid()
}
