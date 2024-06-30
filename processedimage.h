#ifndef PROCESSED_IMAGE_H
#define PROCESSED_IMAGE_H

#include <QObject>
#include <QImage>
#include <QWidget>
#include <QLabel>
#include <iostream>
#include <string>
#include <cmath>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

class ProcessedImage{
public:
    ProcessedImage(string img_name);
    ~ProcessedImage();

    void loadImage(string img_name);
    void displayImage();
    void displayOther(QImage* other_img);
    void saveConverted(QImage* img_name, QString path);

    // encoding

    vector<QImage> separateChannels(QImage img_to_separate);
    vector<vector<vector<int>>> channelsToValues(vector<QImage> channels);
    QImage RGBToYCbCr();
    vector<vector<vector<int>>> downsampling(QImage img_other);
    vector<vector<vector<vector<int>>>> divideInto8x8(vector<vector<vector<int>>> data);
    static vector<float> DCT(vector<int> data);
    static void quantization(vector<vector<vector<vector<float>>>>& data);
    vector<vector<vector<vector<float>>>> performDCT(vector<vector<vector<vector<int>>>> data);
    vector<vector<int>> zigzagScan(vector<vector<vector<vector<float>>>> data);
    vector<vector<int>> RLE(vector<vector<int>> data);

    // Huffman encoding
    struct HuffmanNode {
        int value;
        int frequency;
        HuffmanNode* left;
        HuffmanNode* right;
        HuffmanNode(int value, int frequency);
        ~HuffmanNode();
    };
    struct HuffmanTree {
        int size;
        int capacity;
        HuffmanNode** array;
        HuffmanTree(int capacity);
        ~HuffmanTree();
    };
    static void swapHuffmanNode(HuffmanNode** a, HuffmanNode** b);
    static void minHeapify(HuffmanTree* tree, int id);
    static bool isSizeOne(HuffmanTree* tree);
    static HuffmanNode* extractMin(HuffmanTree* tree);
    static void insertTree(HuffmanTree* tree, HuffmanNode* node);
    static void buildTree(HuffmanTree* tree);
    static bool isLeaf(HuffmanNode* root);
    static HuffmanTree* createAndBuildTree(vector<int> values, vector<int> freq);
    static HuffmanNode* buildHuffmanTree(vector<int> values, vector<int> freq);
    static void generateCodes(HuffmanNode* root, string code, map<int, string>& huffman_codes);
    static map<int, string> getHuffmanCodes(vector<int> values, vector<int> freq);
    vector<vector<string>> HuffmanEncoding(vector<vector<int>> rle_data);

    void writeToFileEncoded(vector<vector<string>> data);
    void writeToFileMap();
    void writeToFileDimensions();
    vector<vector<string>> encode();

    // decoding

    // Huffman decoding
    static void generateHuffmanCodeMap(HuffmanNode* root, string code, map<string, int>& huffman_codes);
    vector<vector<int>> HuffmanDecoding(vector<vector<string>> encoded_data);
    vector<vector<int>> HuffmanDecodingFromFile(vector<vector<string>> encoded_data, vector<map<string, int>> codes);

    vector<vector<int>> RLEDecode(vector<vector<int>> data);
    vector<vector<vector<vector<int>>>> backToBlocksFromZigzag(vector<vector<int>> data);
    vector<float> IDCT(vector<int> block);
    void dequantization(vector<vector<vector<vector<float>>>>& data);
    vector<vector<vector<vector<int>>>> reverseDCT(vector<vector<vector<vector<int>>>> data);
    vector<vector<vector<int>>> reverseBlocks(vector<vector<vector<vector<int>>>> data);
    QImage upscaling(vector<vector<vector<int>>> data);
    QImage YCbCrToRGB(QImage img_to_convert);
    vector<vector<string>> readFromFileEncodedPath(QString path);
    vector<map<string, int>> readFromFileMapPath(QString path);
    void readFromFileDimensionsPath(QString path);
    QImage* decodeWithFiles(QString path);

private:
    QImage* img = nullptr;
    string img_name;
    QLabel* label = nullptr;
    int width;
    int height;
    int num_blocks_x_luminance;
    int num_blocks_y_luminance;
    int num_blocks_x_chrominance;
    int num_blocks_y_chrominance;
    vector<HuffmanNode*> tree_channels = {nullptr, nullptr, nullptr};
};

#endif // PROCESSED_IMAGE_H
