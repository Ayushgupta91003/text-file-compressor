#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <bitset>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void buildCodeMap(Node* root, string str, unordered_map<char, string>& codeMap) {
    if (!root) return;
    if (!root->left && !root->right) {
        codeMap[root->ch] = str;
    }
    buildCodeMap(root->left, str + "0", codeMap);
    buildCodeMap(root->right, str + "1", codeMap);
}

int main() {
    ifstream inFile("sample.txt");
    if (!inFile) {
        cerr << "Error opening sample.txt" << endl;
        return 1;
    }

    // Step 1: Read file and calculate frequency
    unordered_map<char, int> freqMap;
    string text;
    char ch;
    while (inFile.get(ch)) {
        freqMap[ch]++;
        text += ch;
    }
    inFile.close();

    // Step 2: Build Huffman Tree
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;

        pq.push(merged);
    }

    Node* root = pq.top();

    // Step 3: Generate Huffman Codes
    unordered_map<char, string> codeMap;
    buildCodeMap(root, "", codeMap);

    // Step 4: Write code map to file
    ofstream mapFile("mapping.txt");
    for (auto& pair : codeMap) {
        mapFile << (int)pair.first << " " << pair.second << "\n";
    }
    mapFile.close();

    // Step 5: Encode the text
    string encodedString = "";
    for (char c : text) {
        encodedString += codeMap[c];
    }

    // Step 6: Write binary string to file
    ofstream outFile("compressed.bin", ios::binary);
    int extraBits = 8 - (encodedString.size() % 8);
    if (extraBits != 8) {
        encodedString.append(extraBits, '0');
    } else {
        extraBits = 0;
    }

    outFile.put(extraBits); // store padding info

    for (size_t i = 0; i < encodedString.size(); i += 8) {
        bitset<8> byte(encodedString.substr(i, 8));
        outFile.put((unsigned char)byte.to_ulong());
    }

    outFile.close();

    cout << "Compression Done Successfully!" << endl;

    return 0;
}
