import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    width: parent.width
    height: 150

    Rectangle {
        width: parent.width
        height: parent.height
        radius: 15
        color: "#F4F4F4"
        border.color: "#D81B60"
        border.width: 2
        Text {
            text: modelData.projectName
            font.family: "Helvetica"
            font.pointSize: 16
            font.bold: true
            color: "black"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 30
        }
        Text {
            text: {
                let date = new Date(modelData.createdAt);
                return "(Created: " + date.toLocaleDateString(Locale.ShortFormat, { month: 'long' }) + ")";
            }

            font.family: "Helvetica"
            font.pointSize: 16
            font.bold: true
            color: "grey"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 30
            anchors.topMargin: 60
        }

        Row {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 30
            anchors.topMargin: 90
            spacing: 20
            width: parent.width

            Repeater {
                id: tagsTags
                model: DBController.getTagsForProject(modelData.projectId)
                delegate: Item {
                    width: 100
                    height: 30

                    Rectangle {
                        color: "#FF8A80"
                        radius: 12
                        height: 30
                        width: 100

                        Text {
                            text: modelData.name
                            color: "white"
                            anchors.centerIn: parent
                            font.pixelSize: 14
                        }
                    }
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                rects.visible = true;
                stackView.push(Qt.createComponent("../page/ProjectPage.qml"), {
                                   projectId: modelData.projectId,
                                   project: modelData
                               });
                rects.visible = false;
            }
        }

        Button {
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 10
            text: "Edit"
            onClicked: {
                editTagsDialog.projectId = modelData.projectId
                editTagsDialog.projectName = modelData.projectName
                editTagsDialog.open();
            }
        }
    }
}
