ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Drag and Drop Example")

    Rectangle {
        id: dropArea
        width: 400
        height: 300
        anchors.centerIn: parent
        color: "lightgray"
        border.color: "gray"
        border.width: 2

        Text {
            id: dropText
            text: "Drag and drop a file here"
            anchors.centerIn: parent
        }

        // Enable drag-and-drop
        Drag.active: dropArea.containsDrag
        Drag.hotSpot.x: drag.width / 2
        Drag.hotSpot.y: drag.height / 2

        // Handle drag and drop events
        DragEnterEvent: {
            accepted = true
            dropArea.color = "lightblue"
        }

        DragMoveEvent: {
            accepted = true
        }

        DragLeaveEvent: {
            dropArea.color = "lightgray"
        }

        DropEvent: {
            dropArea.color = "lightgray"
            var mimeData = drag.mimeData
            if (mimeData.hasUrls) {
                var url = mimeData.urls[0].toString()
                dropText.text = "Dropped file: " + url
                filePath = url
            }
        }
    }

    // Variable to store the file path
    property string filePath: ""
}
