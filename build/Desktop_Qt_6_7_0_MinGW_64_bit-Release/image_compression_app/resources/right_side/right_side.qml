import QtQuick 2.15

Rectangle{
    id: dropAreaContainer
    width: parent.width / 10 * 9
    height: parent.height
    anchors.left: parent.left
    color: "darkgray"
    border.color: "gray"
    border.width: 2

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
        width: parent.width / 10 * 9
        height: parent.height / 10 * 9
        color: "lightgray"
        border.color: "black"
        border.width: 2
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
        }
        onDropped:{
            console.log("File dropped")
            dropAreaBackground.color = "gray"
            if(drag.hasUrls){
                for(var i = 0; i < drag.urls.length; i++){
                    var url = drag.urls[i].toString()
                    console.log("Dropped URL: " + url)
                    dropAreaText.text = "Dropped file: " + url
                    filePath = url
                    break
                }
                console.log(filePath)
            }else{
                console.log("No URLs found in the drag event.")
            }
        }
    }
}
