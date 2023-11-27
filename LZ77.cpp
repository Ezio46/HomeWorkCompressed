#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct DictionaryEntry {
    int position;
    int length;
    char nextChar;

    DictionaryEntry(int pos, int len, char next)
        : position(pos), length(len), nextChar(next) {}
};

void compressLZ77(const string& inputFilename, const string& outputFilename, int windowSize, int bufferSize) {
    ifstream inputFile(inputFilename, ios::binary);
    ofstream compressedFile(outputFilename, ios::binary);

    if (!inputFile.is_open() || !compressedFile.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    map<string, int> dictionary;
    vector<char> buffer;

    int currentPos = 0;

    cout << "Original Text: ";
    while (!inputFile.eof()) {
        char nextChar = inputFile.get();
        buffer.push_back(nextChar);

        if (buffer.size() > bufferSize) {
            buffer.erase(buffer.begin());
        }

        cout << nextChar;

        string currentStr(buffer.begin(), buffer.end());

        if (dictionary.find(currentStr) != dictionary.end()) {
            currentPos = dictionary[currentStr];
        } else {
            compressedFile.write(reinterpret_cast<const char*>(&currentPos), sizeof(currentPos));
            compressedFile.write(reinterpret_cast<const char*>(&bufferSize), sizeof(bufferSize));
            compressedFile.write(&nextChar, sizeof(nextChar));

            dictionary[currentStr] = currentPos;
            currentPos = 0;
            buffer.clear();
        }
    }

    cout << endl;

    inputFile.close();
    compressedFile.close();
}

void decompressLZ77(const string& compressedFilename, const string& decompressedFilename) {
    ifstream compressedFile(compressedFilename, ios::binary);
    ofstream decompressedFile(decompressedFilename, ios::binary);

    if (!compressedFile.is_open() || !decompressedFile.is_open()) {
        cerr << "Error opening files." << endl;
        return;
    }

    int currentPos;
    int bufferSize;
    char nextChar;

    vector<char> buffer;

    cout << "Decompressed Text: ";

    while (compressedFile.read(reinterpret_cast<char*>(&currentPos), sizeof(currentPos))) {
        compressedFile.read(reinterpret_cast<char*>(&bufferSize), sizeof(bufferSize));
        compressedFile.read(&nextChar, sizeof(nextChar));

        if (currentPos > 0) {
            int startPos = buffer.size() - currentPos;
            for (int i = 0; i < bufferSize; ++i) {
                buffer.push_back(buffer[startPos + i]);
            }
        }

        buffer.push_back(nextChar);
        cout << nextChar;
        decompressedFile.write(&buffer[0], buffer.size());
        buffer.clear();
    }

    cout << endl;

    compressedFile.close();
    decompressedFile.close();
}

bool isFileEmpty(const string& filename) {
    ifstream file(filename);
    return file.peek() == ifstream::traits_type::eof();
}

int main() {
    string inputFile = "input.txt";
    string compressedFilename = "compressed.txt";
    string decompressedFilename = "decompressed.txt";

    if (isFileEmpty(inputFile)) {
        cout << "File is empty. Add an existing non-empty file to the root folder of the project" << endl;
        return 1;
    }

    int windowSize = 4096; // размер словаря
    int bufferSize = 20;   // размер буфера

    compressLZ77(inputFile, compressedFilename, windowSize, bufferSize);

    decompressLZ77(compressedFilename, decompressedFilename);

    return 0;
}
