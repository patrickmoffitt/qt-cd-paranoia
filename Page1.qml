import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.4
import Qt.labs.platform 1.0

Page1Form {

    property alias messageDialog: messageDialog
    property alias searchResults: searchResults

    property var currentCategory: {}

    objectName: "page1Form"
    contentWidth: parent.width;
    contentHeight: parent.height;

    ListModel {
        id: cddbSearchResultsModel
    }

    ListModel {
        id: cddbAlbumModel
    }

    FolderDialog {
        id: folderDialog
        title: qsTr("Please choose a directory.")
        options: FolderDialog.ShowDirsOnly
        currentFolder: "file://" + QtCdParanoia.getMusicHome();
        onAccepted: {
            musicHome.text = folder.toString().replace(/^file:\/\//, '');
        }
        onRejected: {
            musicHome.text = "";
        }
    }

    pickMusicHome.onClicked: {
        musicHome.text = "";
        folderDialog.open();
    }

    freedbResults.onLinkActivated: {
        Qt.openUrlExternally(link);
    }

    driveScanButton.onClicked: {
        cddbSearchResultsModel.clear();
        artistName.text = "";
        titleName.text = "";
        cddbAlbumModel.clear();
        importStatus.text = "\n\n\n\n\n";
        QtCdParanoia.scanForDrive();
    }

    importMusic.onClicked: {
        if (musicHome.text === ""
                || musicHome.text === "Select Music Home Directory.") {
            message("No music home directory.",
                    "Please choose a music home directory.");
            return;
        }

        var importData = {
            "dest_root_dir": musicHome.text,
            "dest_artist_dir": artistName.text,
            "dest_album_dir": titleName.text,
            "tracks": {}
        };
        var i = 0;
        var track = {};
        var track_num = 0;
        for (; i<cddbAlbumModel.count; i++) {
            track = cddbAlbumModel.get(i);
            track_num = i + 1;
            importData.tracks = Object.defineProperty(
                        importData.tracks,
                        i.toString(),
                        {
                            enumerable: true,
                            value: {
                            "track_num": track_num,
                            "title": track.title
                            }
                        }
                        );
        }
        QtCdParanoia.writeTracks(importData);
    }

    cancelImportMusic.onClicked: {
        QtCdParanoia.cancelImport();
    }

    MessageDialog {
        id: messageDialog
        objectName: "messageDialog"
        title: qsTr("Warning")
        buttons: MessageDialog.Close
        Component.onCompleted: visible = false
        onYesClicked: {
            updateArtistTitleTracks(searchResults.current);
        }
        onCancelClicked: {
            currentCategory.checked = true;
        }
    }

    Connections {
        target: QtCdParanoia
        onErrorMessage: message(text, detailedText)
    }

    function message(text, detailedText) {
        messageDialog.text = text;
        messageDialog.detailedText = detailedText;
        messageDialog.buttons = MessageDialog.Close
        messageDialog.open();
    }

    function getAlbum(category) {
        var getAlbumResult = {};
        var item = {};
        for(var i=0; i<cddbSearchResultsModel.count; i++) {
            item = cddbSearchResultsModel.get(i);
            if (category === item.category) {
               getAlbumResult = {
                    "artist": item.artist,
                    "title": item.title,
                    "category": item.category
                }
            }
        }
        return getAlbumResult;
    }

    ExclusiveGroup {
        id: searchResults
        onCurrentChanged: {
            if (current && noAlbumSelected()) {
                updateArtistTitleTracks(current);
            } else if (currentCategory.objectName !== current.objectName ) {
                askDeleteArtistTitleTracks();
            }
        }
    }

    function noAlbumSelected() {
        if (artistName.text.length === 0 &&
                titleName.text.length === 0 &&
                cddbAlbumModel.count === 0) {
            return true;
        } else {
            return false;
        }
    }

    function updateArtistTitleTracks(current) {
        currentCategory = current;
        var category = current.objectName;
        var item = getAlbum(category);
        cddbAlbumModel.clear();
        QtCdParanoia.listTracks(item.category);
        if (item !== {}) {
            artistName.text = item.artist;
            titleName.text = item.title;
        }
    }

    function askDeleteArtistTitleTracks() {
        messageDialog.text = "Discard edits?";
        messageDialog.informativeText = "Changing the selected album will"
        + " discard any edits to Artist, Title, and track titles.";
        messageDialog.buttons = MessageDialog.Yes | MessageDialog.Cancel
        messageDialog.open();
    }


    importStatusView.onContentHeightChanged: {
        scrollDown();
    }

    function scrollDown() {
        importStatusView.contentY = importStatus.contentHeight -
                importStatusView.height + importStatus.padding + 12;
        importStatusView.forceActiveFocus();
    }

}
