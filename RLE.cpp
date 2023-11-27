#include <iostream>
#include <vector>

using namespace std;

struct symString {
    char symbol;
    int count;
};

vector<symString> compressRLE(const string& input) {
    vector<symString> result;

    int n = input.length();
    if (n == 0) {
        return result;
    }

    char currentSymbol = input[0];
    int count = 1;

    for (int i = 1; i < n; ++i) {
        if (input[i] == currentSymbol) {
            ++count;
        } else {
            result.push_back({currentSymbol, count});

            currentSymbol = input[i];
            count = 1;
        }
    }

    result.push_back({currentSymbol, count});

    return result;
}

string decompressRLE(const vector<symString>& compressed) {
    string result;

    for (const symString& sym : compressed) {
        for (int i = 0; i < sym.count; ++i) {
            result += sym.symbol;
        }
    }

    return result;
}

int main() {
    string input = "1111111133333444AAAAAGGGG";

    vector<symString> result = compressRLE(input);

    cout << "Сжатый массив: ";
    for (const symString& sym : result) {
        cout << sym.count << sym.symbol;
    }
    cout << endl;

    string decompressed = decompressRLE(result);
    cout << "Распакованный массив: " << decompressed << endl;

    return 0;
}
