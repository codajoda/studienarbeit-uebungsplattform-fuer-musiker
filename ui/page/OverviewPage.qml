import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.example 1.0
import "../component"

Item {
    id: overviewPage

    Layout.fillWidth: true
    Layout.fillHeight: true
    property var projects: []
    property var tags: []
    property string searchQuery: ""
    property var selectedTags: []


    Rectangle {
        id: rects
        visible: false
        z: 1000
        color: "black"
    }


    ListModel {
        id: projectsModel
    }

    ListModel {
        id: tagsModel
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            searchFieldFocus.forceActiveFocus();
        }
    }

    function filterProjects() {
        projectsModel.clear();

        for (var i = overviewPage.projects.length - 1; i >= 0; i--) {
            var project = overviewPage.projects[i];
            var hasMatchingTag = true;

            if (selectedTags.length > 0) {
                hasMatchingTag = false;

                var projectTags = DBController.getTagsForProject(project.projectId);
                for (var j = 0; j < projectTags.length; j++) {
                    if (selectedTags.indexOf(projectTags[j].name) !== -1) {
                        hasMatchingTag = true;
                        break;
                    }
                }
            }

            if (hasMatchingTag && project.projectName.toLowerCase().includes(searchQuery.toLowerCase())) {
                projectsModel.append({ "project": project });
            }
        }
        if (tagsTags) {
            tagsTags.update();
        }
    }

    onSearchQueryChanged: {
        filterProjects();
    }

    onSelectedTagsChanged: {
        filterProjects();
    }

    Column {
        width: parent.width
        height: parent.height

        Row {
            width: parent.width
            height: 70

            Item {
                width: parent.width * 0.5
                height: 70
                MouseArea {
                    id: searchFieldFocus
                    anchors.fill: parent
                    anchors.margins: 20
                    TextField {
                        id: searchField
                        placeholderText: "Search for Project"
                        font.pixelSize: 16
                        width: parent.width
                        text: overviewPage.searchQuery
                        onTextChanged: overviewPage.searchQuery = text
                    }
                }
            }

            Item {
                width: parent.width * 0.3
                height: 95
                ComboBox {
                    id: comboboxId
                    anchors.fill: parent
                    anchors.margins: 20
                    model: tagsModel

                    displayText: {
                        if (overviewPage.selectedTags.join(", ") === "") {
                            return "Select Tags";
                        }
                        return overviewPage.selectedTags.join(", ");
                    }

                    delegate: Item {
                        width: parent.width
                        height: 50
                        Row {
                            spacing: 5
                            anchors.fill: parent
                            anchors.margins: 5
                            CheckBox {
                                id: checkboxId
                                height: parent.height
                                width: height
                                onCheckedChanged: {
                                    if (checked) {
                                        overviewPage.selectedTags.push(name);
                                    } else {
                                        var index = overviewPage.selectedTags.indexOf(name);
                                        if (index !== -1) {
                                            overviewPage.selectedTags.splice(index, 1);
                                        }
                                    }
                                    overviewPage.selectedTags = selectedTags;
                                }
                            }
                            Label {
                                text: name
                                width: parent.width - checkboxId.width
                                height: parent.height
                                verticalAlignment: Qt.AlignVCenter
                                horizontalAlignment: Qt.AlignHCenter
                            }
                        }
                    }
                }
            }

            Item {
                width: parent.width * 0.2
                height: 90
                Button {
                    anchors.fill: parent
                    anchors.margins: 20
                    text: "+"
                    onClicked: {
                        newProjectDialog.open()
                    }
                }
            }
        }

        Item {
            width: parent.width
            height: parent.height - 50
            ScrollView {
                anchors.fill: parent
                anchors.margins: 20
                ScrollBar.vertical: ScrollBar {
                    policy: "AlwaysOff"
                }

                Column {
                    width: parent.width
                    height: parent.height
                    spacing: 10

                    Item {
                        visible: projectsModel.count === 0
                        width: parent.width
                        height: 200
                        Text {
                            anchors.centerIn: parent
                            text: "Keine Projekte gefunden"
                            font.pixelSize: 16
                            color: "gray"
                        }
                    }

                    Repeater {
                        model: projectsModel
                        delegate: ProjectComponent {}
                    }
                }
            }
        }
    }

    Dialog {
        id: newProjectDialog
        modal: true
        title: "Create new Project"
        standardButtons: Dialog.Ok | Dialog.Cancel
        width: 450
        height: 600
        anchors.centerIn: parent

        contentItem: Item {
            width: parent.width
            height: parent.height

            CreateProjectComponent {
                id: createProjectComponentInstance
                anchors.fill: parent
            }
        }

        Component.onCompleted: {
            newProjectDialog.standardButton(Dialog.Ok).enabled = false;
        }
        Connections {
            target: createProjectComponentInstance
            function onValidFieldsChanged(valid) {
                newProjectDialog.standardButton(Dialog.Ok).enabled = valid;
            }
        }

        onAccepted: {
            const name = createProjectComponentInstance.projectName;
            const bpm = createProjectComponentInstance.bpm;
            const key = createProjectComponentInstance.musicalKey;
            let backingTrack = createProjectComponentInstance.instrumentalPath;
            let folder = createProjectComponentInstance.recordingsFolder;

            backingTrack = backingTrack.toString().replace(/^file:\/\//, "");
            folder = folder.toString().replace(/^file:\/\//, "");

            if (typeof DBController !== "undefined") {
                DBController.createProject(name, bpm, key, backingTrack, folder);

                projectsModel.clear();
                overviewPage.projects = DBController.getAllProjects();
                for (var i = overviewPage.projects.length-1; i >= 0 ; i--) {
                    projectsModel.append({ "project": overviewPage.projects[i]});
                }
            }
        }
    }

    Dialog {
        id: editTagsDialog
        modal: true
        title: "Edit Tags for " + projectName
        width: 450
        height: 400
        anchors.centerIn: parent

        property var projectName
        property int projectId
        property bool isDialogLoading: true

        contentItem: Column {
            width: parent.width
            height: parent.height
            spacing: 40

            Column {
                width: parent.width
                spacing: 10
                Repeater {
                    model: tagsModelDialog
                    delegate: Item {
                        width: parent.width
                        height: 40

                        Row {
                            spacing: 5
                            CheckBox {
                                id: tagCheckbox
                                text: model.name
                                checked: model.isChecked

                                onCheckedChanged: {
                                    if (!editTagsDialog.isDialogLoading) {
                                        if (checked) {
                                            DBController.attachTagToProject(editTagsDialog.projectId, model.name);
                                        } else {
                                            DBController.removeTagFromProject(editTagsDialog.projectId, model.name);
                                        }
                                        overviewPage.filterProjects();
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Button {
                text: "Delete"
                onClicked: {
                    DBController.deleteProject(editTagsDialog.projectId);
                    projectsModel.clear();
                    overviewPage.projects = DBController.getAllProjects();
                    for (var i = overviewPage.projects.length-1; i >= 0 ; i--) {
                        projectsModel.append({ "project": overviewPage.projects[i]});
                    }
                    editTagsDialog.close();
                }
            }
        }

        onOpened: {
            editTagsDialog.isDialogLoading = true;
            var selectedTags = DBController.getTagsForProject(editTagsDialog.projectId);
            tagsModelDialog.clear();

            for (var i = 0; i < overviewPage.tags.length; i++) {
                var isChecked = false;

                for (var j = 0; j < selectedTags.length; j++) {
                    if (overviewPage.tags[i].name === selectedTags[j].name) {
                        isChecked = true;
                        break;
                    }
                }

                tagsModelDialog.append({ "name": overviewPage.tags[i].name, "isChecked": isChecked });
            }
            editTagsDialog.isDialogLoading = false;
        }

        ListModel {
            id: tagsModelDialog
        }
    }

    Component.onCompleted: {
        overviewPage.projects = DBController.getAllProjects();
        for (var i = overviewPage.projects.length-1; i >= 0 ; i--) {
            projectsModel.append({ "project": overviewPage.projects[i]});
        }
        overviewPage.tags = DBController.getTags();
        for (var j = 0; j < overviewPage.tags.length; j++) {
            tagsModel.append({ "name": overviewPage.tags[j].name, "isChecked": false });
        }
    }
}
