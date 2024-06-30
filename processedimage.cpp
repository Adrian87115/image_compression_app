#include "processedimage.h"

ProcessedImage::ProcessedImage(string img_name){
    this->img_name = img_name;
    loadImage(img_name);
}

ProcessedImage::~ProcessedImage(){
    if(img){
        delete img;
    }
    if(label){
        delete label;
    }
    if(tree_channels[0]){
        for(int i = 0; i < 3; ++i){
            delete tree_channels[i];
        }
    }
}

void ProcessedImage::loadImage(string img_name){
    img = new QImage(img_name.c_str());
    if(!img || img->isNull()){
        cerr << "Error: Failed to load image" << endl;
        delete img;
        img = nullptr;
        return;
    }
    int target_width = (img->width() / 64) * 64;
    int target_height = (img->height() / 64) * 64;
    *img = img->scaled(target_width, target_height);
    width = img->width();
    height = img->height();
    num_blocks_y_luminance = height / 8;
    num_blocks_x_luminance = width / 8;
    num_blocks_y_chrominance = num_blocks_y_luminance / 2;
    num_blocks_x_chrominance = num_blocks_x_luminance / 2;
}

void ProcessedImage::displayImage(){
    if(!img){
        cerr << "Error: Can't display empty image" << endl;
        return;
    }
    label = new QLabel();
    label->setPixmap(QPixmap::fromImage(*img));
    label->setScaledContents(true);
    label->setWindowTitle("original");
    label->show();
}

void ProcessedImage::displayOther(QImage* other_img){
    if(!other_img){
        cerr << "Error: Can't display empty image" << endl;
        return;
    }
    QLabel* temp_label = new QLabel();
    temp_label->setAttribute(Qt::WA_DeleteOnClose);
    temp_label->setPixmap(QPixmap::fromImage(*other_img));
    temp_label->setScaledContents(true);
    temp_label->setWindowTitle("other");
    temp_label->show();
}

void ProcessedImage::saveConverted(QImage* img_name, QString path){
    if(!img_name){
        cerr << "Error: Can't save empty image" << endl;
        return;
    }
    path.chop(4);
    path += "_output.jpg";
    img_name->save(path);
}

vector<QImage> ProcessedImage::separateChannels(QImage img_to_separate){
    if(img_to_separate.isNull()){
        img_to_separate = *img;
    }
    vector<QImage> channels(3);

    for(int i = 0; i < 3; i++){
        channels[i] = QImage(img_to_separate.size(), img_to_separate.format());
    }

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            QRgb curr_pixel = img_to_separate.pixel(x, y);
            int r = qRed(curr_pixel);
            int g = qGreen(curr_pixel);
            int b = qBlue(curr_pixel);
            channels[0].setPixel(x, y, qRgb(r, 0, 0));
            channels[1].setPixel(x, y, qRgb(0, g, 0));
            channels[2].setPixel(x, y, qRgb(0, 0, b));
        }
    }
    return channels;
}

vector<vector<vector<int>>> ProcessedImage::channelsToValues(vector<QImage> channels){
    vector<vector<vector<int>>> channelsValues(3);

    for(int i = 0; i < 3; i++){
        QImage channel = channels[i];
        if(channel.isNull()){
            cerr << "Error: Channels not found" << endl;
            return {};
        }
        vector<vector<int>>& values = channelsValues[i];
        values.resize(height, vector<int>(width));
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                QRgb pixel = channel.pixel(x, y);
                int value;
                switch(i){
                case 0:
                    value = qRed(pixel);
                    break;
                case 1:
                    value = qGreen(pixel);
                    break;
                case 2:
                    value = qBlue(pixel);
                    break;
                default:
                    break;
                }
                values[y][x] = value;
            }
        }
    }
    return channelsValues;
}

