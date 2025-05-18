import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.settings

Item {

    Label {
        id: outputLabel
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        width: 100
        text: "OUTPUT"
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    ColumnLayout {
        id: effectsGridLayout
        width: parent.width
        anchors.top: outputLabel.bottom
        anchors.topMargin: 5
        spacing: 20



        Repeater {
            id: effectRepeater
            model: AudioController.getUsedEffects()

            delegate: Item {
                visible: modelData !== "Raw"

                width: effectsGridLayout.width
                height: 50
                RowLayout {
                    spacing: 5

                    Label {
                        Layout.preferredWidth: 100
                        text: modelData
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    Button {
                        Layout.preferredWidth: 100
                        text: "↑"
                        font.pixelSize: 12
                        onClicked: {
                            AudioController.moveEffectFromTo(index, index-1);
                            effectRepeater.model = AudioController.getUsedEffects();
                        }
                    }

                    Button {
                        Layout.preferredWidth: 100
                        text: "↓"
                        font.pixelSize: 12
                        onClicked: {
                            AudioController.moveEffectFromTo(index, index+1);
                            effectRepeater.model = AudioController.getUsedEffects();
                        }
                    }

                    Button {
                        Layout.preferredWidth: 100
                        text: "Delete"
                        font.pixelSize: 12
                        onClicked: {
                            AudioController.removeEffect(index);
                            effectRepeater.model = AudioController.getUsedEffects();
                        }
                    }
                }
            }
        }
        Button {
            id: addButton
            visible: effectRepeater.model.length <= 6
            Layout.alignment: Qt.AlignHCenter
            text: "Add"
            font.pixelSize: 12
            onClicked: {
                effectDialog.open()

            }
        }
    }
    Label {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 100
        text: "INPUT"
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Dialog {
        id: effectDialog
        title: "Select Audio Effect"
        visible: false
        anchors.centerIn: parent

        ColumnLayout {
            spacing: 10

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                Layout.alignment: Qt.AlignHCenter
                ComboBox {
                    id: effectComboBox
                    anchors.horizontalCenter: parent.horizontalCenter
                    model: ["Reverb", "Delay", "Distortion"]
                    font.pixelSize: 14
                }
            }

            RowLayout {
                spacing: 10

                Button {
                    text: "Cancel"
                    onClicked: {
                        effectDialog.close();
                    }
                }

                Button {
                    text: "Add"
                    onClicked: {
                        AudioController.addEffect(effectComboBox.currentText);
                        effectDialog.close();
                        effectRepeater.model = AudioController.getUsedEffects();
                    }
                }
            }
        }
    }
}
