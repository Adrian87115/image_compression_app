import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "resources/right_side"
import "resources/left_side"

Window{
    width: 600
    height: 600
    visible: true
    title: qsTr("Image Compression App")

    property alias leftS: leftS
    property alias rightS: rightS

    LeftSide{
        id: leftS
        anchors.left: parent.left
        width: parent.width / 10 * 9
        height: parent.height
    }

    RightSide{
        id: rightS
        width: parent.width / 10
        height: parent.height
        anchors.right: parent.right
    }
}