QImage ProcessedImage::RGBToYCbCr(){
    vector<QImage> channels(3);
    channels = separateChannels(*img);
    vector<vector<vector<int>>> channels_values;
    channels_values = channelsToValues(channels);
    QImage converted = QImage(img->size(), img->format());

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            double R = channels_values[0][y][x] / 255.0;
            double G = channels_values[1][y][x] / 255.0;
            double B = channels_values[2][y][x] / 255.0;
            int Y = 16 + 65.738 * R + 129.057 * G + 25.064 * B;
            int Cb = 128 - 37.945 * R - 74.494 * G + 112.439 * B;
            int Cr = 128 + 112.439 * R - 94.154 * G - 18.285 * B;
            Y = max(0, min(255, Y));
            Cb = max(16, min(235, Cb));
            Cr = max(16, min(235, Cr));
            converted.setPixel(x, y, qRgb(Y, Cb, Cr));
        }
    }
    return converted;
}

vector<vector<vector<int>>> ProcessedImage::downsampling(QImage img_other){
    vector<QImage> channels(3);
    channels = separateChannels(img_other);
    vector<vector<vector<int>>> channels_values;
    channels_values = channelsToValues(channels);
    int halved_width = width / 2;
    int halved_height = height / 2;
    vector<vector<vector<int>>> downsampled(3);
    downsampled[0].resize(height, vector<int>(width)); // luminance
    downsampled[1].resize(halved_height, vector<int>(halved_width)); // blue chrominance
    downsampled[2].resize(halved_height, vector<int>(halved_width)); // red chrominance

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            downsampled[0][y][x] = channels_values[0][y][x];
        }
    }

    for(int y = 0; y < halved_height; y++){
        for(int x = 0; x < halved_width; x++){
            int average_Cb = 0, average_Cr = 0;
            for(int y2 = 0; y2 < 2; y2++){
                for(int x2 = 0; x2 < 2; x2++){
                    average_Cb += channels_values[1][y * 2 + y2][x * 2 + x2];
                    average_Cr += channels_values[2][y * 2 + y2][x * 2 + x2];
                }
            }
            average_Cb /= 4;
            average_Cr /= 4;
            downsampled[1][y][x] = average_Cb;
            downsampled[2][y][x] = average_Cr;
        }
    }
    return downsampled;
}

vector<vector<vector<vector<int>>>> ProcessedImage::divideInto8x8(vector<vector<vector<int>>> data){
    vector<vector<vector<vector<int>>>> blocks(3);
    blocks[0] = vector<vector<vector<int>>>(num_blocks_y_luminance, vector<vector<int>>(num_blocks_x_luminance, vector<int>(64)));
    blocks[1] = vector<vector<vector<int>>>(num_blocks_y_chrominance, vector<vector<int>>(num_blocks_x_chrominance, vector<int>(64)));
    blocks[2] = vector<vector<vector<int>>>(num_blocks_y_chrominance, vector<vector<int>>(num_blocks_x_chrominance, vector<int>(64)));

    for(int block_y = 0; block_y < num_blocks_y_luminance; block_y++){
        for(int block_x = 0; block_x < num_blocks_x_luminance; block_x++){
            for(int y = 0; y < 8; y++){
                for(int x = 0; x < 8; x++){
                    blocks[0][block_y][block_x][y * 8 + x] = data[0][block_y * 8 + y][block_x * 8 + x];
                }
            }
        }
    }

    for(int ch = 1; ch < 3; ch++){
        for(int block_y = 0; block_y < num_blocks_y_chrominance; block_y++){
            for(int block_x = 0; block_x < num_blocks_x_chrominance; block_x++){
                for(int y = 0; y < 8; y++){
                    for(int x = 0; x < 8; x++){
                        blocks[ch][block_y][block_x][y * 8 + x] = data[ch][block_y * 8 + y][block_x * 8 + x];
                    }
                }
            }
        }
    }
    return blocks;
}

vector<float> ProcessedImage::DCT(vector<int> data){
    float pi = M_PI;
    vector<float> transformed(64, 0.0f);
    float ci, cj, temp_sum, sum;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(!i){
                ci = 1 / sqrt(8);
            }else{
                ci = sqrt(2) / sqrt(8);
            }
            if(!j){
                cj = 1 / sqrt(8);
            }else{
                cj = sqrt(2) / sqrt(8);
            }
            sum = 0;
            for(int k = 0; k < 8; k++){
                for(int l = 0; l < 8; l++){
                    temp_sum = data[k * 8 + l] * cos((2 * k + 1) * i * pi / (2 * 8)) * cos((2 * l + 1) * j * pi / (2 * 8));
                    sum += temp_sum;
                }
            }
            transformed[i * 8 + j] = ci * cj * sum;
        }
    }
    return transformed;
}

