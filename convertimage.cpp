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
    return "Image encoded";
}

QString ConvertImage::decodeFiles(QString path){
    if(path == ""){
        return "Invalid path";
    }
    ProcessedImage image("C:/Users/adria/Desktop/sunset.png");
    vector<vector<string>> readed_data = image.readFromFileEncodedPath(path);
    vector<map<string, int>> codes = image.readFromFileMapPath(path);
    vector<vector<int>> decoded_huffman = image.HuffmanDecodingFromFile(readed_data, codes);
    vector<vector<int>> rle_decoded = image.RLEDecode(decoded_huffman);
    vector<vector<vector<vector<int>>>> zigzag_reversed = image.backToBlocksFromZigzag(rle_decoded);
    vector<vector<vector<vector<int>>>> dct_reversed = image.reverseDCT(zigzag_reversed);
    vector<vector<vector<int>>> blocks_reversed = image.reverseBlocks(dct_reversed);
    QImage upscaled = image.upscaling(blocks_reversed);
    QImage color_converted = image.YCbCrToRGB(upscaled);
    image.saveConverted(&color_converted, path);

    return "Image decoded";
}

QString ConvertImage::performConversion(bool mode, QString path){
    if(path == ""){
        return "Invalid path";
    }
    QString out;
    if(mode){
        out = encodeImage(path);
    }else{
        out = decodeFiles(path);
    }
    return out;
}
