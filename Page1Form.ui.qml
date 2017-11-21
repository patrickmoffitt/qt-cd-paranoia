import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4

Flickable {

    anchors.fill: parent
    contentWidth: Page1Form.contentWidth
    contentHeight: Page1Form.contentHeight
    ScrollBar.vertical: ScrollBar {
    }
    ScrollBar.horizontal: ScrollBar {
    }

    property alias driveScanButton: driveScanButton
    property alias driveScanBusyIndicator: driveScanBusyIndicator
    property alias driveName: driveName
    property alias pickMusicHome: pickMusicHome
    property alias musicHome: musicHome
    property alias freedbResults: freedbResults
    property alias cddbSearchResultsView: cddbSearchResultsView
    property alias cddbAlbumView: cddbAlbumView
    property alias artistName: artistName
    property alias titleName: titleName
    property alias importMusic: importMusic
    property alias cancelImportMusic: cancelImportMusic
    property alias importStatusView: importStatusView
    property alias importStatus: importStatus

    Item {
        id: mainForm
        anchors.fill: parent
        width: parent.width
        height: parent.height

        RoundButton {
            id: driveScanButton
            height: mainWindow.buttonHeight
            radius: mainWindow.buttonRadius
            text: qsTr("Drive Scan")
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: horizontalMargin
            font.family: mainWindow.fontFamily
            font.capitalization: mainWindow.fontCapitalization
            font.pointSize: mainWindow.fontSize
            ToolTip.visible: hovered
            ToolTip.delay: 2000
            ToolTip.text: qsTr("Scan for a CD-ROM drive with an Audio CD.")

            BusyIndicator {
                id: driveScanBusyIndicator
                objectName: "driveScanBusyIndicator"
                width: 38
                height: 38
                visible: true
                running: false
                anchors.top: parent.top
                anchors.left: driveScanButton.right
                anchors.leftMargin: 10
                Connections {
                    target: QtCdParanoia
                    onBusyIndicator: driveScanBusyIndicator.running
                                     = !driveScanBusyIndicator.running
                }
            }

            Text {
                id: driveName
                objectName: "driveName"
                width: parent.width - driveScanButton.width - (2 * horizontalMargin)
                anchors.verticalCenter: driveScanButton.verticalCenter
                anchors.left: driveScanButton.right
                anchors.leftMargin: 10
                text: "No disc found."
                font.family: mainWindow.fontFamily
                font.pointSize: mainWindow.fontSize
                Connections {
                    target: QtCdParanoia
                    onDriveName: driveName.text = text
                }
            }
        }

        Label {
            id: freedbResults
            text: qsTr('<a href="http://www.freedb.org/">freedb</a> Artist and Title')
            textFormat: Text.StyledText
            linkColor: "blue"
            anchors.top: driveScanButton.bottom
            anchors.topMargin: mainWindow.topMargin
            anchors.left: driveScanButton.left
            anchors.leftMargin: 7
            font.family: mainWindow.fontFamily
            font.capitalization: mainWindow.fontCapitalization
            font.pointSize: mainWindow.fontSize
        }

        TableView {
            id: cddbSearchResultsView
            objectName: "cddbSearchResultsView"
            model: cddbSearchResultsModel
            anchors.top: freedbResults.bottom
            anchors.left: freedbResults.left
            anchors.topMargin: 7
            frameVisible: false
            sortIndicatorVisible: false
            width: parent.width - (2 * horizontalMargin)
            height: 99
            anchors.leftMargin: 0
            Connections {
                target: QtCdParanoia
                onCddbSearchResult: cddbSearchResultsModel.append(result)
            }

            TableViewColumn {
                title: qsTr("Pick")
                width: 40
                delegate: RadioButton {
                    objectName: model.category
                    exclusiveGroup: searchResults
                    anchors.left: parent ? parent.left : undefined
                    anchors.leftMargin: 15
                }
            }
            TableViewColumn {
                role: "category"
                title: qsTr('Category')
                width: 75
            }
            TableViewColumn {
                role: "artist"
                title: qsTr('Artist')
                width: 100
            }
            TableViewColumn {
                role: "title"
                title: qsTr('Title')
                width: 500
            }
        }

        Label {
            id: artistLabel
            text: qsTr('Artist')
            anchors.top: cddbSearchResultsView.bottom
            anchors.topMargin: mainWindow.topMargin * 2
            anchors.left: cddbSearchResultsView.left
            font.family: mainWindow.fontFamily
            font.capitalization: mainWindow.fontCapitalization
            font.pointSize: mainWindow.fontSize
        }

        TextField {
            id: artistName
            width: cddbSearchResultsView.width
            anchors.top: artistLabel.bottom
            anchors.left: artistLabel.left
            height: textFieldHeight
        }

        Label {
            id: titleLabel
            text: qsTr('Title')
            anchors.top: artistName.bottom
            anchors.topMargin: mainWindow.topMargin
            anchors.left: artistName.left
            font.family: mainWindow.fontFamily
            font.capitalization: mainWindow.fontCapitalization
            font.pointSize: mainWindow.fontSize
        }

        TextField {
            id: titleName
            width: cddbSearchResultsView.width
            anchors.top: titleLabel.bottom
            anchors.left: titleLabel.left
            height: textFieldHeight
        }

        Label {
            id: tracksLabel
            text: qsTr('Tracks')
            anchors.top: titleName.bottom
            anchors.topMargin: mainWindow.topMargin
            anchors.left: titleName.left
            font.family: mainWindow.fontFamily
            font.capitalization: mainWindow.fontCapitalization
            font.pointSize: mainWindow.fontSize
        }

        TableView {
            id: cddbAlbumView
            model: cddbAlbumModel
            anchors.top: tracksLabel.bottom
            anchors.left: tracksLabel.left
            frameVisible: false
            sortIndicatorVisible: false
            width: cddbSearchResultsView.width
            height: 135
            anchors.leftMargin: 0
            Connections {
                target: QtCdParanoia
                onCddbTrackInfo: cddbAlbumModel.append(result)
            }

            TableViewColumn {
                role: "track"
                title: qsTr('#')
                width: 30
                horizontalAlignment: 3
            }

            TableViewColumn {
                role: "length"
                title: qsTr('Length')
                width: 70
            }

            TableViewColumn {
                role: "title"
                title: qsTr('Title')
                width: 500
                delegate: TextInput {
                    text: styleData.value
                }
            }
        }

        RoundButton {
            id: pickMusicHome
            height: mainWindow.buttonHeight
            radius: mainWindow.buttonRadius
            text: qsTr("Choose")
            anchors.top: cddbAlbumView.bottom
            anchors.topMargin: mainWindow.topMargin
            anchors.left: cddbAlbumView.left
            font.family: mainWindow.fontFamily
            font.capitalization: mainWindow.fontCapitalization
            font.pointSize: mainWindow.fontSize
            ToolTip.visible: hovered
            ToolTip.delay: 2000
            ToolTip.text: qsTr("Choose a home directory for music imports.")

            Text {
                id: musicHome
                text: qsTr("Select Music Home Directory.")
                width: parent.width - pickMusicHome.width - (2 * horizontalMargin)
                anchors.verticalCenter: pickMusicHome.verticalCenter
                anchors.left: pickMusicHome.right
                anchors.leftMargin: 10
                font.family: mainWindow.fontFamily
                font.pointSize: mainWindow.fontSize
            }
        }

        RoundButton {
            id: importMusic
            height: mainWindow.buttonHeight
            radius: mainWindow.buttonRadius
            text: qsTr("Import")
            anchors.top: pickMusicHome.bottom
            anchors.topMargin: mainWindow.topMargin
            anchors.left: cddbAlbumView.left
            font.family: mainWindow.fontFamily
            font.capitalization: mainWindow.fontCapitalization
            font.pointSize: mainWindow.fontSize
            ToolTip.visible: hovered
            ToolTip.delay: 2000
            ToolTip.text: qsTr("Import tracks as WAV files.")
        }

        RoundButton {
            id: cancelImportMusic
            height: mainWindow.buttonHeight
            radius: mainWindow.buttonRadius
            text: qsTr("Cancel Import")
            anchors.top: pickMusicHome.bottom
            anchors.topMargin: mainWindow.topMargin
            anchors.left: importMusic.right
            anchors.leftMargin: 75
            font.family: mainWindow.fontFamily
            font.capitalization: mainWindow.fontCapitalization
            font.pointSize: mainWindow.fontSize
            ToolTip.visible: hovered
            ToolTip.delay: 2000
            ToolTip.text: qsTr("Stop writing tracks as WAV files.")
        }

        Label {
            id: trackWriteStatusLabel
            text: qsTr('Track Write Status')
            anchors.top: importMusic.bottom
            anchors.topMargin: mainWindow.topMargin
            anchors.left: importMusic.left
            font.family: mainWindow.fontFamily
            font.capitalization: mainWindow.fontCapitalization
            font.pointSize: mainWindow.fontSize
        }

        Frame {
            id: cddbSearchResultsFrame
            objectName: "cddbSearchResultsFrame"
            width: cddbSearchResultsView.width
            height: 100
            padding: 1
            anchors.top: trackWriteStatusLabel.bottom
            anchors.topMargin: mainWindow.topMargin
            anchors.left: trackWriteStatusLabel.left
            background: Rectangle {
                color: "transparent"
                border.color: "#9E9E9E"
                border.width: 1
                radius: 1
            }
            Flickable {
                id: importStatusView
                objectName: "importStatusView"
                anchors.fill: parent
                clip: true
                contentWidth: parent.width
                contentHeight: importStatus.contentHeight + 20
                Rectangle {
                    color: "#FFFFFF"
                    width: importStatusView.width
                    height: importStatusView.contentHeight
                    Text {
                        id: importStatus
                        objectName: "importStatus"
                        padding: 5
                        wrapMode: Text.WordWrap
                        width: cddbSearchResultsView.width - 10
                        text: "\n\n\n\n\n"
                        font.family: mainWindow.fontFamily
                        font.capitalization: mainWindow.fontCapitalization
                        font.pointSize: mainWindow.fontSize
                    }
                }
            }
        }
        Connections {
            target: QtCdParanoia
            onUpdateStatus: importStatus.text += "\n" + text
        }
    }
}