void ProcessedImage::quantization(vector<vector<vector<vector<float>>>>& data){
    vector<vector<int>> chrominance_table = {{10, 8, 9, 9, 9, 8, 10, 9},
                                             {9, 9, 10, 10, 10, 11, 12, 17},
                                             {13, 12, 12, 12, 12, 20, 16, 16},
                                             {14, 17, 18, 20, 23, 23, 22, 20},
                                             {25, 25, 25, 25, 25, 25, 25, 25},
                                             {25, 25, 25, 25, 25, 25, 25, 25},
                                             {25, 25, 25, 25, 25, 25, 25, 25},
                                             {25, 25, 25, 25, 25, 25, 25, 25}};
    vector<vector<int>> luminance_table = {{6, 4, 4, 6, 10, 16, 20, 24},
                                           {5, 5, 6, 8, 10, 23, 24, 22},
                                           {6, 5, 6, 10, 16, 23, 28, 22},
                                           {6, 7, 9, 12, 20, 35, 32, 25},
                                           {7, 9, 15, 22, 27, 44, 41, 31},
                                           {10, 14, 22, 26, 32, 42, 45, 37},
                                           {20, 26, 31, 35, 41, 48, 48, 40},
                                           {29, 37, 38, 39, 45, 40, 41, 40}};

    for(int ch = 0; ch < 3; ch++){
        for(int block_y = 0; block_y < data[ch].size(); block_y++){
            for(int block_x = 0; block_x < data[ch][block_y].size(); block_x++){
                for(int i = 0; i < 64; ++i){
                    int y = i / 8;
                    int x = i % 8;
                    if(!ch){
                        data[ch][block_y][block_x][i] = round(data[ch][block_y][block_x][i] / luminance_table[y][x]);
                    }else{
                        data[ch][block_y][block_x][i] = round(data[ch][block_y][block_x][i] / chrominance_table[y][x]);
                    }
                }
            }
        }
    }
}

vector<vector<vector<vector<float>>>> ProcessedImage::performDCT(vector<vector<vector<vector<int>>>> data){
    vector<vector<vector<vector<float>>>> DCT_result;

    for(const auto& channel : data){
        vector<vector<vector<float>>> channel_DCT;
        for(const auto& blocks_row : channel){
            vector<vector<float>> row_DCT;
            for(const auto& block : blocks_row){
                vector<float> block_DCT = DCT(block);
                row_DCT.push_back(block_DCT);
            }
            channel_DCT.push_back(row_DCT);
        }
        DCT_result.push_back(channel_DCT);
    }
    quantization(DCT_result);
    return DCT_result;
}

vector<vector<int>> ProcessedImage::zigzagScan(vector<vector<vector<vector<float>>>> data){
    vector<vector<int>> channels_wrap(3);

    for(int ch = 0; ch < 3; ch++){
        for(const auto& blocks_row : data[ch]){
            for(const auto& block : blocks_row){
                int row = 0, col = 0;
                bool up = true;
                vector<int> zigzag;
                while(row < 8 && col < 8){
                    zigzag.push_back(static_cast<int>(block[row * 8 + col]));
                    if(up){
                        if(col == 7){
                            ++row;
                            up = false;
                        }else if(row == 0){
                            ++col;
                            up = false;
                        }else{
                            --row;
                            ++col;
                        }
                    }else{
                        if(row == 7){
                            ++col;
                            up = true;
                        }else if(col == 0){
                            ++row;
                            up = true;
                        }else{
                            ++row;
                            --col;
                        }
                    }
                }
                channels_wrap[ch].insert(channels_wrap[ch].end(), zigzag.begin(), zigzag.end());
            }
        }
    }
    return channels_wrap;
}

vector<vector<int>> ProcessedImage::RLE(vector<vector<int>> data){
    vector<vector<int>> compressed(3);

    for(int ch = 0; ch < 3; ch++){
        int current = data[ch][0];
        int count = 1;
        for(int i = 1; i < data[ch].size(); i++){
            if(data[ch][i] == current){
                count++;
            }else{
                compressed[ch].push_back(count);
                compressed[ch].push_back(current);
                current = data[ch][i];
                count = 1;
            }
        }
        compressed[ch].push_back(count);
        compressed[ch].push_back(current);
    }
    return compressed;
}

