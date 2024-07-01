import QtQuick
import QtQuick.Controls


Item{
    id: itm1
    property alias statusText: dropAreaStatus.text
    Rectangle{
        id: dropAreaContainer
        anchors.fill: parent
        width: parent.width
        height: parent.height
        color: "darkgray"
        border.color: "gray"
        border.width: 2
        property string path: ""

        Text{
            id: dropAreaText
            font.pixelSize: 15
            text: "Drag and drop a file here"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 5
        }

        Rectangle{
            id: dropAreaBackground
            anchors.centerIn: parent
            width: parent.width * 0.9
            height: parent.height * 0.9
            color: "lightgray"
            border.color: "black"
            border.width: 2

            Text{
                id: dropAreaStatus
                font.pixelSize: 12
                text: ""
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 10
            }
        }

        DropArea{
            id: dropArea
            anchors.fill: parent
            onEntered:{
                console.log("Drag entered")
                dropAreaBackground.color = "darkgray"
            }
            onExited:{
                console.log("Drag exited")
                dropAreaBackground.color = "lightgray"
                dropAreaStatus.text = ""
                dropAreaContainer.path = ""
                GetFilePath.setPath("")
            }
            onDropped:{
                console.log("File dropped")
                dropAreaBackground.color = "gray"
                var path = drop.urls.toString().substring(8);
                dropAreaContainer.path = path
                dropAreaStatus.text = "Current file: " + dropAreaContainer.path
                console.log(dropAreaContainer.path)
                GetFilePath.setPath(dropAreaContainer.path)
            }
        }
    }
}

