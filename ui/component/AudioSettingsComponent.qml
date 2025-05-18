import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: audioSettingsComponent
    width: 450
    height: 400
    anchors.centerIn: parent

    GridLayout {
        anchors.fill: parent
        columns: 2
        columnSpacing: 20
        rowSpacing: 15

        Text {
            text: "Input Device:"
            Layout.alignment: Qt.AlignVCenter
        }
        ComboBox {
            id: inputDeviceDropdown
            model: AudioController.getAvailableInputDevices()
            currentIndex: AudioController.getAvailableInputDevices().indexOf(AudioController.getCurrentInputDevice())
            Layout.fillWidth: true
            onActivated: (index) => {
                let selectedDevice = model[index];
                AudioController.setInputDevice(selectedDevice);
                inputChannelsListView.model = AudioController.getAvailableInputChannels();
            }
            onFocusChanged: {
                inputDeviceDropdown.model = AudioController.getAvailableInputDevices();
                inputDeviceDropdown.currentIndex = AudioController.getAvailableInputDevices().indexOf(AudioController.getCurrentInputDevice());
            }
        }

        Text {
            text: "Input Channels:"
            Layout.alignment: Qt.AlignVCenter
        }
        ListView {
            id: inputChannelsListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: AudioController.getAvailableInputChannels()
            delegate: Item {
                width: inputChannelsListView.width
                height: 40
                RowLayout {
                    spacing: 10

                    CheckBox {
                        text: "Input " + (index+1)
                        checked: AudioController.isInputChannelActive(modelData)
                        onClicked: {
                            AudioController.setInputChannel(index, checked);
                        }
                    }
                }
            }
        }

        Text {
            text: "Output Device:"
            Layout.alignment: Qt.AlignVCenter
        }
        ComboBox {
            id: outputDeviceDropdown
            model: AudioController.getAvailableOutputDevices()
            currentIndex: AudioController.getAvailableOutputDevices().indexOf(AudioController.getCurrentOutputDevice())
            Layout.fillWidth: true
            onActivated: (index) => {
                let selectedDevice = model[index];
                AudioController.setOutputDevice(selectedDevice);
                outputChannelsListView.model = AudioController.getAvailableOutputChannels();
            }
            onFocusChanged: {
                outputDeviceDropdown.model = AudioController.getAvailableOutputDevices();
                outputDeviceDropdown.currentIndex = AudioController.getAvailableOutputDevices().indexOf(AudioController.getCurrentOutputDevice());
            }
        }

        Text {
            text: "Output Channels:"
            Layout.alignment: Qt.AlignVCenter
        }
        ListView {
            id: outputChannelsListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: AudioController.getAvailableOutputChannels()
            delegate: Item {
                width: outputChannelsListView.width
                height: 40
                RowLayout {
                    spacing: 10

                    CheckBox {
                        text: "Output " + (index+1)
                        checked: AudioController.isOutputChannelActive(modelData)
                        onClicked: {
                            AudioController.setOutputChannel(index, checked);
                        }
                    }
                }
            }
        }


        Text {
            text: "Sample Rate:"
            Layout.alignment: Qt.AlignVCenter
        }
        ComboBox {
            id: sampleRateDropdown
            model: AudioController.getAvailableSampleRates()
            currentIndex: AudioController.getAvailableSampleRates().indexOf(AudioController.getCurrentSampleRate())
            Layout.fillWidth: true
            onActivated: (index) => {
                let sampleRate = model[index];
                AudioController.setSampleRate(sampleRate);
            }
            onFocusChanged: {
                sampleRateDropdown.model = AudioController.getAvailableSampleRates();
                sampleRateDropdown.currentIndex = AudioController.getAvailableSampleRates().indexOf(AudioController.getCurrentSampleRate());

            }
        }

        Text {
            text: "Buffer Size:"
            Layout.alignment: Qt.AlignVCenter
        }
        ComboBox {
            id: bufferSizeDropdown
            model: AudioController.getAvailableBufferSizes()
            currentIndex: AudioController.getAvailableBufferSizes().indexOf(AudioController.getCurrentBufferSize())
            Layout.fillWidth: true
            onActivated: (index) => {
                let bufferSize = model[index];
                AudioController.setBufferSize(bufferSize);
            }
            onFocusChanged: {
                bufferSizeDropdown.model = AudioController.getAvailableBufferSizes();
                bufferSizeDropdown.currentIndex = AudioController.getAvailableBufferSizes().indexOf(AudioController.getCurrentBufferSize());
            }
        }
    }
}