ProcessedImage::HuffmanNode::HuffmanNode(int value, int frequency) : value(value), frequency(frequency), left(nullptr), right(nullptr) {}

ProcessedImage::HuffmanNode::~HuffmanNode(){
    delete left;
    delete right;
}

ProcessedImage::HuffmanTree::HuffmanTree(int capacity) : size(0), capacity(capacity), array(new HuffmanNode*[capacity]) {}

ProcessedImage::HuffmanTree::~HuffmanTree(){
    for(int i = 0; i < size; i++){
        delete array[i];
    }
    delete[] array;
}

void ProcessedImage::swapHuffmanNode(HuffmanNode** a, HuffmanNode** b){
    HuffmanNode* temp = *a;
    *a = *b;
    *b = temp;
}

void ProcessedImage::minHeapify(HuffmanTree* tree, int id){
    int min_id = id;
    int left = 2 * id + 1;
    int right = 2 * id + 2;
    if(left < tree->size && tree->array[left]->frequency < tree->array[min_id]->frequency){
        min_id = left;
    }
    if(right < tree->size && tree->array[right]->frequency < tree->array[min_id]->frequency){
        min_id = right;
    }
    if(min_id != id){
        swapHuffmanNode(&tree->array[min_id], &tree->array[id]);
        minHeapify(tree, min_id);
    }
}

bool ProcessedImage::isSizeOne(HuffmanTree* tree){
    return tree->size == 1;
}

ProcessedImage::HuffmanNode* ProcessedImage::extractMin(HuffmanTree* tree){
    HuffmanNode* temp = tree->array[0];
    tree->array[0] = tree->array[tree->size - 1];
    --tree->size;
    minHeapify(tree, 0);
    return temp;
}

