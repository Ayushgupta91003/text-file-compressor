#include <iostream>
#include <fstream>

using namespace std;

long getFileSize(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);
    return file.tellg(); 
}

int main() {
    long originalSize = getFileSize("sample.txt");
    long compressedSize = getFileSize("compressed.bin") + getFileSize("mapping.txt");

    double compressionRate = ((originalSize - compressedSize) * 100.0) / originalSize;

    ofstream output("compression_stats.txt");
    if (!output) {
        cerr << "Error: Could not create output file!" << endl;
        return 1;
    }

    output << "Original Size (bytes): " << originalSize << endl;
    output << "Compressed Size (bytes): " << compressedSize << endl;
    output << "Compression Rate: " << compressionRate << " %" << endl;

    output.close();

    cout << "Compression stats written to compression_stats.txt" << endl;

    return 0;
}
