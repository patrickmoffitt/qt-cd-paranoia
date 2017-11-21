import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: mainWindow
    objectName: "mainWindow"
    visible: true
    width: 500
    height: 700
    title: qsTr("QT CD Paranoia")
    color: "#B0BEC5"

    property int topMargin: 5
    property int horizontalMargin: 25
    property int textFieldHeight: 25
    property int buttonRadius: 8
    property int buttonHeight: 40
    property int fontSize: 12
    property string fontFamily: "Noto Mono"
    property int fontCapitalization: Font.MixedCase

    header: ToolBar {
        height: 25
        background: Rectangle {
            gradient: Gradient {
                GradientStop { position: 0 ; color: "#EEEEEE"}
                GradientStop { position: 1 ; color: "#B0BEC5" }
            }
        }

        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                height: parent.height
                anchors.right: parent.right
                contentItem: Image {
                    scale: 0.7
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "images/menu.png"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "About"
                        font.family: fontFamily
                        font.pixelSize: fontSize
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    Dialog {
        id: aboutDialog
        modal: true
        focus: true
        font.family: fontFamily
        font.pixelSize: fontSize
        background: Rectangle { color: "#EEEEEE" }
        title: "About"
        x: (parent.width - width) / 2
        y: parent.height / 12
        width: Math.min(parent.width, parent.height) / 5 * 4
        contentHeight: aboutColumn.height

        Column {
            id: aboutColumn
            spacing: 20

            Label {
                width: aboutDialog.availableWidth
                text: "Qt CD Paranoia imports music from the CD-ROM drive to the local storage."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "Copyright (C) 2017\n"
                      + "Patrick Moffitt\n"
                      + "\n"
                      + "Copyright (C) 2011\n"
                      + "Rocky Bernstein <rocky@gnu.org>\n"
                      + "\n"
                      + "CD-Paranoia was originally developed by Xiph.org\n"
                      + "https://en.wikipedia.org/wiki/Cdparanoia"
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }

            Label {
                width: aboutDialog.availableWidth
                text: "This program is free software: you can redistribute it and/or modify "
                      +"it under the terms of the GNU General Public License as published by "
                      +"the Free Software Foundation, either version 3 of the License, or "
                      +"(at your option) any later version.\n"
                      +"\n"
                      +"This program is distributed in the hope that it will be useful, "
                      +"but WITHOUT ANY WARRANTY; without even the implied warranty of "
                      +"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                      +"GNU General Public License for more details.\n"
                      +"\n"
                      +"You should have received a copy of the GNU General Public License "
                      +"along with this program.  If not, see <http://www.gnu.org/licenses/>."
                wrapMode: Label.Wrap
                font.pixelSize: 12
            }
        }
    }

    StackView {
        id: stack
        anchors.fill: parent
        initialItem: Qt.resolvedUrl("Page1.qml")
    }
}