void ProcessedImage::insertTree(HuffmanTree* tree, HuffmanNode* node){
    ++tree->size;
    int i = tree->size - 1;
    while(i && node->frequency < tree->array[(i - 1) / 2]->frequency){
        tree->array[i] = tree->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    tree->array[i] = node;
}

void ProcessedImage::buildTree(HuffmanTree* tree){
    int n = tree->size - 1;
    for(int i = (n - 1) / 2; i >= 0; i--){
        minHeapify(tree, i);
    }
}

bool ProcessedImage::isLeaf(HuffmanNode* root){
    return !(root->left) && !(root->right);
}

ProcessedImage::HuffmanTree* ProcessedImage::createAndBuildTree(vector<int> values, vector<int> freq){
    int size = values.size();
    HuffmanTree* tree = new HuffmanTree(size);

    for(int i = 0; i < size; i++){
        tree->array[i] = new HuffmanNode(values[i], freq[i]);
    }
    tree->size = size;
    buildTree(tree);
    return tree;
}

ProcessedImage::HuffmanNode* ProcessedImage::buildHuffmanTree(vector<int> values, vector<int> freq){
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode* top;
    HuffmanTree* tree = createAndBuildTree(values, freq);

    while(!isSizeOne(tree)){
        left = extractMin(tree);
        right = extractMin(tree);
        top = new HuffmanNode(-1, left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        insertTree(tree, top);
    }
    HuffmanNode* root = extractMin(tree);
    delete tree;
    return root;
}

void ProcessedImage::generateCodes(HuffmanNode* root, string code, map<int, string>& huffman_codes){
    if(!root){
        return;
    }
    if(isLeaf(root)){
        huffman_codes[root->value] = code;
    }
    generateCodes(root->left, code + "0", huffman_codes);
    generateCodes(root->right, code + "1", huffman_codes);
}

map<int, string> ProcessedImage::getHuffmanCodes(vector<int> values, vector<int> freq){
    HuffmanNode* root = buildHuffmanTree(values, freq);
    map<int, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);
    return huffmanCodes;
}

vector<vector<string>> ProcessedImage::HuffmanEncoding(vector<vector<int>> rle_data){
    vector<vector<string>> encoded_channels(rle_data.size());
    for(int ch = 0; ch < rle_data.size(); ch++){
        map<int, int> freq_map;
        for(int val : rle_data[ch]){
            freq_map[val]++;
        }
        vector<int> values, frequencies;
        for(const auto& pair : freq_map){
            values.push_back(pair.first);
            frequencies.push_back(pair.second);
        }
        HuffmanNode* root = buildHuffmanTree(values, frequencies);
        tree_channels[ch] = root;
        map<int, string> huffmanCodes = getHuffmanCodes(values, frequencies);

        // for(const auto& pair : huffmanCodes){
        //     cout << "Value: " << pair.first << " Code: " << pair.second << endl;
        // }

        for(int val : rle_data[ch]){
            encoded_channels[ch].push_back(huffmanCodes[val]);
        }
    }
    return encoded_channels;
}

void ProcessedImage::writeToFileEncoded(vector<vector<string>> data){
    ofstream outfile(img_name + ".txt");
    if(!outfile.is_open()){
        cerr << "Error: Unable to open the file" << endl;
        return;
    }

    for(const auto& channel : data){
        for(const string& code : channel){
            outfile << code << " ";
        }
        outfile << endl;
    }
    outfile.close();
}

void ProcessedImage::writeToFileMap(){
    vector<map<string, int>> codes;

    for(int ch = 0; ch < 3; ch++){
        map<string, int> tab;
        generateHuffmanCodeMap(tree_channels[ch], "", tab);
        codes.push_back(tab);
    }
    ofstream outfile(img_name + "_codes.txt");
    if(!outfile.is_open()){
        cerr << "Error: Unable to open the file" << endl;
        return;
    }

    for(const auto& channel : codes){
        for(const auto& pair : channel){
            outfile << pair.first << " " << pair.second << endl;
        }
        outfile << endl;
    }
    outfile.close();
}

vector<vector<string>> ProcessedImage::encode(){
    QImage ycbcr_img = RGBToYCbCr();
    // cout << "YCbCr converted\n";
    vector<vector<vector<int>>> downsampled_data = downsampling(ycbcr_img);
    // cout << "Data downsampled\n";
    vector<vector<vector<vector<int>>>> blocks = divideInto8x8(downsampled_data);
    // cout << "Spplitted into blocks\n";
    vector<vector<vector<vector<float>>>> after_DCT = performDCT(blocks);
    // cout << "DCT performed\n";
    vector<vector<int>> zigzaged = zigzagScan(after_DCT);
    // cout << "Zigzag performed\n";
    vector<vector<int>> after_RLE = RLE(zigzaged);
    // cout << "RLE converted\n";
    vector<vector<string>> huffmaned = HuffmanEncoding(after_RLE);
    // cout << "Successfully encoded\n";
    return huffmaned;
}

void ProcessedImage::generateHuffmanCodeMap(HuffmanNode* root, string code, map<string, int>& huffman_codes){
    if(!root){
        return;
    }
    if(isLeaf(root)){
        huffman_codes[code] = root->value;

    }
    generateHuffmanCodeMap(root->left, code + "0", huffman_codes);
    generateHuffmanCodeMap(root->right, code + "1", huffman_codes);
}

vector<vector<int>> ProcessedImage::HuffmanDecoding(vector<vector<string>> encoded_data){
    vector<vector<int>> decoded_channels(3);

    for(int ch = 0; ch < 3; ch++){
        // printHuffmanTree(tree_channels[ch], "");
        map<string, int> codes;
        generateHuffmanCodeMap(tree_channels[ch], "", codes);
        // cout << "Channel " << ch << " Huffman Codes:" << endl;
        // for (const auto& pair : tab) {
        //     cout << "Code: " << pair.first << " Value: " << pair.second << endl;
        // }
        for(const string& encoded_str : encoded_data[ch]){
            if(codes.find(encoded_str) != codes.end()){
                decoded_channels[ch].push_back(codes[encoded_str]);
            }else{
                if(encoded_str != " " && encoded_str != "\n" && encoded_str != "\0"){
                    cerr << "Error: Huffman code not found for " << encoded_str << endl;
                }
            }
        }
    }
    return decoded_channels;
}

vector<vector<int>> ProcessedImage::HuffmanDecodingFromFile(vector<vector<string>> encoded_data, vector<map<string, int>> codes){
    vector<vector<int>> decoded_channels(3);

    for(int ch = 0; ch < 3; ch++){
        vector<int> decoded_channel;
        const auto& encoded_channel = encoded_data[ch];
        const auto& code_map = codes[ch];
        for(const string& code : encoded_channel){
            if(code == " " || code == "\n" || code == "\0"){
                continue;
            }
            auto val = code_map.find(code);
            if (val != code_map.end()) {
                decoded_channel.push_back(val->second);
            }else{
                cerr << "Error: Huffman code not found for " << code << endl;
            }
        }
        decoded_channels[ch] = decoded_channel;
    }
    return decoded_channels;
}

vector<vector<int>> ProcessedImage::RLEDecode(vector<vector<int>> data){
    vector<vector<int>> decoded(data.size());

    for(int ch = 0; ch < data.size(); ch++){
        for(int i = 0; i < data[ch].size(); i = i + 2){
            int count = data[ch][i];
            int val = data[ch][i + 1];
            for(int n = 0; n < count; n++){
                decoded[ch].push_back(val);
            }
        }
    }
    return decoded;
}

vector<vector<vector<vector<int>>>> ProcessedImage::backToBlocksFromZigzag(vector<vector<int>> data){
    vector<vector<vector<vector<int>>>> blocks(3);

    for(int ch = 0; ch < 3; ch++){
        int num_blocks_x = (ch == 0) ? num_blocks_x_luminance : num_blocks_x_chrominance;
        int num_blocks_y = (ch == 0) ? num_blocks_y_luminance : num_blocks_y_chrominance;
        blocks[ch].resize(num_blocks_y, vector<vector<int>>(num_blocks_x, vector<int>(8 * 8, 0.0)));
        for(int block_idx = 0; block_idx < num_blocks_x * num_blocks_y; block_idx++){
            int block_row = block_idx / num_blocks_x;
            int block_col = block_idx % num_blocks_x;
            int row = 0, col = 0;
            bool up = true;
            int zigzag_idx = 0;
            while(row < 8 && col < 8){
                blocks[ch][block_row][block_col][row * 8 + col] = static_cast<int>(data[ch][block_idx * 64 + zigzag_idx]);
                zigzag_idx++;
                if(up){
                    if(col == 7){
                        ++row;
                        up = false;
                    }else if(row == 0){
                        ++col;
                        up = false;
                    }else{
                        --row;
                        ++col;
                    }
                }else{
                    if(row == 7){
                        ++col;
                        up = true;
                    }else if(col == 0){
                        ++row;
                        up = true;
                    }else{
                        ++row;
                        --col;
                    }
                }
            }
        }
    }
    return blocks;
}

vector<float> ProcessedImage::IDCT(vector<int> block){
    vector<float> idct_block(64);
    float c[8];

    for(int i = 0; i < 8; i++){
        c[i] = (i == 0) ? (1.0 / sqrt(2.0)) : 1.0;
    }

    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            float sum = 0.0;
            for(int v = 0; v < 8; v++){
                for(int u = 0; u < 8; u++){
                    sum += c[u] * c[v] * block[v * 8 + u] * cos((2 * x + 1) * u * M_PI / 16) * cos((2 * y + 1) * v * M_PI / 16);
                }
            }
            idct_block[y * 8 + x] = sum / 4.0;
        }
    }
    return idct_block;
}

