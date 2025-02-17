import QtQuick
import QtQuick.Controls

Item{
    id: itm2

    Rectangle{
        id: taskArea
        height: parent.height
        width: parent.width
        color: "black"

        Image{
            id: playImg
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 10
            width: parent.width * 0.5
            fillMode: Image.PreserveAspectFit
            source: "qrc:/resources/img/play_white.png"

            MouseArea{
                id: playButtonArea
                anchors.fill: parent
                onClicked:{
                    console.log("Play button clicked")
                    console.log(ButtonSignalsHandling.mode)
                    console.log(GetFilePath.stored_path)
                    var result = ConvertImage.performConversion(ButtonSignalsHandling.mode, GetFilePath.stored_path).toString();
                    console.log(result)
                    leftS.statusText = result;
                }
                onPressed: playImg.opacity = 0.7
                onReleased: playImg.opacity = 1.0
                onCanceled: playImg.opacity = 1.0
            }
        }

        Image{
            id: txtImg
            anchors.bottom: playImg.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 15
            width: parent.width * 0.5
            fillMode: Image.PreserveAspectFit
            source: "qrc:/resources/img/txt_white.png" // to make it work also set(CMAKE_AUTORCC ON) had to be added to CMake

            MouseArea{
                id: txtImgArea
                anchors.fill: parent
                onClicked:{
                    ButtonSignalsHandling.setMode(false)
                    console.log("Txt button clicked")
                    console.log(ButtonSignalsHandling.mode) // access has to be direct, otherwise it will not work when getMode is used
                    txtImg.opacity = 0.5
                    imgIconImg.opacity = 1.0
                }
                onPressed: txtImg.opacity = 0.7
                onReleased: txtImg.opacity = 1.0
                onCanceled: txtImg.opacity = 1.0
            }
        }

        Image{
            id: imgIconImg
            anchors.bottom: txtImg.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 15
            width: parent.width * 0.5
            fillMode: Image.PreserveAspectFit
            source: "qrc:/resources/img/img_icon_white.png"
            opacity: 0.5

            MouseArea{
                id: imgIconArea
                anchors.fill: parent
                onClicked:{
                    ButtonSignalsHandling.setMode(true)
                    console.log("Img button clicked")
                    console.log(ButtonSignalsHandling.mode)
                    txtImg.opacity = 1.0
                    imgIconImg.opacity = 0.5
                }
                onPressed: imgIconImg.opacity = 0.7
                onReleased: imgIconImg.opacity = 1.0
                onCanceled: imgIconImg.opacity = 1.0
            }
        }
    }
}
