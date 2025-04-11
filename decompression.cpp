#include <iostream>
#include <fstream>
#include <unordered_map>
#include <bitset>

using namespace std;

int main() {
    unordered_map<string, char> reverseCodeMap;
    ifstream mapFile("mapping.txt");
    if (!mapFile) {
        cerr << "Error opening mapping.txt" << endl;
        return 1;
    }

    int ascii;
    string code;
    while (mapFile >> ascii >> code) {
        reverseCodeMap[code] = (char)ascii;
    }
    mapFile.close();

    ifstream inFile("compressed.bin", ios::binary);
    if (!inFile) {
        cerr << "Error opening compressed.bin" << endl;
        return 1;
    }

    unsigned char paddingByte;
    inFile.read((char*)&paddingByte, 1);  

    string bitString = "";
    unsigned char byte;
    while (inFile.read((char*)&byte, 1)) {
        bitset<8> bits(byte);
        bitString += bits.to_string();
    }
    inFile.close();

    if (paddingByte > 0 && paddingByte <= 7) {
        bitString = bitString.substr(0, bitString.size() - paddingByte);
    }

    ofstream outFile("decompressed.txt");
    string current = "";
    for (char bit : bitString) {
        current += bit;
        if (reverseCodeMap.count(current)) {
            outFile.put(reverseCodeMap[current]);
            current = "";
        }
    }
    outFile.close();

    cout << "Decompression Done Successfully!" << endl;
    return 0;
}
