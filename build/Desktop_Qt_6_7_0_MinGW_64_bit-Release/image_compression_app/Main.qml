import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "resources/right_side"
import "resources/left_side"

Window {
    width: 600
    height: 600
    visible: true
    title: qsTr("Image Compression App")

    LeftSide{
        id: left_s
        anchors.left: parent.left
        width: parent.width / 10 * 9
        height: parent.height
    }

    RightSide{
        id: right_s
        width: parent.width / 10
        height: parent.height
        anchors.right: parent.right
    }


}

// https://www.flaticon.com/
// https://www.flaticon.com/free-icons/picture
// https://www.flaticon.com/free-icons/txt
// https://www.flaticon.com/free-icons/play-button
