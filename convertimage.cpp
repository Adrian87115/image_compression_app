#include "convertimage.h"

ConvertImage::ConvertImage(QObject *parent){}

QString ConvertImage::encodeImage(QString path){
    if(path == ""){
        return "Invalid path";
    }
    ProcessedImage image(path.toStdString());
    vector<vector<string>> encoded = image.encode();
    image.writeToFileEncoded(encoded);
    image.writeToFileMap();
    image.writeToFileDimensions();
    return "Image encoded";
}

QString ConvertImage::decodeFiles(QString path){
    if(path == ""){
        return "Invalid path";
    }
    ProcessedImage image("");
    QImage* decoded_image = image.decodeWithFiles(path);
    image.saveConverted(decoded_image, path);
    delete decoded_image;
    return "Image decoded";
}

QString ConvertImage::performConversion(bool mode, QString path) {
    if(path.isEmpty()){
        return "Error: Invalid path";
    }
    QString out;
    if(mode){
        if(path.right(3) == "jpg" || path.right(3) == "png"){
            out = encodeImage(path);
        }else{
            out = "Error: Invalid file type";
        }

    }else{
        if(path.right(12) == "_encoded.txt"){
            out = decodeFiles(path);
        }else{
            out = "Error: Invalid file name";
        }
    }
    return out;

    return "Error: Problem with the input file";
}