void ProcessedImage::dequantization(vector<vector<vector<vector<float>>>>& data){
    vector<vector<int>> chrominance_table = {{10, 8, 9, 9, 9, 8, 10, 9},
                                             {9, 9, 10, 10, 10, 11, 12, 17},
                                             {13, 12, 12, 12, 12, 20, 16, 16},
                                             {14, 17, 18, 20, 23, 23, 22, 20},
                                             {25, 25, 25, 25, 25, 25, 25, 25},
                                             {25, 25, 25, 25, 25, 25, 25, 25},
                                             {25, 25, 25, 25, 25, 25, 25, 25},
                                             {25, 25, 25, 25, 25, 25, 25, 25}};
    vector<vector<int>> luminance_table = {{6, 4, 4, 6, 10, 16, 20, 24},
                                           {5, 5, 6, 8, 10, 23, 24, 22},
                                           {6, 5, 6, 10, 16, 23, 28, 22},
                                           {6, 7, 9, 12, 20, 35, 32, 25},
                                           {7, 9, 15, 22, 27, 44, 41, 31},
                                           {10, 14, 22, 26, 32, 42, 45, 37},
                                           {20, 26, 31, 35, 41, 48, 48, 40},
                                           {29, 37, 38, 39, 45, 40, 41, 40}};

    for(int ch = 0; ch < 3; ch++){
        for(int block_y = 0; block_y < data[ch].size(); block_y++){
            for(int block_x = 0; block_x < data[ch][block_y].size(); block_x++){
                for(int i = 0; i < 64; ++i){
                    int y = i / 8;
                    int x = i % 8;
                    if(!ch){
                        data[ch][block_y][block_x][i] *= luminance_table[y][x];
                    }else{
                        data[ch][block_y][block_x][i] *= chrominance_table[y][x];
                    }
                }
            }
        }
    }
}

vector<vector<vector<vector<int>>>> ProcessedImage::reverseDCT(vector<vector<vector<vector<int>>>> data){
    vector<vector<vector<vector<float>>>> dequantized(3);

    for(int ch = 0; ch < 3; ch++){
        dequantized[ch].resize(data[ch].size());
        for(int block_y = 0; block_y < data[ch].size(); block_y++){
            dequantized[ch][block_y].resize(data[ch][block_y].size());
            for(int block_x = 0; block_x < data[ch][block_y].size(); block_x++){
                vector<float> block(data[ch][block_y][block_x].begin(), data[ch][block_y][block_x].end());
                dequantized[ch][block_y][block_x] = block;
            }
        }
    }
    dequantization(dequantized);
    vector<vector<vector<vector<int>>>> reversed(data.size());

    for(int ch = 0; ch < 3; ch++){
        reversed[ch].resize(dequantized[ch].size());
        for(int block_y = 0; block_y < dequantized[ch].size(); block_y++){
            reversed[ch][block_y].resize(dequantized[ch][block_y].size());
            for(int block_x = 0; block_x < dequantized[ch][block_y].size(); block_x++){
                reversed[ch][block_y][block_x].resize(64);
                vector<int> block_ints(64);
                vector<float> idct_block = IDCT(vector<int>(dequantized[ch][block_y][block_x].begin(), dequantized[ch][block_y][block_x].end()));
                for(int i = 0; i < 64; ++i){
                    block_ints[i] = round(idct_block[i]);
                }
                reversed[ch][block_y][block_x] = block_ints;
            }
        }
    }
    return reversed;
}

vector<vector<vector<int>>> ProcessedImage::reverseBlocks(vector<vector<vector<vector<int>>>> blocks){
    vector<vector<vector<int>>> data(3);
    data[0] = vector<vector<int>>(num_blocks_y_luminance * 8, vector<int>(num_blocks_x_luminance * 8));
    data[1] = vector<vector<int>>(num_blocks_y_chrominance * 8, vector<int>(num_blocks_x_chrominance * 8));
    data[2] = vector<vector<int>>(num_blocks_y_chrominance * 8, vector<int>(num_blocks_x_chrominance * 8));

    for(int block_y = 0; block_y < num_blocks_y_luminance; block_y++){
        for(int block_x = 0; block_x < num_blocks_x_luminance; block_x++){
            for(int y = 0; y < 8; y++){
                for(int x = 0; x < 8; x++){
                    data[0][block_y * 8 + y][block_x * 8 + x] = blocks[0][block_y][block_x][y * 8 + x];
                }
            }
        }
    }

    for(int ch = 1; ch < 3; ch++){
        for(int block_y = 0; block_y < num_blocks_y_chrominance; block_y++){
            for(int block_x = 0; block_x < num_blocks_x_chrominance; block_x++){
                for(int y = 0; y < 8; y++){
                    for(int x = 0; x < 8; x++){
                        data[ch][block_y * 8 + y][block_x * 8 + x] = blocks[ch][block_y][block_x][y * 8 + x];
                    }
                }
            }
        }
    }
    return data;
}

QImage ProcessedImage::upscaling(vector<vector<vector<int>>> data){
    int halved_width = width / 2;
    int halved_height = height / 2;
    vector<QImage> upscaled_channels(3);
    upscaled_channels[0] = QImage(width, height, QImage::Format_RGB32);
    upscaled_channels[1] = QImage(width, height, QImage::Format_RGB32);
    upscaled_channels[2] = QImage(width, height, QImage::Format_RGB32);
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int luminance = data[0][y][x];
            upscaled_channels[0].setPixel(x, y, qRgb(luminance, luminance, luminance));
        }
    }

    for(int y = 0; y < halved_height; y++){
        for(int x = 0; x < halved_width; x++){
            int Cb_value = data[1][y][x];
            int Cr_value = data[2][y][x];
            for(int dy = 0; dy < 2; dy++){
                for(int dx = 0; dx < 2; dx++){
                    int new_x = x * 2 + dx;
                    int new_y = y * 2 + dy;
                    upscaled_channels[1].setPixel(new_x, new_y, qRgb(Cb_value, Cb_value, Cb_value));
                    upscaled_channels[2].setPixel(new_x, new_y, qRgb(Cr_value, Cr_value, Cr_value));
                }
            }
        }
    }

    QImage merged_channels = QImage(width, height, QImage::Format_RGB32);

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int Y = qRed(upscaled_channels[0].pixel(x, y));
            int Cb = qRed(upscaled_channels[1].pixel(x, y));
            int Cr = qRed(upscaled_channels[2].pixel(x, y));
            merged_channels.setPixel(x, y, qRgb(Y, Cb, Cr));
        }
    }
    return merged_channels;
}

QImage ProcessedImage::YCbCrToRGB(QImage img_to_convert){
    vector<QImage> channels = separateChannels(img_to_convert);
    vector<vector<vector<int>>> channels_values = channelsToValues(channels);
    QImage converted = QImage(img_to_convert.size(), img_to_convert.format());

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            double Y = channels_values[0][y][x];
            double Cb = channels_values[1][y][x] - 128;
            double Cr = channels_values[2][y][x] - 128;
            int R = (int)(Y + 1.402 * Cr);
            int G = (int)(Y - 0.344136 * Cb - 0.714136 * Cr);
            int B = (int)(Y + 1.772 * Cb);
            R = max(0, min(255, R));
            G = max(0, min(255, G));
            B = max(0, min(255, B));
            converted.setPixel(x, y, qRgb(R, G, B));
        }
    }
    return converted;
}

vector<vector<string>> ProcessedImage::readFromFileEncodedPath(QString path){
    vector<vector<string>> encodedData;
    ifstream infile(path.toStdString());
    if(!infile.is_open()){
        cerr << "Error: Unable to open the file" << endl;
        return encodedData;
    }
    cout << "opened1";
    string line;

    while(getline(infile, line)){
        vector<string> row;
        int pos = 0;
        string token;
        while((pos = line.find(" ")) != string::npos){
            token = line.substr(0, pos);
            row.push_back(token);
            line.erase(0, pos + 1);
        }
        row.push_back(line);
        encodedData.push_back(row);
    }
    infile.close();
    return encodedData;
}

vector<map<string, int>> ProcessedImage::readFromFileMapPath(QString path){
    path.chop(4);
    vector<map<string, int>> codes;
    ifstream infile(path.toStdString() + "_codes.txt");
    if(!infile.is_open()){
        cerr << "Error: Unable to open the file" << endl;
        return codes;
    }
    cout << "opened2";
    string line;
    map<string, int> channel_codes;

    while(getline(infile, line)){
        if(line.empty()){
            if(!channel_codes.empty()){
                codes.push_back(channel_codes);
                channel_codes.clear();
            }
            continue;
        }
        stringstream ss(line);
        string code;
        int value;
        if(!(ss >> code >> value)){
            cerr << "Error: Invalid format in the file" << endl;
            return codes;
        }
        channel_codes[code] = value;
    }
    if(!channel_codes.empty()){
        codes.push_back(channel_codes);
    }
    infile.close();
    return codes;
}

QImage* ProcessedImage::decodeWithFiles(){

}
